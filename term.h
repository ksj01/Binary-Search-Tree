//I Kevin Jacobson have not used any code other than my own(or that in the textbook) for this project.I
//also have not used any function or data - structure from the Standard - Template Library.I
//understand that any violation of this disclaimer will result in a 0 for the project.

#ifndef BINARY_SEARCH_TREE__TERM_H
#define BINARY_SEARCH_TREE__TERM_H

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

using namespace std;

// The term class represents an auto complete term: a string and associated weight.
struct Term {
  uint64_t weight;
  string data;
};

//Overloaded << operator prints both the word and its respective weight
std::ostream& operator<<(std::ostream& out, const Term& stuff)
{
	out << stuff.data << ", " << stuff.weight;
	return out;
}

// Parse a Term from a stream.
std::istream &operator>>(std::istream &is, Term &e) {
  is >> e.weight >> std::ws;
  std::getline(is, e.data);
  return is;
}

// Interface for sorting Terms.
class TermOrder {
    public:
    virtual ~TermOrder() { };
    // Compare returns <0 if a < b, 0 if a == b, or >0 if a > b.
    virtual int Compare(const Term& a, const Term & b) = 0;
};

// Compares two terms by string in ascending order.
class NaturalOrder : public TermOrder {
  public:
  int Compare(const Term& a, const Term& b) {
      return a.data.compare(b.data);
  }
};

//Class: Node
//Holds the data for each leaf of the tree
class Node {
private:

public:
	Node* left;
	Node* right;
	Term data;
	Node() {
	}
	Node(Term b) {
		Node* newRoot = new Node;
		newRoot->data.data = b.data;
		newRoot->data.weight = b.weight;
	}
	void operator = (Term& x) {
		data.data = x.data;
		data.weight = x.weight;
	}
};

//Class: BinarySearchTree
//Contains all data and functions necessary to build trees, traverse them, compare data, and output Autocomplete results
class BinarySearchTree {
	private:
		Node* root = NULL;
		TermOrder* compareObject;
		int size = 0;
	public:

		BinarySearchTree() {
		}
		BinarySearchTree(TermOrder* comp) {
			compareObject = comp;
		}
		~BinarySearchTree() {
			delete(root);
			this->root = NULL;
			root = NULL;
		}

		//Method: Size
		//Parameters: None
		//Returns: size
		//returns the total number of data values in the tree
		int Size() {
			return size;
		}

		//Method: PrefixPrintNode
		//Parameters: Node* _root, int& _count, int _max, int _depth
		//returns: nothing
		//Helper function for PrefixPrint(). Recursively traverses the tree and prints only the requested number of data 
		//with an indentation of 4 spaces*the number of levels deep the value is
		void PrefixPrintNode(Node* _root, int &_count, int _max, int _depth) {
			if (_root->left != NULL) {
				PrefixPrintNode(_root->left, _count, _max, _depth + 1);
			}
			if (_count < _max) {
				//Indentation method
				cout << setw(4 * _depth) << "";
				cout << _root->data << endl;
				_count++;
			}
			if (_root->right != NULL) {
				PrefixPrintNode(_root->right, _count, _max, _depth + 1);
			}
		}

		//Method: PrefixPrint
		//Parameters: int _max
		//Returns: nothing
		//initializes the count and depth variables used by the helper function. Calls the helper function to print
		//requested autocomplete data
		void PrefixPrint(int _max) {
			int count = 0;
			int depth = 0;
			if (root != NULL) {
				PrefixPrintNode(root, count, _max, depth);
			}
		}

		//Method: PrintNode
		//Parameters: Node* _root, int _depth
		//Returns: none
		//Helper function for Print(). Traverses the tree and prints all data with an indentation
		//of 4 spaces*depth of each leaf. This method print ALL data in alphabetical order, starting with A and ending with Z.
		void PrintNode(Node* _root, int _depth) {
			if (_root->left != NULL) {
				PrintNode(_root->left, _depth + 1);
			}
			cout << setw(4 * _depth) << "";
			cout << _root->data << endl;
			if (_root->right != NULL) {
				PrintNode(_root->right, _depth + 1);
			}
		}

