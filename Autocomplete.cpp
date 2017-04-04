//I Kevin Jacobson have not used any code other than my own(or that in the textbook) for this project.I
//also have not used any function or data - structure from the Standard - Template Library.I
//understand that any violation of this disclaimer will result in a 0 for the project.

#include "term.h"

using namespace std;

//Test Cases can be found at the bottom of this file
void test_open(const char* file);
void test_erase(string word, const char* file);
void test_print(const char* file);
void test_destructor(const char* file);
void test_prefixTree(const char* file, const char* _check, int num);


int main(int argc, char **argv) {
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " FILE" << endl;
		return EXIT_FAILURE;
	}
	int max = stoi(argv[2]);

	//**Test Cases have been commented out so they don't interfere with the grading/testing process.**//
	//**Uncomment test cases to view coverage**//
	//test_open(argv[1]);
	//test_destructor(argv[1]);
	//test_prefixTree(argv[1], argv[3], max);
	//test_erase( "u", argv[1]);
	//test_print(argv[1]);

	//Create a new tree and read in all data to it
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* alphaTree = new BinarySearchTree(comp);
	process_file(argv[1], alphaTree);
	
	//Perform the autocomplete on any number of inputs. Only one quantity of results can be specified for all inputs.
	for (int i = 3; i < argc; i++) {
		string check = argv[i];
		int length = check.length();
		cout << check << endl;
		//Creates new tree for each input and sorts by weight
		NaturalOrder* prefix = new NaturalOrder;
		BinarySearchTree prefixTree;
		//Traverses the original tree looking for any value that matches the requested input and builds the new tree with those values
		alphaTree->CompareEqual(check, length, prefixTree);
		//outputs the requested results in order sorted by weight
		prefixTree.PrefixPrint(max);
	}
}

//Method: test_open
//Parameters: const char* file
//Returns void
//Unit test. Tests the ability to open a file and read in data. Also tests the insert functions and size functions for the tree.
void test_open(const char* file) {
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* alphaTree = new BinarySearchTree(comp);
	assert(process_file(file, alphaTree));
	assert(alphaTree->Size() > 0);
	alphaTree->~BinarySearchTree();
	cout << "Passed Open + Create Tree Test" << endl;
}

//Method: test_erase
//Parameters: string word, const char* file
//returns: none
//tests the ability to erase a node anywhere in the tree and keep the contents in order. Also searches the tree to check if
//the node still exists.
void test_erase(string word, const char* file) {
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* testTree = new BinarySearchTree(comp);
	assert(!testTree->Erase("x"));
	process_file(file, testTree);
	assert(testTree->Erase(word));
	assert(testTree->search(word) != true);
	testTree->~BinarySearchTree();
	Term b;
	b.data = "v";
	b.weight = 45;
	testTree->Insert(b);
	assert(testTree->Erase("v"));
	assert(testTree->search("v") != true);
	testTree->~BinarySearchTree();
	cout << "Passed Erase Test" << endl;
}


//Method: test_print
//Parameters: const char* file
//Returns: none
//tests the ability to print the entire tree.
void test_print(const char* file) {
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* testTree = new BinarySearchTree(comp);
	assert(!testTree->Print());
	process_file(file, testTree);
	assert(testTree->Print());
	testTree->~BinarySearchTree();
	cout << "Passed Print Test" << endl;
}

//Method: test_destructor
//Parameters: const char* file
//Returns: none
//tests to make sure the destructor deletes the tree completely.
void test_destructor(const char* file) {
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* testTree = new BinarySearchTree(comp);
	process_file(file, testTree);
	testTree->~BinarySearchTree();
	assert(!testTree->Print());
	cout << "Passed Destructor Test" << endl;
}

//Method: test_prefixTree
//Parameters: const char* file, const char* _check, int num
//Returns: none
//Tests the autocomplete functionality. Creates a tree and reads in contents from a file, then traverses that tree and builds
//a second tree with only contents that match our autocomplete, sorted by weight. It will then display the highest weighted 
//results in the requested quantity.
void test_prefixTree(const char* file, const char* _check, int num) {
	NaturalOrder *comp = new NaturalOrder;
	BinarySearchTree* testTree = new BinarySearchTree(comp);
	process_file(file, testTree);
	NaturalOrder* prefix = new NaturalOrder;
	BinarySearchTree testTree2;
	string check = _check;
	int length = check.length();
	testTree->CompareEqual(check, length, testTree2);
	testTree2.PrefixPrint(num);
	assert(testTree2.Size() > 0);
	testTree->~BinarySearchTree();
	cout << "Passed Prefix Tree Test" << endl;
}