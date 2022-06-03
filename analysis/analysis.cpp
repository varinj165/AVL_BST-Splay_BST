#include "../avl/avlbst.h"
#include "../splay/splay.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please provide an input and output file." << endl;
        return 1;
    }

    ifstream input(argv[1]);

    if (input.fail()) {
    	cout << "File " << argv[1] << " cannot be opened." << endl;
    	return 1;
    }

    vector<string> strings;

    string newString;

    // Read in all strings from file into vector
    while (!input.fail()) {
    	input >> newString;

    	strings.push_back(newString);
    }

    clock_t start;
    double AVLduration;

    double totalDuration;

    // To keep track of number of operations
    int i = 0;

    // Start timing
    start = clock();

    // Repeat AVL insert algorithm 100 times
    while (i < 100) {
    	// Create AVL Tree
   		AVLTree<string, int>* avl = new AVLTree<string,int>();

    	for (unsigned int j = 0; j < strings.size(); ++j) {
    		// Create pair to insert
    		pair<string, int> node;

    		node.first = strings[j];
    		node.second = 0;

    		avl->insert(node);
    	}

    	++i;
    }

    // Total duration for all 100 repititions
    totalDuration = (clock() - start) / (double) CLOCKS_PER_SEC;

    // Duration for one repitions of AVL inserts
    AVLduration = totalDuration / 100.0;



    // Splay Analysis

    i = 0;

    double splayDuration;

    // Start timing
    start = clock();

    int badInserts;

    // Repeat Splay insert algorithm 100 times
    while (i < 100) {
    	// Create Splay Tree
    	SplayTree<string, int>* splay = new SplayTree<string,int>();

    	for (unsigned int j = 0; j < strings.size(); ++j) {
    		// Create pair to insert
    		pair<string, int> node;

    		node.first = strings[j];
    		node.second = 0;

    		splay->insert(node);
    	}

    	// No. of bad inserts will be same for every repitition
    	if (i == 0) {
    		badInserts = splay->report();
    	}

    	++i;
    }

    // Total duration for all 100 repititions
    totalDuration = (clock() - start) / (double) CLOCKS_PER_SEC;

    // Duration for one repition of Splay inserts
    splayDuration = totalDuration / 100.0;


    // Write to output file

    ofstream output(argv[2]);

    output << strings.size() - 1 << " insertions" << endl;

    output << "AVL: " << AVLduration << " seconds" << endl;

    output << "Splay: " << splayDuration << " seconds" << endl;

    output << "Splay expensive: " << badInserts << endl;

    input.close();

    output.close();

    return 0;
}