		//Method: Print
		//Parameters: none
		//Returns: bool (false if the tree is empty, true otherwise
		//Initializes the depth variable used by the helper function, calls PrintNode().
		bool Print() {
			if (root != NULL) {
				int depth = 0;
				PrintNode(root, depth);
				return true;
			}
			return false;
		}

		//Method: EraseHelper
		//Parameters: string _check, Node* _root, Term& _temp, int& _farRight
		//returns: none
		//Helper function of Erase(). Traverses the tree and deletes the requested Node. If there is data
		//attached to the node being deleted, it will find the right-most leaf on its first left branch and
		//replace it. This function will also move data up if there is a branch attached to the right-most leaf on the first left
		//branch of the data being deleted.
		bool EraseHelper(string _check, Node* _root, Term& _temp, int &_farRight) {
			//_farRight is used for when the tree needs to be traversed looking for replacement data
			if (_farRight == 0) {
				if (_root->left != NULL) {
					EraseHelper(_check, _root->left, _temp, _farRight);

				}
				//Compare node with data requested for deletion.
				string compareWord = _root->data.data;
				if (compareWord.compare(_check) == 0) {
					//If no children, delete current Node.
					if (_root->left == NULL && _root->right == NULL) {
						delete _root;
						_root = NULL;
						this->root = NULL;
						return true;
					}
					//If children exist, find replacement node.
					else {
						_farRight = 1;
						EraseHelper(_check, _root->left, _temp, _farRight);
						_root->data.data = _temp.data;
						_root->data.weight = _temp.weight;
						return true;
					}
				}
				if (_root->right != NULL) {
					EraseHelper(_check, _root->right, _temp, _farRight);
				}
			}

			//This portion is used exclusively to find the replacement node.
			else {
				if (_root->right->right != NULL) {
					EraseHelper(_check, _root->right, _temp, _farRight);
				}
				else {
					_temp = _root->right->data;
					if (_root->right->left != NULL) {
						_root->right = _root->right->left;
					}
					else {
						delete _root->right;
						_root->right = NULL;
					}
					_farRight = 0;
				}
			}
		}

		//Method: Erase
		//Parameters: string _check
		//Returns: bool
		//Creates the _farRight data and Temp data used by the helper function. Calls EraseHelper().
		bool Erase(string _check) {
			if (root != NULL) {
				int farRight = 0;
				Term temp;
				return EraseHelper(_check, root, temp, farRight);
			}
			return false;
		}

		//Method: Insert
		//Parameters: Term b
		//returns: none
		//Check to see if the tree is empty. If so, creates the first node. If not, calls InsertHelper().
		void Insert(Term b) {
			if (root == NULL) {
				root = new Node;
				root->data = b;
				size++;
			}
			else {
				InsertHelper(root, b);
			}
		}

		//Method: InsertHelper
		//Parameters: Node* a, Term b)
		//Returns: none
		// Traverses the tree and compares Term b to each value until it finds where it should go.
		void InsertHelper(Node* a, Term b) {
			if (compareObject->Compare(a->data, b) > 0) {
				if (a->left == NULL) {
					Node* aLeft = new Node;
					aLeft->data = b;
					a->left = aLeft;
					size++;
				}

				else {
					InsertHelper(a->left, b);
				}
			}
			else if (compareObject->Compare(a->data, b) < 0) {
				if (a->right == NULL) {
					Node* aRight = new Node;
					aRight->data = b;
					a->right = aRight;
					size++;
				}
				else {
					InsertHelper(a->right, b);
				}
			}
			else {
			}
		}

