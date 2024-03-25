#include <iostream>
#include <fstream>
#include <unordered_map> // c++ hash table
#include <vector>
#include <string>
#include <queue> // for priority queue

using namespace std;

struct Node
{
    char data;
    int frequency;
    Node* left; // pointer to left child
    Node* right; // pointer to right child

    Node(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

string parseFile(string & filename)
{
    ifstream file(filename);
    string result;
    string line;

    if (getline(file, line))
    {
        result = line;
    }

    return result;

}

// hash table, tracks frequency of characters
unordered_map<char, int> count_frequency(string & s)
{
    unordered_map<char, int> frequency;
    for (char c : s)
    {
        if(frequency.find(c) != frequency.end()) //if char c is in hash table, increase frequency int
        {
            frequency[c]++; 
        }
        else
        {
            frequency[c] = 1;
        }
    }

    return frequency;

}

bool compareNodes(Node* a, Node* b)
{

    // calculating the differnce between left and right frequencies
    int balance_a = (a->left ? a->left->frequency : 0) - (a->right ? a->right->frequency : 0);
    int balance_b = (b->left ? b->left->frequency : 0) - (b->right ? b->right->frequency : 0);

   // Compare nodes based on frequency and balance factor
    if (a->frequency != b->frequency)
    {
        return a->frequency > b->frequency; // Higher frequency has higher priority
    }
    else
    {
        return balance_a < balance_b; // Lower balance factor has higher priority
    }
}

void printHuffmanTree(Node* root, int depth = 0) {
    if (root == nullptr)
        return;

    // Print right child
    printHuffmanTree(root->right, depth + 1);

    // Indent based on depth
    for (int i = 0; i < depth; ++i)
        cout << "    ";

    // Print node
    if (root->data == '\0')
        cout << "Internal: " << root->frequency << endl;
    else
        cout << "Leaf: " << root->data << " (" << root->frequency << ")" << endl;

    // Print left child
    printHuffmanTree(root->left, depth + 1);
}

// returns root of huffman tree
Node* huffman(unordered_map<char, int>& char_frequency)
{
    // setting up a prioity queue that sorts the nodes from least frequency to greatest frequence
    // decltype : specifys what type of comparision, in this context it is Node pointer
    // pq : priority queue that holds Node pointers, contains them into vector<Node*>
    priority_queue<Node*, vector<Node*>, decltype(&compareNodes)> pq(&compareNodes);

    // makes new leaf, .first = key, .second = value from hashtable
    for (auto & pair : char_frequency)
    {
        Node* leaf = new Node(pair.first, pair.second);
        pq.push(leaf); 
    }

     // Build Huffman tree
    while (pq.size() > 1) 
    {
        // taking smallest value frequency nodes and removing them from priority queue via .pop()
        Node* left = pq.top(); 
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        // Create a new internal node with frequency equal to the sum of frequencies of left and right children
        Node* internal = new Node('\0', left->frequency + right->frequency);
        internal->left = left;
        internal->right = right;

        // calculate the balance factor for the internal node, use to make the tree balanced
        int balance_factor = abs(left->frequency - right->frequency);

        pq.push(internal); // Add the new internal node back to the priority queue
    }

    // The remaining node in the priority queue is the root of the Huffman tree
    return pq.top();
}

// returns the hash table of the character and it's huffmancode, given a root
unordered_map<char, string> getHuffmanCodes(Node* root, string code = "")
{
    unordered_map<char,string> huffmanCodes;

    if (root == nullptr)
    {
        return huffmanCodes;
    }

    //leaf node
    if (root -> data != '\0')
    {
        huffmanCodes[root->data] = code;
    }

    // traverse left with code '0', merge returned maps recurssively
    unordered_map<char, string> leftCodes = getHuffmanCodes(root->left, code + "0");
    huffmanCodes.insert(leftCodes.begin(), leftCodes.end());

    // traverse right with code '1', merge retruned maps recurssively
    unordered_map<char, string> rightCodes = getHuffmanCodes(root->right, code + "1");
    huffmanCodes.insert(rightCodes.begin(), rightCodes.end());

    return huffmanCodes;
}


int findBits(unordered_map<char, string>& huffmanCodes, unordered_map<char, int>& char_frequency)
{
    int totalBits = 0;
    for (auto & pair : huffmanCodes)
    {  
        string& huffman_code = pair.second;
        int frequency = char_frequency.at(pair.first); // gets frequency at given pair character
        totalBits += frequency * huffman_code.size(); // frequency * length of huffman code = total bits
    }
    return totalBits;
}



int main()
{
    string filename;
    cout << "Enter filename (.txt): ";
    cin >> filename;
    cout << endl;

    string fileString = parseFile(filename);
    unordered_map<char, int> char_frequency = count_frequency(fileString);

    Node* root = huffman(char_frequency);
    unordered_map<char, string> codes = getHuffmanCodes(root);
    
    int totalBits = findBits(codes, char_frequency);
 
    cout << "Total bits : " << totalBits << endl;


    return 0;
}