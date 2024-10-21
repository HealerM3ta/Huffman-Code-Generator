#include "HuffmanBase.hpp"
#include <iostream>
#include <map>
#include <stack>
#include "HeapQueue.hpp"

// Function to print the contents of a map
template<typename K, typename V>
void printMap(const std::map<K, V>& map) {
    for (const auto& pair : map) {
        std::cout << "Character: " << pair.first << ", Huffman Code: " << pair.second << std::endl;
    }
}

template<typename E, typename C>
void printPriorityQueue(const HeapQueue<E, C>& priority_queue) {
    HeapQueue<E, C> copyQueue = priority_queue; // Create a copy to avoid modifying the original queue
    std::cout << "Priority Queue Contents:" << std::endl;
    while (!copyQueue.empty()) {
        const E& node = copyQueue.min();
        std::cout << "Character: " << node->getCharacter()
                  << ", Frequency: " << node->getFrequency() << std::endl;
        copyQueue.removeMin();
    }
}

std::string serializedTreeHelper(const HuffmanNode* node) {
    std::string serializedTree;
    if (node == nullptr) {
        return "";
    }

    //Postorder Traversal
    serializedTree += serializedTreeHelper(node->left);
    serializedTree += serializedTreeHelper(node->right);
    if(node->isLeaf()){
        serializedTree += "L" + std::string(1, node->getCharacter());
    }else{
        serializedTree += "B$" + std::string(1, node->getCharacter());
    }
    return serializedTree;
}

// Implementation of HuffmanNode member functions

char HuffmanNode::getCharacter() const {
    return character;
}

size_t HuffmanNode::getFrequency() const {
    return frequency;
}

bool HuffmanNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

bool HuffmanNode::isBranch() const {
    return !isLeaf();
}

bool HuffmanNode::isRoot() const {
    return parent == nullptr;
}

bool HuffmanNode::Compare::operator()(const HuffmanNode &n1, const HuffmanNode &n2) const {
    return lessThan ? n1.frequency < n2.frequency : n1.frequency > n2.frequency;
}

bool HuffmanNode::Compare::operator()(const HuffmanNode *n1, const HuffmanNode *n2) const {
    return lessThan ? n1->frequency < n2->frequency : n1->frequency > n2->frequency;
}

//Added function to compare frequencies
bool Compare_freq(HuffmanNode* l, HuffmanNode *r){
    if (l->getFrequency() == r->getFrequency()){
        return l->getCharacter() > r->getCharacter();
    }else{
    return l->getFrequency() < r->getFrequency();
    }
}

//code to add 0 and 1 while traversing trees
void generate_code(const HuffmanNode* node, std::string code, std::map<char, std::string>& HuffmanCode) {
    if (node == nullptr){
        return;
    }

    //Reached a leaf node
    if(node->isLeaf()){
        HuffmanCode[node->getCharacter()] = code;
    }

    //Go left (add 0)
    generate_code(node->left, code + '0', HuffmanCode);

    //Go right (add  1)
    generate_code(node->right, code + '1', HuffmanCode);
}

// Implementation of HuffmanTreeBase member functions

std::string HuffmanTreeBase::compress (const std::string inputStr) {
        // Frequency map to store character frequencies
    std::map<char, int> frequency_map;

    // Iterate over each character in the inputStr
    for (char c : inputStr) {
        // If "c" is in the map, add one to its frequency
        if(frequency_map.find(c) != frequency_map.end()) {
            frequency_map[c]++;
        } else {
            // If not, add it to the map with a frequency of 1
            frequency_map[c] = 1;
        }
    }

    // Priority Queue to store HuffmanNodes
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> priority_queue;

    for (const auto& pair : frequency_map) {
        HuffmanNode* newNode = new HuffmanNode(pair.first, pair.second);

        //Insert into priority queue
        priority_queue.insert(newNode);
    }

    printPriorityQueue(priority_queue);

    //Make into binary tree
    while(priority_queue.size()>1) {
        HuffmanNode* removed1 = priority_queue.min();
        priority_queue.removeMin();

        HuffmanNode* removed2 = priority_queue.min();
        priority_queue.removeMin();

        HuffmanNode* Parent = new HuffmanNode('\0', removed1->getFrequency() + removed2->getFrequency(), nullptr, removed1, removed2);
        removed1->parent = Parent;
        removed2->parent = Parent;

        priority_queue.insert(Parent);
    }
    root = priority_queue.min();

    std::string code;
    std::map<char, std::string> HuffmanCode;
    generate_code(root, code, HuffmanCode);

    printMap(HuffmanCode);

    std::string compressedString;
    for(char c : inputStr) {
        compressedString += HuffmanCode[c];
    }

    return compressedString;
}

std::string HuffmanTreeBase::serializeTree() const {
    return serializedTreeHelper(root);
}

std::string HuffmanTreeBase::decompress(const std::string inputCode, const std::string serializedTree) {
    std::string decodedString;
    const HuffmanNode* current = root;

    // Traverse the Huffman tree based on the encoded string
    for (char bit : inputCode) {
        if (bit == '0') {
            // Move to the left child for '0'
            current = current->left;
        } else if (bit == '1') {
            // Move to the right child for '1'
            current = current->right;
        }

        // If a leaf node is reached, append the corresponding character to the decoded string and reset to the root
        if (current->isLeaf()) {
            decodedString += current->getCharacter();
            current = root;
        }
    }

    return decodedString;
}

int main() {
    // Test input string
    std::string inputStr = "abcdef";

    // Create a Huffman tree instance
    HuffmanTreeBase huffmanTree;

    // Compress the input string
    std::string compressedString = huffmanTree.compress(inputStr);
    std::cout << "Compressed string: " << compressedString << std::endl;

    // Serialize the Huffman tree
    std::string serializedTree = huffmanTree.serializeTree();
    std::cout << "Serialized tree: " << serializedTree << std::endl;

    // Decompress the compressed string using the serialized tree
    std::string decompressedString = huffmanTree.decompress(compressedString, serializedTree);
    std::cout << "Decompressed string: " << decompressedString << std::endl;

    return 0;
}
