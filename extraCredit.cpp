/*******************************************************************************
 * Name          : testrbt.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.1
 * Date          : October 8, 2014
 * Last modified : April 9, 2015
 * Description   : Driver program to test implementation of red-black tree.
 ******************************************************************************/
#include "rbtree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

Tree *rbt;
RedBlackTree<int, int> *rbti;
RedBlackTree<string, string> *rbts;
bool using_ints = false;
const char DELIMITER = ' ';

string inorder_traversal() {
	ostringstream oss;
	oss << "[";
	if (using_ints) {
		RedBlackTree<int, int>::iterator it = rbti->begin();
		while (it != rbti->end()) {
			if (it != rbti->begin()) {
				oss << ", ";
			}
			oss << (*it).first;
			++it;
		}
	} else {
		RedBlackTree<string, string>::iterator it = rbts->begin();
		while (it != rbts->end()) {
			if (it != rbts->begin()) {
				oss << ", ";
			}
			oss << (*it).first;
			++it;
		}
	}
	oss << "]";
	return oss.str();
}

void test_find() {
	if (using_ints) {
		RedBlackTree<int, int>::iterator it = rbti->begin();
		while (it != rbti->end()) {
			int key = (*it).first;
			if (rbti->find(key) == rbti->end()) {
				ostringstream oss;
				oss << "Cannot find key '" << key << "' in tree.";
				throw tree_exception(oss.str());
			}
			++it;
		}
	} else {
		RedBlackTree<string, string>::iterator it = rbts->begin();
		while (it != rbts->end()) {
			string key = (*it).first;
			if (rbts->find(key) == rbts->end()) {
				throw tree_exception("Cannot find key '" + key + "' in tree.");
			}
			++it;
		}
	}
}

/**
 * Used in sorting arrays
 */
bool sort_val(int i, int j){
	return i < j;
}



int main(int argc, char *argv[]) {
	if (argc <= 1 || argc > 3) {
		cout << "Usage: ./commonwordfinder <filename> [limit]";
		return 0;
	}

	// Create an ifstream object.
	ifstream input_file(argv[1]);
	// If it does not exist, print an error message.
	if (!input_file) {
		cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
		return 1;
	}
	// Add read errors to the list of exceptions the ifstream will handle.
	input_file.exceptions(ifstream::badbit);
	string line;
	try {
		unsigned int line_number = 1;
		rbt = new RedBlackTree<string, string>();
		rbts = static_cast<RedBlackTree<string, string> *>(rbt);

		while (getline(input_file, line)) {
			for (int i = 1; i < argc; ++i) {

				istringstream converter;
				vector<string> words;
				size_t start = 0, end = line.find(DELIMITER);
				string inserted_key;
				while (end != string::npos) {
					converter.str(line.substr(start, end - start));
					converter >> inserted_key;
					try {
						static_cast<RedBlackTree<string, string> *>(rbt)->insert(
								inserted_key, inserted_key);
					} catch (const tree_exception &te) {
						RedBlackTree<string, string>::iterator it = rbts->find(
								inserted_key);
						++(*it).second;
					}
					converter.clear();
					start = end + 1;
					end = line.find(DELIMITER, start);
				}
				converter.str(line.substr(start, end));
				converter >> inserted_key;
				converter.clear();
				try {
					static_cast<RedBlackTree<string, string> *>(rbt)->insert(
							inserted_key, inserted_key);
				} catch (const tree_exception &te) {
					RedBlackTree<string, string>::iterator it = rbts->find(
													inserted_key);
											++(*it).second;
				}

			}
		}
		// Don't forget to close the file.
		input_file.close();
	} catch (const ifstream::failure &f) {
		cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
		return 1;
	}

	try {
		test_find();
	} catch (const tree_exception &te) {
		cerr << "Error: " << te.what() << endl;
	}

	//given a vector of nodes, sort them by value first them key if values are equilivalent
	std::sort (rbt.begin(), rbt.end(), sort_val)

	//print the exact amount of words the user wants to see
	for (int w = 1; w < arg[2] || 10; ++w){
		cout << rbt[w-1] << endl;
	}

	delete rbt;
	return 0;
}
