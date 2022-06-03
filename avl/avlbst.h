#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "../bst/bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
	AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
	virtual ~AVLNode();

	// Getter/setter for the node's height.
	int getHeight() const;
	void setHeight(int height);

	// Getters for parent, left, and right. These need to be redefined since they 
	// return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
	// for more information.
	virtual AVLNode<Key, Value>* getParent() const override;
	virtual AVLNode<Key, Value>* getLeft() const override;
	virtual AVLNode<Key, Value>* getRight() const override;

protected:
	int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
	: Node<Key, Value>(key, value, parent)
	, mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
	mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
	void LLRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z);
	void RRRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z);
	void LRRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z);
	void RLRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z);

	// This function will insert an AVLNode into the tree
	void insertHelper(const std::pair<Key, Value>& keyValuePair);

	// This function will remove an AVLNode and update all heights
	AVLNode<Key, Value>* removeHelper(const Key& key);
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* This helper function performs left zig-zig rotation on nodes x,y and z
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::LLRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z) {
	// if Z has no parent
	if (z->getParent() == NULL) {
		// Save y's right child if it exists 
		if (y->getRight() != NULL) {
			AVLNode<Key, Value>* yRight = y->getRight();
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

		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}

		y->setHeight(std::max(x->getHeight(), z->getHeight()) + 1);
	}

	// If Z has a parent
	else {
		// Save y's right child if it exists 
		if (y->getRight() != NULL) {
			AVLNode<Key, Value>* yRight = y->getRight();
			// Set z's left child to y's right child
			z->setLeft(yRight);

			// Set yRight's parent to z
			yRight->setParent(z);
		}

		else {
			// Set z's left to null if y's right child does not exist
			z->setLeft(NULL);
		}

		AVLNode<Key, Value>* zParent = z->getParent();

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

		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}

		y->setHeight(std::max(x->getHeight(), z->getHeight()) + 1);

		// If y is the left child of it's parent and the right child exists
		if (y->getParent()->getLeft() == y && y->getParent()->getRight() != NULL) {
			y->getParent()->setHeight(std::max(y->getHeight(), y->getParent()->getRight()->getHeight()) + 1);
		}

		// If y is the right child of it's parent and the left child exists
		else if (y->getParent()->getRight() == y && y->getParent()->getLeft() != NULL) {
			y->getParent()->setHeight(std::max(y->getHeight(), y->getParent()->getLeft()->getHeight()) + 1);
		}

		// If y is the only child 
		else {
			// Update y's parent's height
			y->getParent()->setHeight(y->getHeight() + 1);
		}
	}
}

