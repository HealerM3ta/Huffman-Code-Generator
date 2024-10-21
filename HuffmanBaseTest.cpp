#include "HuffmanBase.hpp"
#include <iostream>
#include <map>
#include <stack>
#include "HeapQueue.hpp"

//Global root for the seralizeTree
const HuffmanNode* globalroot = nullptr;

std::string serializeTreeHelper(const HuffmanNode* node) {
    std::string serialTree;
    if (node == nullptr) {
        return serialTree;
    }

    // Postorder traversal to serialize the tree
    serialTree += serializeTreeHelper(node->left);
    serialTree += serializeTreeHelper(node->right);
    if (node->isLeaf()) {
        serialTree += "L" + std::string(1, node->getCharacter());
    } else {
        serialTree += "B$";
    }

    return serialTree;
}

void printMap(const std::map<char, std::string>& HuffmanCode) {
    for (const auto& pair : HuffmanCode) {
        std::cout << "Character: " << pair.first << ", Code: " << pair.second << std::endl;
    }
}
void printTree(const HuffmanNode* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    // Print the current node
    std::cout << std::string(depth, ' ') << "- ";
    if (node->isLeaf()) {
        std::cout << "Leaf: " << node->getCharacter() << std::endl;
    } else {
        std::cout << "Branch" << std::endl;
    }

    // Recursive call for left and right subtrees
    printTree(node->left, depth + 2);
    printTree(node->right, depth + 2);
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
        return l->getCharacter() < r->getCharacter();
    }else{
    return l->getFrequency() > r->getFrequency();
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
    globalroot = priority_queue.min();

    std::string code;
    std::map<char, std::string> HuffmanCode;
    generate_code(globalroot, code, HuffmanCode);

    printMap(HuffmanCode);

    std::string compressedString;
    for(char c : inputStr) {
        compressedString += HuffmanCode[c];
    }

    return compressedString;
}

std::string HuffmanTreeBase::serializeTree() const {
    if (globalroot == nullptr) {
        return "";
    }
    return serializeTreeHelper(globalroot);
}

std::string HuffmanTreeBase::decompress(const std::string inputCode, const std::string serializedTree) {
    std::string decodedString;
    return decodedString;
}











int main() {
    // Create a Huffman tree instance
    HuffmanTreeBase huffmanTree;

    // Test input string
    std::string inputStr = "if a machine is expected to be infallible it cannot also be intelligent";

    // Compress the input string
    std::string compressedString = huffmanTree.compress(inputStr);
    std::cout << "Compressed string: " << compressedString << std::endl;
    std::cout << "Expected string: " << "010011010010100001111010101101001111101011111000001001100001000111000111011001101011101001011010011101011100110111000001011110110110101100110001011011110010000010111000110101010111111111011111100010101100011001011100110111000001011111110100110011000101011001001111110" <<std::endl;

    // Serialize the Huffman tree
    std::string serializedTree = huffmanTree.serializeTree();
    std::cout << "Serialized tree: " << serializedTree << std::endl;

    // Decompress the compressed string using the serialized tree
    std::cout << "Decompress Initializer" << std::endl;
    std::string decompressedString = huffmanTree.decompress("010011010010100001111010101101001111101011111000001001100001000111000111011001101011101001011010011101011100110111000001011110110110101100110001011011110010000010111000110101010111111111011111100010101100011001011100110111000001011111110100100110001010110010011111110", "L LiLsLfB$LxLpB$LmLhB$B$B$B$B$LeLaLgLdB$LoB$B$B$LlLcLbB$B$LtLnB$B$B$B$");
    std::cout << "Decompressed string: " << decompressedString << std::endl;

    return 0;
}