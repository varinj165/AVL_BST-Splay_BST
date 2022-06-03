#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include "../bst/bst.h"

/**
* A templated binary search tree implemented as a Splay tree.
*/
template <class Key, class Value>
class SplayTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods.
	SplayTree();
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);
	int report() const;

private:
	/* You'll need this for problem 5. Stores the total number of inserts where the
	   node was added at level strictly worse than 2*log n (n is the number of nodes
	   including the added node. The root is at level 0). */
	int badInserts;

	int n; // To keep track of number of nodes

	/* Helper functions are encouraged. */

	// These are same as AVL rotations
	void LRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);
	void RLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);

	// These will be used in SplayLL and SplayRR rotations
	void AVLLLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);
	void AVLRRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);

	// These will be used when node to splay is child of root or in SplayLL andSplayRR
	// Rotations
	void leftSingleRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);
	void rightSingleRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);

	// These are splay zig zig rotations
	void SplayLLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);
	void SplayRRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z);

	// Returns last accessed leaf node if key is not in tree
	Node<Key, Value>* findHelper(const Key& key);
};

/*
--------------------------------------------
Begin implementations for the SplayTree class.
--------------------------------------------
*/

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree() : badInserts(0) {
	n = 0;
}

template<typename Key, typename Value>
int SplayTree<Key, Value>::report() const {
	return badInserts;
}

