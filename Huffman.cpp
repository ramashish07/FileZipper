#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// A Tree node containing character and its frequency
struct Node{
	char ch;
	int freq;
	Node *left, *right;
	
	// Constructor for the Node
	Node(char ch1, int freq1, Node* l=NULL, Node* r=NULL){
	    ch = ch1;
	    freq = freq1;
	    left = l;
	    right = r;
	}
};

// User-Defined Comparison object to be used to order the heap
struct comp{
	bool operator()(Node* l, Node* r){
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

// Traverse the Huffman Tree and store Huffman Codes in a map.
void encode(Node* root, string str,unordered_map<char, string> &huffmanCode){
	if (root == NULL){
		return;
    }

	// Found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	// Left Subtree Call
	encode(root->left, str + "0", huffmanCode);
	// Right Subtree Call
	encode(root->right, str + "1", huffmanCode);
}

// Traverse the Huffman Tree and Decode the encoded string
void decode(Node* root, int &index, string str){
	if (root == NULL) {
		return;
	}

	// Found a leaf node
	if (!root->left && !root->right){
		cout << root->ch;
		return;
	}

	index++;

	// Moving to left of Tree
	if (str[index] == '0'){
		decode(root->left, index, str);
    }
    // Moving to right of Tree
	else{
		decode(root->right, index, str);
    }
}

// Builds Huffman Tree and decode given input text
Node* buildHuffmanTree(string text){
	// Count frequency of appearance of each character and store it in a map
	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	// Create a priority queue to store live nodes of Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it to the priority queue.
	for (auto pair: freq) {
		pq.push(new Node(pair.first, pair.second));
	}

	// Do till there is more than one node in the queue
	while (pq.size() != 1){
		// Remove the two nodes of highest priority (i.e. lowest frequency) 
		// from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(new Node('\0', sum, left, right));
	}
	
	return pq.top();
}

// Huffman coding Algorithm
int main(){
	string text = "Data dominates. If you've chosen the right data structures and organized things well, the algorithms will almost always be self-evident. Data structures, not algorithms, are central to programming.   -Rob Pike";
	
	// root stores pointer to root of Huffman Tree
	Node* root = buildHuffmanTree(text);

	// Traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them...
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	// Huffman Code of each Character of the string
	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair: huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
	}

	// Input String
	cout << "\nOriginal string was :\n" << text << '\n';

	// Print encoded string
	string str = "";
	for (char ch: text) {
		str += huffmanCode[ch];
	}

	cout << "\nEncoded string is :\n" << str << '\n';

	// Traverse the Huffman Tree again and this time
	// decode the encoded string
	int index = -1;
	cout << "\nDecoded string is: \n";
	while (index < (int)str.size() - 1) {
		decode(root, index, str);
	}

	return 0;
}

// Time Complexity : O(nlog(n)) 