/**
* This helper function performs right zig-zig rotation on nodes x,y and z
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::RRRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z) {
	// if Z has no parent
	if (z->getParent() == NULL) {
		// Save y's left child if it exists 
		if (y->getLeft() != NULL) {
			AVLNode<Key, Value>* yLeft = y->getLeft();
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

		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}

		y->setHeight(std::max(x->getHeight(), z->getHeight()) + 1);
	}

	// If Z has a parent
	else {
		// Save y's left child if it exists 
		if (y->getLeft() != NULL) {
			AVLNode<Key, Value>* yLeft = y->getLeft();
			// Set z's right child to y's left child
			z->setRight(yLeft);

			// Set yLeft's parent to z
			yLeft->setParent(z);
		}

		else {
			z->setRight(NULL);
		}

		AVLNode<Key, Value>* zParent = z->getParent();

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

		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}

		y->setHeight(std::max(x->getHeight(), z->getHeight()) + 1);

		// If y is the left child of it's parent and the right child exists
		if (y->getParent()->getLeft() == y && y->getParent()->getRight() != NULL) {
			y->getParent()->setHeight(std::max(y->getHeight(), y->getParent()->getRight()->getHeight()) + 1);
		}

		// If y is the right child of it's parent and the left child exists
		else if (y->getParent()->getRight() == y && y->getParent()->getLeft() != NULL) {
			y->getParent()->setHeight(std::max(y->getHeight(), y->getParent()->getLeft()->getHeight()) + 1);
		}

		// If y is the only child 
		else {
			// Update y's parent's height
			y->getParent()->setHeight(y->getHeight() + 1);
		}
	}
}

/**
* This helper function performs left zig-zag rotation on nodes x,y and z
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::LRRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z) {
	// If z does not have a parent
	if (z->getParent() == NULL) {
		if (x->getLeft() != NULL) {
			// Save x's left child
			AVLNode<Key, Value>* xLeft = x->getLeft();

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
			AVLNode<Key, Value>* xRight = x->getRight();

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

		// If y has two children
		if (y->getLeft() != NULL && y->getRight() != NULL) {
			// Update heights of nodes
			y->setHeight(std::max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);
		}

		// If y has only left child
		else if (y->getLeft() != NULL && y->getRight() == NULL) {
			y->setHeight(y->getLeft()->getHeight() + 1);
		}

		// If y has only right child
		else if (y->getRight() != NULL && y->getLeft() == NULL) {
			y->setHeight(y->getRight()->getHeight() + 1);
		}

		// If y is a leaf node
		else {
			y->setHeight(1);
		}


		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}


		x->setHeight(std::max(y->getHeight(), z->getHeight()) + 1);
	}

	// If z does have a parent
	else {
		if (x->getLeft() != NULL) {
			// Save x's left child
			AVLNode<Key, Value>* xLeft = x->getLeft();

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
			AVLNode<Key, Value>* xRight = x->getRight();

			// Set z's left to x's right
			z->setLeft(xRight);

			// Set x's right's parent to z 
			xRight->setParent(z);
		}

		else {
			z->setLeft(NULL);
		}

		// Save z's parent
		AVLNode<Key, Value>* zParent = z->getParent();

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

		// If y has two children
		if (y->getLeft() != NULL && y->getRight() != NULL) {
			// Update heights of nodes
			y->setHeight(std::max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);
		}

		// If y has only left child
		else if (y->getLeft() != NULL && y->getRight() == NULL) {
			y->setHeight(y->getLeft()->getHeight() + 1);
		}

		// If y has only right child
		else if (y->getRight() != NULL && y->getLeft() == NULL) {
			y->setHeight(y->getRight()->getHeight() + 1);
		}

		// If y is a leaf node
		else {
			y->setHeight(1);
		}


		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}


		x->setHeight(std::max(y->getHeight(), z->getHeight()) + 1);

		// If x is the left child of it's parent and the right child exists
		if (x->getParent()->getLeft() == x && x->getParent()->getRight() != NULL) {
			x->getParent()->setHeight(std::max(x->getHeight(), x->getParent()->getRight()->getHeight()) + 1);
		}

		// If x is the right child of it's parent and the left child exists
		else if (x->getParent()->getRight() == x && x->getParent()->getLeft() != NULL) {
			x->getParent()->setHeight(std::max(x->getHeight(), x->getParent()->getLeft()->getHeight()) + 1);
		}

		// If x is the only child 
		else {
			// Update x's parent's height
			x->getParent()->setHeight(x->getHeight() + 1);
		}
	}
}

/**
* This helper function performs right zig-zag rotation on nodes x,y and z
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::RLRotation(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z) {
	// If z does not have a parent
	if (z->getParent() == NULL) {
		if (x->getLeft() != NULL) {
			// Save x's left child
			AVLNode<Key, Value>* xLeft = x->getLeft();

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
			AVLNode<Key, Value>* xRight = x->getRight();

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

		// If y has two children
		if (y->getLeft() != NULL && y->getRight() != NULL) {
			// Update heights of nodes
			y->setHeight(std::max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);
		}

		// If y has only left child
		else if (y->getLeft() != NULL && y->getRight() == NULL) {
			y->setHeight(y->getLeft()->getHeight() + 1);
		}

		// If y has only right child
		else if (y->getRight() != NULL && y->getLeft() == NULL) {
			y->setHeight(y->getRight()->getHeight() + 1);
		}

		// If y is a leaf node
		else {
			y->setHeight(1);
		}


		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}


		x->setHeight(std::max(y->getHeight(), z->getHeight()) + 1);
	}

	// If z does have a parent
	else {
		if (x->getLeft() != NULL) {
			// Save x's left child
			AVLNode<Key, Value>* xLeft = x->getLeft();

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
			AVLNode<Key, Value>* xRight = x->getRight();

			// Set y's left to x's right
			y->setLeft(xRight);

			// Set x's right's parent to y 
			xRight->setParent(y);
		}

		else {
			y->setLeft(NULL);
		}

		// Save z's parent
		AVLNode<Key, Value>* zParent = z->getParent();

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

		// If y has two children
		if (y->getLeft() != NULL && y->getRight() != NULL) {
			// Update heights of nodes
			y->setHeight(std::max(y->getLeft()->getHeight(), y->getRight()->getHeight()) + 1);
		}

		// If y has only left child
		else if (y->getLeft() != NULL && y->getRight() == NULL) {
			y->setHeight(y->getLeft()->getHeight() + 1);
		}

		// If y has only right child
		else if (y->getRight() != NULL && y->getLeft() == NULL) {
			y->setHeight(y->getRight()->getHeight() + 1);
		}

		// If y is a leaf node
		else {
			y->setHeight(1);
		}


		// If z has two children
		if (z->getLeft() != NULL && z->getRight() != NULL) {
			// Update heights of nodes
			z->setHeight(std::max(z->getLeft()->getHeight(), z->getRight()->getHeight()) + 1);
		}

		// If z has only left child
		else if (z->getLeft() != NULL && z->getRight() == NULL) {
			z->setHeight(z->getLeft()->getHeight() + 1);
		}

		// If z has only right child
		else if (z->getRight() != NULL && z->getLeft() == NULL) {
			z->setHeight(z->getRight()->getHeight() + 1);
		}

		// If z is a leaf node
		else {
			z->setHeight(1);
		}

		
		x->setHeight(std::max(y->getHeight(), z->getHeight()) + 1);

		// If x is the left child of it's parent and the right child exists
		if (x->getParent()->getLeft() == x && x->getParent()->getRight() != NULL) {
			x->getParent()->setHeight(std::max(x->getHeight(), x->getParent()->getRight()->getHeight()) + 1);
		}

		// If x is the right child of it's parent and the left child exists
		else if (x->getParent()->getRight() == x && x->getParent()->getLeft() != NULL) {
			x->getParent()->setHeight(std::max(x->getHeight(), x->getParent()->getLeft()->getHeight()) + 1);
		}

		// If x is the only child 
		else {
			// Update x's parent's height
			x->getParent()->setHeight(x->getHeight() + 1);
		}
	}
}

/**
* This function will do BST insert on an AVLNode rather than Node
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insertHelper(const std::pair<Key, Value>& keyValuePair) {
	// If the BST is empty
	if (BinarySearchTree<Key, Value>::mRoot == NULL) {
		// Create new node with keyValuePair's information
		// Set Parent to null
		Node<Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);

		// Add new node to BST as root
		BinarySearchTree<Key, Value>::mRoot = newNode;
	}

	// If BST has only one node
	else if (BinarySearchTree<Key, Value>::mRoot->getLeft() == NULL && BinarySearchTree<Key, Value>::mRoot->getRight() == NULL) {
		// If there is a duplicate entry
		if (keyValuePair.first == BinarySearchTree<Key, Value>::mRoot->getKey()) {
			// Update root's value
			BinarySearchTree<Key, Value>::mRoot->setValue(keyValuePair.second);
			return;
		}

		// Create new node with keyValuePair's information
		// Set Parent to mRoot
		Node<Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair.first, 
			keyValuePair.second, static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot));

		// If new node is lesser than root
		if (newNode->getKey() < BinarySearchTree<Key, Value>::mRoot->getKey()) {
			// Set newNode as root's left child
			BinarySearchTree<Key, Value>::mRoot->setLeft(newNode);
		}

		// If new node is greater than root
		else {
			// Set newNode as root's right child
			BinarySearchTree<Key, Value>::mRoot->setRight(newNode);
		}
	}

	// If BST has more than one node
	else {
		Node<Key, Value>* currNode = BinarySearchTree<Key, Value>::mRoot;
		// Will store the parent of the new node that
		// needs to be added after the while loop finishes
		Node<Key, Value>* prevNode;

		// Create new node with keyValuePair's information
		// Set Parent to NULL (Will be changed after we find where to
		// insert in tree)
		Node<Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);

		// While loop to find location to add newNode
		while(currNode != NULL) {
			// If there is a duplicate entry
			if (newNode->getKey() == currNode->getKey()) {
				// Update currNode's value
				currNode->setValue(newNode->getValue());
				return;
			}

			// If new node is lesser than currNode
			else if (newNode->getKey() < currNode->getKey()) {
				// Save currNode
				prevNode = currNode;
				// Move to left child of currNode
				currNode = currNode->getLeft();
			}

			// If new node is greater than currNode
			else {
				// Save currNode
				prevNode = currNode;
				// Move to right child of currNode
				currNode = currNode->getRight();
			}
		}

		// Set parent of newNode
		newNode->setParent(prevNode);

		// If newNode needs to be added as left child of prevNode
		if (newNode->getKey() < prevNode->getKey()) {
			prevNode->setLeft(newNode);
		}

		// If newNode needs to be added as right child of prevNode
		else {
			prevNode->setRight(newNode);
		}
	}
}

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	// Insert using BST insert
	insertHelper(keyValuePair);

	// If BST only has one node
	if (BinarySearchTree<Key, Value>::mRoot->getLeft() == NULL && BinarySearchTree<Key, Value>::mRoot->getRight() == NULL) {
		// Set height of root
		static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::mRoot)->setHeight(1);

		return;
	}

	// Find the newly inserted node
	Node<Key, Value>* insertedNode = BinarySearchTree<Key, Value>::internalFind(keyValuePair.first);

	// Cast it to AVL node type
	AVLNode<Key, Value>* insertedAVLNode = static_cast<AVLNode<Key, Value>*>(insertedNode);

	// Set height of new AVLNode
	insertedAVLNode->setHeight(1);

	AVLNode<Key, Value>* currNode = insertedAVLNode;
	AVLNode<Key, Value>* parentNode;

	while (currNode != NULL) {
		parentNode = currNode->getParent();

		// Terminate when there is no parent
		if (parentNode == NULL) {
			break;
		}

		int parentHeight = parentNode->getHeight();

		// If currNode is the only child of the parent
		// and parent was a leaf node before insertion
		if (currNode->getHeight() == parentHeight && parentHeight == 1) {
			// Update parent node's height
			parentNode->setHeight(parentHeight + 1);

			currNode = parentNode;
		}

		// If currNode is left child of parent and right child exists
		else if (parentNode->getLeft() == currNode && parentNode->getRight() != NULL) {
			int rightHeight = parentNode->getRight()->getHeight();

			// Calculate balance factor to check if parent is unbalanced
			int balanceFactor = abs(currNode->getHeight() - rightHeight);

			// If parent is balanced
			if (balanceFactor <= 1) {
				parentNode->setHeight(std::max(rightHeight, currNode->getHeight()) + 1);

				currNode = parentNode;
			}

			// If parent is unbalanced
			else {
				// Will store greatest height child of parentNode
				AVLNode<Key, Value>* y;

				// If right child is greatest height child
				if (currNode->getHeight() < rightHeight) {
					y = parentNode->getRight();
				}

				// If currNode is greatest height child
				else {
					y = currNode;
				}

				AVLNode<Key, Value>* x;

				// If y has two children
				if (y->getLeft() != NULL && y->getRight() != NULL) {
					// If y has equal height children
					if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
						// Choose x so that the easiest rotation is done
						if (y == currNode)  {
							// LL Rotation
							x = y->getLeft();

							LLRotation(x, y, parentNode);

							break;
						}

						else {
							// RR Rotation
							x = y->getRight();

							RRRotation(x, y, parentNode);

							break;
						}
					}

					// If y's left child has greatest height
					else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
						// Set x as y's left child
						x = y->getLeft();

						// Check which kind of rotation to perform
						if (y == currNode) {
							// LL Rotation

							LLRotation(x, y, parentNode);

							break;
						}

						else {
							// RL Rotation

							RLRotation(x, y, parentNode);

							break;
						}
					}

					// If y's right child has greatest height
					else {
						// Set x as y's right child
						x = y->getRight();

						if (y == currNode) {
							// LR Rotation

							LRRotation(x, y, parentNode);

							break;
						}

						else {
							// RR Rotation

							RRRotation(x, y, parentNode);

							break;
						}
					}
				}

				// If y has only left child
				else if (y->getLeft() != NULL) {
					// Set x
					x = y->getLeft();

					if (y == currNode) {
						// LL Rotation

						LLRotation(x, y, parentNode);

						break;
					}

					else {
						// RL Rotation

						RLRotation(x, y, parentNode);

						break;
					}
				}

				// If y has only right child
				else {
					// Set x
					x = y->getRight();

					if (y == currNode) {
						// LR Rotation

						LRRotation(x, y, parentNode);

						break;
					}

					else {
						// RR Rotation

						RRRotation(x, y, parentNode);

						break;
					}
				}
			}
		}

		// If currNode is right child  of parent and left child exists
		else if (parentNode->getRight() == currNode && parentNode->getLeft() != NULL) {
			int leftHeight = parentNode->getLeft()->getHeight();

			// Calculate balance factor to check if parent is unbalanced
			int balanceFactor = abs(currNode->getHeight() - leftHeight);

			// If parent is balanced
			if (balanceFactor <= 1) {
				// Update height of parent
				parentNode->setHeight(std::max(leftHeight, currNode->getHeight()) + 1);

				currNode = parentNode;
			}

			// If parent is unbalanced
			else {
				// Will store greatest height child of parentNode
				AVLNode<Key, Value>* y;

				// If left child is greatest height child
				if (currNode->getHeight() < leftHeight) {
					y = parentNode->getLeft();
				}

				// If currNode is greatest height child
				else {
					y = currNode;
				}

				AVLNode<Key, Value>* x;

				// If y has two children
				if (y->getLeft() != NULL && y->getRight() != NULL) {
					// If y has equal height children
					if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
						// Choose x so that the easiest rotation is done
						if (y == currNode)  {
							// RR Rotation
							x = y->getRight();

							RRRotation(x, y, parentNode);

							break;
						}

						else {
							// LL Rotation
							x = y->getLeft();

							LLRotation(x, y, parentNode);

							break;
						}
					}

					// If y's left child has greatest height
					else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
						// Set x as y's left child
						x = y->getLeft();

						// Check which kind of rotation to perform
						if (y == currNode) {
							// RL Rotation

							RLRotation(x, y, parentNode);

							break;
						}

						else {
							// LL Rotation

							LLRotation(x, y, parentNode);

							break;
						}
					}

					// If y's right child has greatest height
					else {
						// Set x as y's right child
						x = y->getRight();

						if (y == currNode) {
							// RR Rotation

							RRRotation(x, y, parentNode);

							break;
						}

						else {
							// LR Rotation

							LRRotation(x, y, parentNode);

							break;
						}
					}
				}

				// If y has only left child
				else if (y->getLeft() != NULL) {
					// Set x
					x = y->getLeft();

					if (y == currNode) {
						// RL Rotation

						RLRotation(x, y, parentNode);

						break;
					}

					else {
						// LL Rotation

						LLRotation(x, y, parentNode);

						break;
					}
				}

				// If y has only right child
				else {
					// Set x
					x = y->getRight();

					if (y == currNode) {
						// RR Rotation

						RRRotation(x, y, parentNode);

						break;
					}

					else {
						// LR Rotation

						LRRotation(x, y, parentNode);

						break;
					}
				}
			}
		}

		// currNode is the only child of it's parent
		else {
			// Calculate balance factor of parent
			int balanceFactor = currNode->getHeight() - 0;

			// If parent is balanced
			if (balanceFactor <= 1) {
				// Update height of parent
				parentNode->setHeight(currNode->getHeight() + 1);

				currNode = parentNode;
			}

			// If parent is unbalanced
			else {
				// Parent is z, and currNode is y as it is
				// the only child
				AVLNode<Key, Value>* y = currNode;
				AVLNode<Key, Value>* x;

				// If y has two children
				if (y->getLeft() != NULL && y->getRight() != NULL) {
					// If y has equal height children
					if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
						// Choose x to perform easiest rotationnnnn
						if (y == parentNode->getLeft()) {
							x = y->getLeft();

							// LL Rotation
							LLRotation(x, y, parentNode);

							break;
						}

						// If y is the right child of z
						else {
							x = y->getRight();

							// RR Rotation
							RRRotation(x, y, parentNode);

							break;
						}
					}

					// If y has greatest height left child
					else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
						// Set x to y's left child
						x = y->getLeft();

						// If y is z's left child
						if (y == parentNode->getLeft()) {
							// LL Rotation
							LLRotation(x, y, parentNode);

							break;
						}

						// If y is z's right child
						else {
							// RL Rotation
							RLRotation(x, y, parentNode);

							break;
						}
					}

					// If y had greatest height right child
					else {
						// Set x to y's right child
						x = y->getRight();

						// If y is z's left child
						if (y == parentNode->getLeft()) {
							// LR Rotation
							LRRotation(x, y, parentNode);

							break;
						}

						// If y is z's right child
						else {
							// RR Rotation
							RRRotation(x, y, parentNode);

							break;
						}
					}
				}

				// If y only has a left child
				else if (y->getLeft() != NULL) {
					// Set x
					x = y->getLeft();

					// If y is z's left child
					if (y == parentNode->getLeft()) {
						// LL Rotation
						LLRotation(x, y, parentNode);

						break;
					}

					// If y is z's right child
					else {
						// RL Rotation
						RLRotation(x, y, parentNode);

						break;
					}
				}

				// If y only has a right child
				else {
					// Set x
					x = y->getRight();

					// If y is z's left child
					if (y == parentNode->getLeft()) {
						// LR Rotation
						LRRotation(x, y, parentNode);

						break;
					}

					// If y is z's right child
					else {
						// RR Rotation
						RRRotation(x, y, parentNode);

						break;
					}
				}
			}
		}
	}
}

/**
* This function will return a pointer to the node that we need to start checking
* at for balancing
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeHelper(const Key& key) {
	// Do nothing if tree is empty
	if (BinarySearchTree<Key, Value>::mRoot == NULL) {
		return NULL;
	}

	// Find node to delete
	AVLNode<Key, Value>* nodeToDelete = 
	static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));

	// If node to delete does not exist do nothing
	if (nodeToDelete == NULL) {
		return NULL;
	}

	// Check if node to delete is the root
	if (nodeToDelete == BinarySearchTree<Key, Value>::mRoot) {
		// If root has no children
		if (nodeToDelete->getLeft() == NULL && nodeToDelete->getRight() == NULL) {
			// Set mRoot to null
			BinarySearchTree<Key, Value>::mRoot = NULL;

			// Free allocated memory
			delete nodeToDelete;

			return NULL;
		}

		// If root has 2 children
		else if (nodeToDelete->getLeft() != NULL && nodeToDelete->getRight() != NULL) {
			// To store successor of nodeToDelete
			AVLNode<Key, Value>* successor = nodeToDelete->getRight();

			while (successor->getLeft() != NULL) {
				successor = successor->getLeft();
			}

			// If the right child has no left child
			if (successor == nodeToDelete->getRight()) {
				// Set successors left child to root's left child
				successor->setLeft(nodeToDelete->getLeft());
				// Set successor as nodeToDelete's left child's parent
				nodeToDelete->getLeft()->setParent(successor);

				// Set successor to NULL as it's the new root
				successor->setParent(NULL);

				// Set successor as the root
				BinarySearchTree<Key, Value>::mRoot = successor;

				// Set new height of successor
				if (successor->getRight() != NULL) {
					successor->setHeight(std::max(successor->getRight()->getHeight(), successor->getLeft()->getHeight()) + 1);
				}

				else {
					successor->setHeight(successor->getLeft()->getHeight() + 1);
				}

				// Free allocated memory
				delete nodeToDelete;

				return successor;
			}

			// If successor has right child 
			else if (successor->getRight() != NULL) {
				// Set successor's right as it's parent's left
				successor->getParent()->setLeft(successor->getRight());
				// Set successor's right's parent as it's parent
				successor->getRight()->setParent(successor->getParent());

				AVLNode<Key, Value>* nodeToCheck = successor->getParent();

				// Update heights
				// If successor's parent has a right child
				if (successor->getParent()->getRight() != NULL) {
					successor->getParent()->setHeight(std::max(successor->getRight()->getHeight(), 
						successor->getParent()->getRight()->getHeight()) + 1);
				}

				else {
					successor->getParent()->setHeight(successor->getRight()->getHeight() + 1);
				}

				// Set successor's left as root's left
				successor->setLeft(nodeToDelete->getLeft());
				// Set Successor as the parent of the left child of the root
				nodeToDelete->getLeft()->setParent(successor);

				// Set Successor's right to root's right
				successor->setRight(nodeToDelete->getRight());
				nodeToDelete->getRight()->setParent(successor);

				// Set successor's parent to NULL
				successor->setParent(NULL);

				// Make successor the root
				BinarySearchTree<Key, Value>::mRoot = successor;

				// Free allocated memory
				delete nodeToDelete;

				return nodeToCheck;
			}

			// If successor is a leaf node
			else {
				// Set successor's parent's left to null
				successor->getParent()->setLeft(NULL);

				AVLNode<Key, Value>* nodeToCheck = successor->getParent();

				// Update heights
				if (successor->getParent()->getRight() != NULL) {
					successor->getParent()->setHeight(successor->getParent()->getRight()->getHeight() + 1);
				}

				else {
					successor->getParent()->setHeight(1);
				}

				// Set successor's parent to null
				successor->setParent(NULL);

				// Update successor's left and right children
				successor->setLeft(nodeToDelete->getLeft());
				nodeToDelete->getLeft()->setParent(successor);

				successor->setRight(nodeToDelete->getRight());
				nodeToDelete->getRight()->setParent(successor);

				// Make successor the root
				BinarySearchTree<Key, Value>::mRoot = successor;

				// Free allocated memory
				delete nodeToDelete;

				return nodeToCheck;
			}
		}

		// If root has only one child
		else {
			// If root has only left child
			if (nodeToDelete->getLeft() != NULL) {
				// Set child of root's parent to null
				nodeToDelete->getLeft()->setParent(NULL);

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getLeft();

				// Promote child of root
				BinarySearchTree<Key, Value>::mRoot = nodeToDelete->getLeft();

				// Free allocated memory
				delete nodeToDelete;

				return nodeToCheck;
			}

			// If root has only right child
			else {
				// Set child of root's parent to null
				nodeToDelete->getRight()->setParent(NULL);

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getRight();

				// Promote child of root
				BinarySearchTree<Key, Value>::mRoot = nodeToDelete->getRight();

				// Free allocated memory
				delete nodeToDelete;

				return nodeToCheck;
			}
		}
	}

	// If node to delete is not the root
	else {
		// If node to delete is a leaf node
		if (nodeToDelete->getLeft() == NULL && nodeToDelete->getRight() == NULL) {
			// If node to delete is left child
			if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
				// Remove node to delete from tree
				nodeToDelete->getParent()->setLeft(NULL);

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getParent();

				if (nodeToCheck->getRight() != NULL) {
					nodeToCheck->setHeight(nodeToCheck->getRight()->getHeight() + 1);
				}

				else {
					nodeToCheck->setHeight(1);
				}

				delete nodeToDelete;

				return nodeToCheck;
			}

			// If node to delete is right child
			else {
				nodeToDelete->getParent()->setRight(NULL);

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getParent();

				if (nodeToCheck->getLeft() != NULL) {
					nodeToCheck->setHeight(nodeToCheck->getLeft()->getHeight() + 1);
				}

				else {
					nodeToCheck->setHeight(1);
				}

				delete nodeToDelete;

				return nodeToCheck;
			}
		}

		// If node to delete has 2 children
		else if (nodeToDelete->getLeft() != NULL && nodeToDelete->getRight() != NULL) {
			// To store successor of nodeToDelete
			AVLNode<Key, Value>* successor = nodeToDelete->getRight();

			while (successor->getLeft() != NULL) {
				successor = successor->getLeft();
			}

			// If the right child of node to delete has no left child
			if (successor == nodeToDelete->getRight()) {
				// Set successors left child to root's left child
				successor->setLeft(nodeToDelete->getLeft());
				// Set successor as nodeToDelete's left child's parent
				nodeToDelete->getLeft()->setParent(successor);

				successor->setParent(nodeToDelete->getParent());

				AVLNode<Key, Value>* nodeToCheck = successor;

				// If nodeToDelete is the left child 
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(successor);
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(successor);
				}

				// Set new height of successor
				if (successor->getRight() != NULL) {
					successor->setHeight(std::max(successor->getRight()->getHeight(), successor->getLeft()->getHeight()) + 1);
				}

				else {
					successor->setHeight(successor->getLeft()->getHeight() + 1);
				}

				delete nodeToDelete;

				return nodeToCheck;
			}

			// If successor has right child 
			else if (successor->getRight() != NULL) {
				// Set successor's right as it's parent's left
				successor->getParent()->setLeft(successor->getRight());
				// Set successor's right's parent as it's parent
				successor->getRight()->setParent(successor->getParent());

				AVLNode<Key, Value>* nodeToCheck = successor->getParent();

				// Update heights
				// If successor's parent has a right child
				if (successor->getParent()->getRight() != NULL) {
					successor->getParent()->setHeight(std::max(successor->getRight()->getHeight(), 
						successor->getParent()->getRight()->getHeight()) + 1);
				}

				else {
					successor->getParent()->setHeight(successor->getRight()->getHeight() + 1);
				}

				// Set successor's left as node to delete's left
				successor->setLeft(nodeToDelete->getLeft());
				// Set Successor as the parent of the left child of the NTD
				nodeToDelete->getLeft()->setParent(successor);

				// Set Successor's right to root's right
				successor->setRight(nodeToDelete->getRight());
				nodeToDelete->getRight()->setParent(successor);

				// Set successor's parent to node to delete's parent
				successor->setParent(nodeToDelete->getParent());

				// If nodeToDelete is the left child 
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(successor);
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(successor);
				}

				delete nodeToDelete;

				return nodeToCheck;
			}

			// If successor is a leaf node
			else {
				// Set successor's parent's left to null
				successor->getParent()->setLeft(NULL);

				AVLNode<Key, Value>* nodeToCheck = successor->getParent();

				// Update heights
				if (successor->getParent()->getRight() != NULL) {
					successor->getParent()->setHeight(successor->getParent()->getRight()->getHeight() + 1);
				}

				else {
					successor->getParent()->setHeight(1);
				}

				// Update successor's left and right children
				successor->setLeft(nodeToDelete->getLeft());
				nodeToDelete->getLeft()->setParent(successor);

				successor->setRight(nodeToDelete->getRight());
				nodeToDelete->getRight()->setParent(successor);

				// Set successor's parent to NTD's parent
				successor->setParent(nodeToDelete->getParent());

				// If nodeToDelete is the left child 
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(successor);
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(successor);
				}

				delete nodeToDelete;

				return nodeToCheck;
			}
		}

		// If node to delete has 1 child
		else {
			// If NTD has only left child
			if (nodeToDelete->getLeft() != NULL) {
				nodeToDelete->getLeft()->setParent(nodeToDelete->getParent());

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getParent();

				// If nodeToDelete is the left child
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(nodeToDelete->getLeft());

					if (nodeToCheck->getRight() != NULL) {
						nodeToCheck->setHeight(std::max(nodeToDelete->getLeft()->getHeight(), nodeToCheck->getRight()->getHeight()) + 1);
					}

					else {
						nodeToCheck->setHeight(nodeToDelete->getLeft()->getHeight() + 1);
					}
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(nodeToDelete->getLeft());

					if (nodeToCheck->getLeft() != NULL) {
						nodeToCheck->setHeight(std::max(nodeToDelete->getLeft()->getHeight(), nodeToCheck->getLeft()->getHeight()) + 1);
					}

					else {
						nodeToCheck->setHeight(nodeToDelete->getLeft()->getHeight() + 1);
					}
				}

				delete nodeToDelete;

				return nodeToCheck;
			}

			// If NTD has only right child
			else {
				nodeToDelete->getRight()->setParent(nodeToDelete->getParent());

				AVLNode<Key, Value>* nodeToCheck = nodeToDelete->getParent();

				// If nodeToDelete is the left child
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(nodeToDelete->getRight());

					if (nodeToCheck->getRight() != NULL) {
						nodeToCheck->setHeight(std::max(nodeToDelete->getRight()->getHeight(), nodeToCheck->getRight()->getHeight()) + 1);
					}

					else {
						nodeToCheck->setHeight(nodeToDelete->getRight()->getHeight() + 1);
					}
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(nodeToDelete->getRight());

					if (nodeToCheck->getLeft() != NULL) {
						nodeToCheck->setHeight(std::max(nodeToDelete->getRight()->getHeight(), nodeToCheck->getLeft()->getHeight()) + 1);
					}

					else {
						nodeToCheck->setHeight(nodeToDelete->getRight()->getHeight() + 1);
					}
				}

				delete nodeToDelete;

				return nodeToCheck;
			}
		}
	}
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	// Find the node to delete
	AVLNode<Key, Value>* nodeToDelete = 
	static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));

	// Do nothing if nodeToDelete is null
	if (nodeToDelete == NULL) {
		return;
	}

	// Call BST Remove
	// To store the node where our function needs to start checking from
	AVLNode<Key, Value>* nodeToCheck = removeHelper(key);

	// Do nothing if tree is already balanced
	if (nodeToCheck == NULL) {
		return;
	}

	// If tree is now empty, do nothing
	if (BinarySearchTree<Key, Value>::mRoot == NULL) {
		return;
	}

	// Start checking if the tree is unbalanced or not
	while (nodeToCheck != NULL) {
		// If nodeToCheck is a leaf node
		if (nodeToCheck->getLeft() == NULL && nodeToCheck->getRight() == NULL) {
			// Node to check is already balanced, check the parent

			// Update Parent's height first
			if (nodeToCheck->getParent() != NULL) {
				// If node to check is the left child
				if (nodeToCheck->getParent()->getLeft() == nodeToCheck) {
					// If node to check's parent has a right child
					if (nodeToCheck->getParent()->getRight() != NULL) {
						nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
							nodeToCheck->getParent()->getRight()->getHeight()) + 1);
					}

					// If it does not
					else {
						nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
					}
				}

				// If node to check is the right child
				else {
					// If node to check's parent has a left child
					if (nodeToCheck->getParent()->getLeft() != NULL) {
						nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
							nodeToCheck->getParent()->getLeft()->getHeight()) + 1);
					}

					// If it does not
					else {
						nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
					}
				}
			}

			else {
				break;
			}

			// Check parent
			nodeToCheck = nodeToCheck->getParent();
		}

		// If nodeToCheck has two children
		else if (nodeToCheck->getLeft() != NULL && nodeToCheck->getRight() != NULL) {
			// Calculate balance factor to check if nodeToCheck is unbalanced
			int balanceFactor = abs(nodeToCheck->getLeft()->getHeight() - nodeToCheck->getRight()->getHeight());

			// If node to check is balanced
			if (balanceFactor <= 1) {
				// Update Parent's height first
				if (nodeToCheck->getParent() != NULL) {
					// If node to check is the left child
					if (nodeToCheck->getParent()->getLeft() == nodeToCheck) {
						// If node to check's parent has a right child
						if (nodeToCheck->getParent()->getRight() != NULL) {
							nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
								nodeToCheck->getParent()->getRight()->getHeight()) + 1);
						}

						// If it does not
						else {
							nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
						}
					}

					// If node to check is the right child
					else {
						// If node to check's parent has a left child
						if (nodeToCheck->getParent()->getLeft() != NULL) {
							nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
								nodeToCheck->getParent()->getLeft()->getHeight()) + 1);
						}

						// If it does not
						else {
							nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
						}
					}
				}

				else {
					break;
				}

				// Check parent
				nodeToCheck = nodeToCheck->getParent();
			}

			// If node to check is unbalanced
			else {
				AVLNode<Key, Value>* z = nodeToCheck;
				AVLNode<Key, Value>* y;

				// If node to check has greatest height left child
				if (nodeToCheck->getLeft()->getHeight() > nodeToCheck->getRight()->getHeight()) {
					y = nodeToCheck->getLeft();

					AVLNode<Key, Value>* x;

					// If y has 2 children
					if (y->getLeft() != NULL && y->getRight() != NULL) {
						// If y has equal height children
						if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
							// Choose x for easier rotation
							x = y->getLeft();

							// LL Rotation
							LLRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height left child
						else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
							x = y->getLeft();

							// LL Rotation
							LLRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height right child
						else {
							x = y->getRight();

							// LR Rotation
							LRRotation(x, y, z);

							nodeToCheck = x->getParent();
						}
					}

					// If y has only left child 
					else if (y->getLeft() != NULL && y->getRight() == NULL) {
						x = y->getLeft();

						// LL Rotation
						LLRotation(x, y, z);

						nodeToCheck = y->getParent();
					}

					// If y only has right child (y cannot have no children)
					else {
						x = y->getRight();

						// LR Rotation
						LRRotation(x, y, z);

						nodeToCheck = x->getParent();
					}
				}

				// If node to check has greatest height right child
				else {
					y = nodeToCheck->getRight();

					AVLNode<Key, Value>* x;

					// If y has 2 children
					if (y->getLeft() != NULL && y->getRight() != NULL) {
						// If y has equal height children
						if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
							x = y->getRight();

							// RR Rotation
							RRRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height left child
						else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
							x = y->getLeft();

							// RL Rotation
							RLRotation(x, y, z);

							nodeToCheck = x->getParent();
						}

						// If y has greatest height right child
						else {
							x = y->getRight();

							// RR Rotation
							RRRotation(x, y, z);

							nodeToCheck = y->getParent();
						}
					}

					// If y has only left child 
					else if (y->getLeft() != NULL && y->getRight() == NULL) {
						x = y->getLeft();

						// RL Rotation
						RLRotation(x, y, z);

						nodeToCheck = x->getParent();
					}

					// If y only has right child (y cannot have no children)
					else {
						x = y->getRight();

						// RR Rotation
						RRRotation(x, y, z);

						nodeToCheck = y->getParent();
					}
				}
			}
		}

		// If nodeToCheck has one child
		else {
			// If nodeToCheck has only left child
			if (nodeToCheck->getLeft() != NULL) {
				// Calculate balance factor to check if nodeToCheck is unbalanced
				int balanceFactor = nodeToCheck->getLeft()->getHeight() - 0;

				// If node to check is balanced
				if (balanceFactor <= 1) {
					// Update Parent's height first
					if (nodeToCheck->getParent() != NULL) {
						// If node to check is the left child
						if (nodeToCheck->getParent()->getLeft() == nodeToCheck) {
							// If node to check's parent has a right child
							if (nodeToCheck->getParent()->getRight() != NULL) {
								nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
									nodeToCheck->getParent()->getRight()->getHeight()) + 1);
							}

							// If it does not
							else {
								nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
							}
						}

						// If node to check is the right child
						else {
							// If node to check's parent has a left child
							if (nodeToCheck->getParent()->getLeft() != NULL) {
								nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
									nodeToCheck->getParent()->getLeft()->getHeight()) + 1);
							}

							// If it does not
							else {
								nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
							}
						}
					}

					else {
						break;
					}

					// Check parent
					nodeToCheck = nodeToCheck->getParent();
				}

				// If node to check is unbalanced
				else {
					AVLNode<Key, Value>* z = nodeToCheck;
					AVLNode<Key, Value>* y = nodeToCheck->getLeft();

					AVLNode<Key, Value>* x;

					// If y has 2 children
					if (y->getLeft() != NULL && y->getRight() != NULL) {
						// If y has equal height children
						if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
							x = y->getLeft();

							// LL Rotation
							LLRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height left child
						else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
							x = y->getLeft();

							// LL Rotation
							LLRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height right child
						else {
							x = y->getRight();

							// LR Rotation
							LRRotation(x, y, z);

							nodeToCheck = x->getParent();
						}
					}

					// If y has only left child 
					else if (y->getLeft() != NULL && y->getRight() == NULL) {
						x = y->getLeft();

						// LL Rotation
						LLRotation(x, y, z);

						nodeToCheck = y->getParent();
					}

					// If y only has right child (y cannot have no children)
					else {
						x = y->getRight();

						// LR Rotation
						LRRotation(x, y, z);

						nodeToCheck = x->getParent();
					}
				}
			}

			// If nodeTocheck has only right child
			else {
				// Calculate balance factor to check if nodeToCheck is unbalanced
				int balanceFactor = abs(0 - nodeToCheck->getRight()->getHeight());

				// If node to check is balanced
				if (balanceFactor <= 1) {
					// Update Parent's height first
					if (nodeToCheck->getParent() != NULL) {
						// If node to check is the left child
						if (nodeToCheck->getParent()->getLeft() == nodeToCheck) {
							// If node to check's parent has a right child
							if (nodeToCheck->getParent()->getRight() != NULL) {
								nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
									nodeToCheck->getParent()->getRight()->getHeight()) + 1);
							}

							// If it does not
							else {
								nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
							}
						}

						// If node to check is the right child
						else {
							// If node to check's parent has a left child
							if (nodeToCheck->getParent()->getLeft() != NULL) {
								nodeToCheck->getParent()->setHeight(std::max(nodeToCheck->getHeight(), 
									nodeToCheck->getParent()->getLeft()->getHeight()) + 1);
							}

							// If it does not
							else {
								nodeToCheck->getParent()->setHeight(nodeToCheck->getHeight() + 1);
							}
						}
					}

					else {
						break;
					}

					// Check parent
					nodeToCheck = nodeToCheck->getParent();
				}

				// If node to check is unbalanced
				else {
					AVLNode<Key, Value>* z = nodeToCheck;
					AVLNode<Key, Value>* y = nodeToCheck->getRight();

					AVLNode<Key, Value>* x;

					// If y has 2 children
					if (y->getLeft() != NULL && y->getRight() != NULL) {
						// If y has equal height children
						if (y->getLeft()->getHeight() == y->getRight()->getHeight()) {
							x = y->getRight();

							// RR Rotation
							RRRotation(x, y, z);

							nodeToCheck = y->getParent();
						}

						// If y has greatest height left child
						else if (y->getLeft()->getHeight() > y->getRight()->getHeight()) {
							x = y->getLeft();

							// RL Rotation
							RLRotation(x, y, z);

							nodeToCheck = x->getParent();
						}

						// If y has greatest height right child
						else {
							x = y->getRight();

							// RR Rotation
							RRRotation(x, y, z);

							nodeToCheck = y->getParent();
						}
					}

					// If y has only left child 
					else if (y->getLeft() != NULL && y->getRight() == NULL) {
						x = y->getLeft();

						// RL Rotation
						RLRotation(x, y, z);

						nodeToCheck = x->getParent();
					}

					// If y only has right child (y cannot have no children)
					else {
						x = y->getRight();

						// RR Rotation
						RRRotation(x, y, z);

						nodeToCheck = y->getParent();
					}
				}
			}
		}
	}
}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