/**
* This function will perform AVL LL Rotation then a 
* right single splay rotation 
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::SplayLLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	AVLLLRotation(x, y, z);

	rightSingleRotation(x, y, z);
}

/**
* This function will perform AVL RR Rotation then a 
* left single splay rotation 
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::SplayRRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	AVLRRRotation(x, y, z);

	leftSingleRotation(x, y, z);
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::rightSingleRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// If node to be splayed is a child of the root
	if (z == NULL) {
		// If x has a right child
		if (x->getRight() != NULL) {
			Node<Key, Value>* xRight = x->getRight();

			// Set to y's left child
			y->setLeft(xRight);
			xRight->setParent(y);
		}

		// If x does not have a right child
		else {
			y->setLeft(NULL);
		}

		// Make x root with y as right child
		x->setRight(y);
		y->setParent(x);

		x->setParent(NULL);

		// Set x as root
		BinarySearchTree<Key, Value>::mRoot = x;
	}

	// If this rotation is part of the SplayLL Rotation
	else {
		// If x has a right child
		if (x->getRight() != NULL) {
			// Set to y's left child
			y->setLeft(x->getRight());
			x->getRight()->setParent(y);
		}

		// If x does not have a right child
		else {
			y->setLeft(NULL);
		}

		// Make y x'ss right child
		x->setRight(y);

		// Set x's parent to y's parent
		x->setParent(y->getParent());

		// Set y's parent to x
		y->setParent(x);

		// If x is already root
		if (x->getParent() == NULL) {
			BinarySearchTree<Key, Value>::mRoot = x;

			return;
		}

		// If y was previously the left child of it's parent
		if (x->getParent()->getLeft() == y) {
			x->getParent()->setLeft(x);
		}

		// If y was previously the right child of it's parent
		else {
			x->getParent()->setRight(x);
		}
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::leftSingleRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// If node to be splayed is a child of the root
	if (z == NULL) {
		// If x has left child
		if (x->getLeft() != NULL) {
			// Set to y's right child
			y->setRight(x->getLeft());
			x->getLeft()->setParent(y);
		}

		// If x does not have a left child
		else {
			y->setRight(NULL);
		}

		// Make x root with y as right child
		x->setLeft(y);
		y->setParent(x);

		x->setParent(NULL);

		// Set x as root
		BinarySearchTree<Key, Value>::mRoot = x;
	}

	// If this rotation is part of the SplayRR Rotation
	else {
		// If x has left child
		if (x->getLeft() != NULL) {
			// Set to y's right child
			y->setRight(x->getLeft());
			x->getLeft()->setParent(y);
		}

		// If x does not have a left child
		else {
			y->setRight(NULL);
		}

		// Make y x's left child
		x->setLeft(y);

		// Set x's parent to y's parent
		x->setParent(y->getParent());

		// Set y's parent to x
		y->setParent(x);

		// If x is already root
		if (x->getParent() == NULL) {
			BinarySearchTree<Key, Value>::mRoot = x;

			return;
		}

		// If y was previously the left child of it's parent
		if (x->getParent()->getLeft() == y) {
			x->getParent()->setLeft(x);
		}

		// If y was previously the right child of it's parent
		else {
			x->getParent()->setRight(x);
		}
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::AVLRRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// if Z has no parent
	if (z->getParent() == NULL) {
		// Save y's left child if it exists 
		if (y->getLeft() != NULL) {
			Node<Key, Value>* yLeft = y->getLeft();
			// Set z's right child to y's left child
			z->setRight(yLeft);

			// Set yLeft's parent to z
			yLeft->setParent(z);
		}

		else {
			z->setRight(NULL);
		}

		// Set y's left child to z
		y->setLeft(z);

		// Set z's parent to y
		z->setParent(y);

		// Set y's parent to null
		y->setParent(NULL);

		// Set y to root
		BinarySearchTree<Key, Value>::mRoot = y;
	}

	// If Z has a parent
	else {
		// Save y's left child if it exists 
		if (y->getLeft() != NULL) {
			Node<Key, Value>* yLeft = y->getLeft();
			// Set z's right child to y's left child
			z->setRight(yLeft);

			// Set yLeft's parent to z
			yLeft->setParent(z);
		}

		else {
			z->setRight(NULL);
		}

		Node<Key, Value>* zParent = z->getParent();

		// Set y's left child to z
		y->setLeft(z);

		// Set z's parent to y
		z->setParent(y);

		// Set y's parent to z's parent
		y->setParent(zParent);

		// If z was the left child of it's parent
		if (zParent->getLeft() == z) {
			// Set y to be left child of z's previous parent
			zParent->setLeft(y);
		}

		// If z was the right child of it's parent
		else {
			// Set y to be the right child of z's previous parent
			zParent->setRight(y);
		}
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::AVLLLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// if Z has no parent
	if (z->getParent() == NULL) {
		// Save y's right child if it exists 
		if (y->getRight() != NULL) {
			Node<Key, Value>* yRight = y->getRight();
			// Set z's left child to y's right child
			z->setLeft(yRight);

			// Set yRight's parent to z
			yRight->setParent(z);
		}

		else {
			z->setLeft(NULL);
		}

		// Set y's right child to z
		y->setRight(z);

		// Set z's parent to y
		z->setParent(y);

		// Set y's parent to null
		y->setParent(NULL);

		// Set y to root
		BinarySearchTree<Key, Value>::mRoot = y;
	}

	// If Z has a parent
	else {
		// Save y's right child if it exists 
		if (y->getRight() != NULL) {
			Node<Key, Value>* yRight = y->getRight();
			// Set z's left child to y's right child
			z->setLeft(yRight);

			// Set yRight's parent to z
			yRight->setParent(z);
		}

		else {
			// Set z's left to null if y's right child does not exist
			z->setLeft(NULL);
		}

		Node<Key, Value>* zParent = z->getParent();

		// Set y's right child to z
		y->setRight(z);

		// Set z's parent to y
		z->setParent(y);

		// Set y's parent to z's parent
		y->setParent(zParent);

		// If z was the left child of it's parent
		if (zParent->getLeft() == z) {
			// Set y to be left child of z's previous parent
			zParent->setLeft(y);
		}

		// If z was the right child of it's parent
		else {
			// Set y to be the right child of z's previous parent
			zParent->setRight(y);
		}
	}
}

/**
* This function performs the zig zag rotation
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::LRRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// If z does not have a parent
	if (z->getParent() == NULL) {
		if (x->getLeft() != NULL) {
			// Save x's left child
			Node<Key, Value>* xLeft = x->getLeft();

			// Set y's right to x's left
			y->setRight(xLeft);

			// Set y as x's left's parent
			xLeft->setParent(y);
		}

		else {
			y->setRight(NULL);
		}

		if (x->getRight() != NULL) {
			// Save x's right child
			Node<Key, Value>* xRight = x->getRight();

			// Set z's left to x's right
			z->setLeft(xRight);

			// Set x's right's parent to z 
			xRight->setParent(z);
		}

		else {
			z->setLeft(NULL);
		}

		// Set x's left to y and y's parent to x
		x->setLeft(y);
		y->setParent(x);

		// Set x's right to z and z's parent to x
		x->setRight(z);
		z->setParent(x);

		// Set x's parent to NULL
		x->setParent(NULL);

		// Set x to root
		BinarySearchTree<Key, Value>::mRoot = x;
	}

	// If z does have a parent
	else {
		if (x->getLeft() != NULL) {
			// Save x's left child
			Node<Key, Value>* xLeft = x->getLeft();

			// Set y's right to x's left
			y->setRight(xLeft);

			// Set y as x's left's parent
			xLeft->setParent(y);
		}

		else {
			y->setRight(NULL);
		}

		if (x->getRight() != NULL) {
			// Save x's right child
			Node<Key, Value>* xRight = x->getRight();

			// Set z's left to x's right
			z->setLeft(xRight);

			// Set x's right's parent to z 
			xRight->setParent(z);
		}

		else {
			z->setLeft(NULL);
		}

		// Save z's parent
		Node<Key, Value>* zParent = z->getParent();

		// Set x's left to y and y's parent to x
		x->setLeft(y);
		y->setParent(x);

		// Set x's right to z and z's parent to x
		x->setRight(z);
		z->setParent(x);

		// Set x's parent to z's parent
		x->setParent(zParent);

		// If z was the left child of it's parent
		if (zParent->getLeft() == z) {
			// Set x to be left child of z's previous parent
			zParent->setLeft(x);
		}

		// If z was the right child of it's parent
		else {
			// Set x to be the right child of z's previous parent
			zParent->setRight(x);
		}
	}
}

/**
* This function performs the zig zag rotation
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::RLRotation(Node<Key, Value>* x, Node<Key, Value>* y, Node<Key, Value>* z) {
	// If z does not have a parent
	if (z->getParent() == NULL) {
		if (x->getLeft() != NULL) {
			// Save x's left child
			Node<Key, Value>* xLeft = x->getLeft();

			// Set z's right to x's left
			z->setRight(xLeft);

			// Set z as x's left's parent
			xLeft->setParent(z);
		}

		else {
			z->setRight(NULL);
		}

		if (x->getRight() != NULL) {
			// Save x's right child
			Node<Key, Value>* xRight = x->getRight();

			// Set y's left to x's right
			y->setLeft(xRight);

			// Set x's right's parent to y 
			xRight->setParent(y);
		}

		else {
			y->setLeft(NULL);
		}

		// Set x's left to z and z's parent to x
		x->setLeft(z);
		z->setParent(x);

		// Set x's right to y and y's parent to x
		x->setRight(y);
		y->setParent(x);

		// Set x's parent to NULL
		x->setParent(NULL);

		// Set x to root
		BinarySearchTree<Key, Value>::mRoot = x;
	}

	// If z does have a parent
	else {
		if (x->getLeft() != NULL) {
			// Save x's left child
			Node<Key, Value>* xLeft = x->getLeft();

			// Set z's right to x's left
			z->setRight(xLeft);

			// Set z as x's left's parent
			xLeft->setParent(z);
		}

		else {
			z->setRight(NULL);
		}

		if (x->getRight() != NULL) {
			// Save x's right child
			Node<Key, Value>* xRight = x->getRight();

			// Set y's left to x's right
			y->setLeft(xRight);

			// Set x's right's parent to y 
			xRight->setParent(y);
		}

		else {
			y->setLeft(NULL);
		}

		// Save z's parent
		Node<Key, Value>* zParent = z->getParent();

		// Set x's left to z and z's parent to x
		x->setLeft(z);
		z->setParent(x);

		// Set x's right to y and y's parent to x
		x->setRight(y);
		y->setParent(x);

		// Set x's parent to z's parent
		x->setParent(zParent);

		// If z was the left child of it's parent
		if (zParent->getLeft() == z) {
			// Set x to be left child of z's previous parent
			zParent->setLeft(x);
		}

		// If z was the right child of it's parent
		else {
			// Set x to be the right child of z's previous parent
			zParent->setRight(x);
		}
	}
}

/**
* Insert function for a key value pair. Finds location to insert the node and then splays it to the top.
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	// Insert new node using BST::insert()
	BinarySearchTree<Key, Value>::insert(keyValuePair);

	// Increment number of nodes
	++n;

	// This function will update bad inserts if any
	findHelper(keyValuePair.first);

	// Find newly inserted node in BST
	Node<Key, Value>* insertedNode = BinarySearchTree<Key, Value>::internalFind(keyValuePair.first);

	// Splay inserted node to the root
	while (insertedNode != BinarySearchTree<Key, Value>::mRoot) {
		// If inserted node is a child of the root
		if (insertedNode->getParent() == BinarySearchTree<Key, Value>::mRoot) {
			Node<Key, Value>* y = insertedNode->getParent();

			// If inserted node is the left child
			if (insertedNode->getParent()->getLeft() == insertedNode) {
				// Perform right single rotation
				rightSingleRotation(insertedNode, y, NULL);
			}

			// If inserted node is right child
			else {
				// Perform left single rotation
				leftSingleRotation(insertedNode, y, NULL);
			}
		}

		// If inserted node is left child of its parent
		else if (insertedNode->getParent()->getLeft() == insertedNode) {
			Node<Key, Value>* y = insertedNode->getParent();
			Node<Key, Value>* z = y->getParent();

			// If y is also a left child of its parent
			if (y->getParent()->getLeft() == y) {
				// Perform LL Rotation
				SplayLLRotation(insertedNode, y, z);
			}

			// If y is a right child of its parent
			else {
				// RL Rotation
				RLRotation(insertedNode, y, z);
			}
		}

		// If inserted node is right child of its parent
		else {
			Node<Key, Value>* y = insertedNode->getParent();
			Node<Key, Value>* z = y->getParent();

			// If y is a left child of its parent
			if (z->getLeft() == y) {
				// LR Rotation
				LRRotation(insertedNode, y, z);
			}

			// If y is the right child of its parent
			else {
				// Perform RR Rotation
				SplayRRRotation(insertedNode, y, z);
			}
		}
	}
}


/**
* This function will keep track of the number of bad inserts and
* will return the last accessed leaf node if node does not exist
*/
template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::findHelper(const Key& key) {
	// If BST is empty
	if (BinarySearchTree<Key, Value>::mRoot == NULL) {
		return NULL;
	}

	Node<Key, Value>* currNode = BinarySearchTree<Key, Value>::mRoot;

	int level = 0;

	while (currNode != NULL) {
		if (key == currNode->getKey()) {
			// Update bad inserts
			if (level > (2 * log2(n))) {
				++badInserts;
			}

			return NULL;
		}

		// If key is less than currNode go to left
		// child
		else if (key < currNode->getKey()) {
			if (currNode->getLeft() == NULL) {
				return currNode;
			}
 
			currNode = currNode->getLeft();
		}

		// If key is greater than currNode go to right
		// child
		else {
			if (currNode->getRight() == NULL) {
				return currNode;
			}
			currNode = currNode->getRight();
		}

		// Increment level of insert
		++level;
	}

	return NULL;
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then splays the parent
* of the deleted node to the top.
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* nodeToDelete = BinarySearchTree<Key, Value>::internalFind(key);

	Node<Key, Value>* nodeToSplay;

	// If nodeToDelete is not in tree splay last accessed leaf node
	if (nodeToDelete == NULL) {
		nodeToSplay = findHelper(key);
		// If tree is empty do nothing
		if (nodeToSplay == NULL) {
			return;
		}
	}

	// If nodeToDelete is a leaf node
	else if (nodeToDelete->getLeft() == NULL && nodeToDelete->getRight() == NULL) {
		nodeToSplay = nodeToDelete->getParent();

		// If tree is empty do nothing
		if (nodeToSplay == NULL) {
			BinarySearchTree<Key, Value>::remove(key);
			return;
		}
	}

	// If nodeToDelete has two children
	else if (nodeToDelete->getLeft() != NULL && nodeToDelete->getRight() != NULL) {
		// To store successor of nodeToDelete
		Node<Key, Value>* successor = nodeToDelete->getRight();

		// Find successor
		while (successor->getLeft() != NULL) {
			successor = successor->getLeft();
		}

		if (successor == nodeToDelete->getRight()) {
			nodeToSplay = successor;
		}

		else {
			nodeToSplay = successor->getParent();
		}

		// If tree is empty do nothing
		if (nodeToSplay == NULL) {
			BinarySearchTree<Key, Value>::remove(key);
			return;
		}
	}

	// If nodeToDelete has only one child
	else {
		nodeToSplay = nodeToDelete->getParent();

		// If tree is empty do nothing
		if (nodeToSplay == NULL) {
			BinarySearchTree<Key, Value>::remove(key);
			return;
		}
	}

	// Call BST Remove
	BinarySearchTree<Key, Value>::remove(key);

	// Splay node to splay to the root
	while (nodeToSplay != BinarySearchTree<Key, Value>::mRoot) {
		// If inserted node is a child of the root
		if (nodeToSplay->getParent() == BinarySearchTree<Key, Value>::mRoot) {
			Node<Key, Value>* y = nodeToSplay->getParent();

			// If inserted node is the left child
			if (nodeToSplay->getParent()->getLeft() == nodeToSplay) {
				// Perform right single rotation
				rightSingleRotation(nodeToSplay, y, NULL);
			}

			// If inserted node is right child
			else {
				// Perform left single rotation
				leftSingleRotation(nodeToSplay, y, NULL);
			}
		}

		// If inserted node is left child of its parent
		else if (nodeToSplay->getParent()->getLeft() == nodeToSplay) {
			Node<Key, Value>* y = nodeToSplay->getParent();
			Node<Key, Value>* z = y->getParent();

			// If y is also a left child of its parent
			if (y->getParent()->getLeft() == y) {
				// Perform LL Rotation
				SplayLLRotation(nodeToSplay, y, z);
			}

			// If y is a right child of its parent
			else {
				// RL Rotation
				RLRotation(nodeToSplay, y, z);
			}
		}

		// If inserted node is right child of its parent
		else {
			Node<Key, Value>* y = nodeToSplay->getParent();
			Node<Key, Value>* z = y->getParent();

			// If y is a left child of its parent
			if (z->getLeft() == y) {
				// LR Rotation
				LRRotation(nodeToSplay, y, z);
			}

			// If y is the right child of its parent
			else {
				// Perform RR Rotation
				SplayRRRotation(nodeToSplay, y, z);
			}
		}
	}
}

/*
------------------------------------------
End implementations for the SplayTree class.
------------------------------------------
*/

#endif