		//Method: PrefixOrderHelper
		//Parameters: Node* a, Term b, BinarySearchTree& _prefix
		//Returns: none
		//Traverses the tree and sorts Term b into the tree by weight, descending (left-most is highest, right-most is lowest)
		void PrefixOrderHelper(Node* a, Term b, BinarySearchTree &_prefix) {
			if (a->data.weight <= b.weight) {
				if (a->left == NULL) {
					Node* aLeft = new Node;
					aLeft->data = b;
					a->left = aLeft;
					_prefix.size++;
				}

				else {
					PrefixOrderHelper(a->left, b, _prefix);
				}
			}
			else if (a->data.weight > b.weight) {
				if (a->right == NULL) {
					Node* aRight = new Node;
					a->right = aRight;
					aRight->data = b;
					_prefix.size++;
				}
				else {
					PrefixOrderHelper(a->right, b, _prefix);
				}
			}
		}

		//Method: PrefixOrder
		//Parameters: Term b, BinarySearchTree &_prefix
		//Returns: none
		//If prefix tree is empty, add Term b as root. Otherwise, call PrefixOrderHelper
		void PrefixOrder(Term b, BinarySearchTree &_prefix) {
			if (_prefix.root == NULL) {
				_prefix.root = new Node;
				_prefix.root->data = b;
				_prefix.size++;
			}
			else {
				PrefixOrderHelper(_prefix.root, b, _prefix);
			}
		}

		//Method: CompareEqualHelper
		//Parameters: string _check, int _length, Node* _root, BinarySearchTree& _prefix
		//Returns: none
		//Traverses the tree and compares the requested autocomplete word against every value in the tree. If there's a match,
		//it calls PrefixOrder which will add it to the new tree
		void CompareEqualHelper(string _check, int _length, Node* _root, BinarySearchTree &_prefix) {
			if (_root->left != NULL) {
				CompareEqualHelper(_check, _length, _root->left, _prefix);
			}
			string compareWord = _root->data.data;
			if (compareWord.compare(0, _length, _check) == 0) {
				PrefixOrder(_root->data, _prefix);
			}
			if (_root->right != NULL) {
				CompareEqualHelper(_check, _length, _root->right, _prefix);
			}
		}
		
		//Method: CompareEqual
		//Parameters: string _check, int _length, BinarySearchTree &_prefix
		//Returns: Nothing
		//Check to make sure the original tree isn't empty. If it's not, call CompareEqualHelper
		void CompareEqual(string _check, int _length, BinarySearchTree &_prefix) {
			if (root != NULL) {
				CompareEqualHelper(_check, _length, root, _prefix);
			}
		}

		//Method: SearchHelper
		//Parameters: Node* _root, string _check
		//Returns: bool
		//Traverses the tree to check if a value exists. If so, return true.
		bool searchHelper(Node* _root, string _check) {
			if (_root->left != NULL) {
				return searchHelper(_root->left, _check);
			}
			string compareWord = _root->data.data;
			if (compareWord.compare(_check) == 0) {
				return true;
			}
			if (_root->right != NULL) {
				searchHelper(_root->right, _check);
			}
		}
		
		//Method: search
		//Parameters: string _check
		//returns: bool
		//If the tree exists, call searchHelper().
		bool search(string _check) {
			if (root != NULL) {
				return searchHelper(root, _check);
			}
			return false;
		}
};

//Method: process_file
//Parameters: const char* filename, BinarySearchTree* in
//Returns: bool
//Opens the file to read in data, parses each line into a Term, calls the Insert() function to add each line to a tree. Returns
//true if all actions are successful.
bool process_file(const char* filename, BinarySearchTree* in) {
	ifstream input;
	input.open(filename);
	if (!input.is_open()) {
		return false;
	}
	//BinarySearchTree* in = new BinarySearchTree(comp);
	
	 //read from input...
	while (input.good() && input.peek() != EOF) {
		Term t;
		input >> t;
		in->Insert(t);
	}
	input.close();
	return true;
}

#endif // BINARY_SEARCH_TREE__TERM_H
