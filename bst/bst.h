#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
* A templated class for a Node in a search tree. The getters for parent/left/right are virtual so that they
* can be overridden for future kinds of search trees, such as Red Black trees, Splay trees, and AVL trees.
*/
template <typename Key, typename Value>
class Node
{
public:
	Node(const Key& key, const Value& value, Node<Key, Value>* parent);
	virtual ~Node();

	const std::pair<Key, Value>& getItem() const;
	std::pair<Key, Value>& getItem();
	const Key& getKey() const;
	const Value& getValue() const;
	Key& getKey();
	Value& getValue();

	virtual Node<Key, Value>* getParent() const;
	virtual Node<Key, Value>* getLeft() const;
	virtual Node<Key, Value>* getRight() const;

	void setParent(Node<Key, Value>* parent);
	void setLeft(Node<Key, Value>* left);
	void setRight(Node<Key, Value>* right);
	void setValue(const Value &value);

protected:
	std::pair<Key, Value> mItem;
	Node<Key, Value>* mParent;
	Node<Key, Value>* mLeft;
	Node<Key, Value>* mRight;
};

/* 
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
	: mItem(key, value)
	, mParent(parent)
	, mLeft(NULL)
	, mRight(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the destructor in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<Key, Value>& Node<Key, Value>::getItem() const
{
	return mItem;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& Node<Key, Value>::getItem()
{
	return mItem;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
	return mItem.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
	return mItem.second;
}

/**
* A non-const getter for the key.
*/
template<typename Key, typename Value>
Key& Node<Key, Value>::getKey()
{
	return mItem.first;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
	return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
	return mParent;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
	return mLeft;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
	return mRight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
	mParent = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
	mLeft = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
	mRight = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
	mItem.second = value;
}

/* 
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
	BinarySearchTree();
	~BinarySearchTree();

	virtual void insert(const std::pair<Key, Value>& keyValuePair);
	void clear();
	void print() const;

public:
	/**
	* An internal iterator class for traversing the contents of the BST.
	*/
	class iterator
	{
	public:
		iterator(Node<Key,Value>* ptr);
		iterator();

		std::pair<Key,Value>& operator*();
		std::pair<Key,Value>* operator->();

		bool operator==(const iterator& rhs) const;
		bool operator!=(const iterator& rhs) const;
		iterator& operator=(const iterator& rhs);

		iterator& operator++();

	protected:
		Node<Key, Value>* mCurrent;
	};

public:
	iterator begin();
	iterator end();
	iterator find(const Key& key) const;

protected:
	Node<Key, Value>* internalFind(const Key& key) const;
	Node<Key, Value>* getSmallestNode() const;
	void printRoot (Node<Key, Value>* root) const;

	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */

	virtual void remove(const Key& key);
protected:
	Node<Key, Value>* mRoot;

};

/* 
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/ 

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
	: mCurrent(ptr)
{

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
	: mCurrent(NULL)
{

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*()
{
	return mCurrent->getItem(); 
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->()
{
	return &(mCurrent->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key, Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
	this->mCurrent = rhs.mCurrent;
	return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
	if(mCurrent->getRight() != NULL)
	{
		mCurrent = mCurrent->getRight();
		while(mCurrent->getLeft() != NULL)
		{
			mCurrent = mCurrent->getLeft();
		}
	}
	else if(mCurrent->getRight() == NULL)
	{
		Node<Key, Value>* parent = mCurrent->getParent();
		while(parent != NULL && mCurrent == parent->getRight())
		{
			mCurrent = parent;
			parent = parent->getParent();
		}
		mCurrent = parent;
	}
	return *this;
}

/* 
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/* 
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
	mRoot = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
	clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin()
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end()
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& key) const
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/**
* An insert method to insert into a Binary Search Tree. The tree will not remain balanced when
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	// If the BST is empty
	if (mRoot == NULL) {
		// Create new node with keyValuePair's information
		// Set Parent to null
		Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);

		// Add new node to BST as root
		mRoot = newNode;
	}

	// If BST has only one node
	else if (mRoot->getLeft() == NULL && mRoot->getRight() == NULL) {
		// If there is a duplicate entry
		if (keyValuePair.first == mRoot->getKey()) {
			// Update root's value
			mRoot->setValue(keyValuePair.second);
			return;
		}

		// Create new node with keyValuePair's information
		// Set Parent to mRoot
		Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, mRoot);

		// If new node is lesser than root
		if (newNode->getKey() < mRoot->getKey()) {
			// Set newNode as root's left child
			mRoot->setLeft(newNode);
		}

		// If new node is greater than root
		else {
			// Set newNode as root's right child
			mRoot->setRight(newNode);
		}
	}

	// If BST has more than one node
	else {
		Node<Key, Value>* currNode = mRoot;
		// Will store the parent of the new node that
		// needs to be added after the while loop finishes
		Node<Key, Value>* prevNode;

		// Create new node with keyValuePair's information
		// Set Parent to NULL (Will be changed after we find where to
		// insert in tree)
		Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);

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
* This function will implement BST Remove()
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
	// Do nothing if tree is empty
	if (mRoot == NULL) {
		return;
	}

	// Find node to delete
	Node<Key, Value>* nodeToDelete = internalFind(key);

	// If node to delete does not exist do nothing
	if (nodeToDelete == NULL) {
		return;
	}

	// Check if node to delete is the root
	if (nodeToDelete == mRoot) {
		// If root has no children
		if (nodeToDelete->getLeft() == NULL && nodeToDelete->getRight() == NULL) {
			// Set mRoot to null
			mRoot = NULL;

			// Free allocated memory
			delete nodeToDelete;
		}

		// If root has 2 children
		else if (nodeToDelete->getLeft() != NULL && nodeToDelete->getRight() != NULL) {
			// To store successor of nodeToDelete
			Node<Key, Value>* successor = nodeToDelete->getRight();

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
				mRoot = successor;

				// Free allocated memory
				delete nodeToDelete;
			}

			// If successor has right child 
			else if (successor->getRight() != NULL) {
				// Set successor's right as it's parent's left
				successor->getParent()->setLeft(successor->getRight());
				// Set successor's right's parent as it's parent
				successor->getRight()->setParent(successor->getParent());

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
				mRoot = successor;

				// Free allocated memory
				delete nodeToDelete;
			}

			// If successor is a leaf node
			else {
				// Set successor's parent's right to null
				successor->getParent()->setLeft(NULL);
				// Set successor's parent to null
				successor->setParent(NULL);

				// Update successor's left and right children
				successor->setLeft(nodeToDelete->getLeft());
				nodeToDelete->getLeft()->setParent(successor);

				successor->setRight(nodeToDelete->getRight());
				nodeToDelete->getRight()->setParent(successor);

				// Make successor the root
				mRoot = successor;

				// Free allocated memory
				delete nodeToDelete;
			}
		}

		// If root has only one child
		else {
			// If root has only left child
			if (nodeToDelete->getLeft() != NULL) {
				// Set child of root's parent to null
				nodeToDelete->getLeft()->setParent(NULL);

				// Promote child of root
				mRoot = nodeToDelete->getLeft();

				// Free allocated memory
				delete nodeToDelete;
			}

			// If root has only right child
			else {
				// Set child of root's parent to null
				nodeToDelete->getRight()->setParent(NULL);

				// Promote child of root
				mRoot = nodeToDelete->getRight();

				// Free allocated memory
				delete nodeToDelete;
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

				delete nodeToDelete;
			}

			// If node to delete is right child
			else {
				nodeToDelete->getParent()->setRight(NULL);

				delete nodeToDelete;
			}
		}

		// If node to delete has 2 children
		else if (nodeToDelete->getLeft() != NULL && nodeToDelete->getRight() != NULL) {
			// To store successor of nodeToDelete
			Node<Key, Value>* successor = nodeToDelete->getRight();

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

				// If nodeToDelete is the left child 
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(successor);
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(successor);
				}

				delete nodeToDelete;
			}

			// If successor has right child 
			else if (successor->getRight() != NULL) {
				// Set successor's right as it's parent's left
				successor->getParent()->setLeft(successor->getRight());
				// Set successor's right's parent as it's parent
				successor->getRight()->setParent(successor->getParent());

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
			}

			// If successor is a leaf node
			else {
				// Set successor's parent's left to null
				successor->getParent()->setLeft(NULL);

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
			}
		}

		// If node to delete has 1 child
		else {
			// If NTD has only left child
			if (nodeToDelete->getLeft() != NULL) {
				nodeToDelete->getLeft()->setParent(nodeToDelete->getParent());

				// If nodeToDelete is the left child
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(nodeToDelete->getLeft());
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(nodeToDelete->getLeft());
				}

				delete nodeToDelete;
			}

			// If NTD has only right child
			else {
				nodeToDelete->getRight()->setParent(nodeToDelete->getParent());

				// If nodeToDelete is the left child
				if (nodeToDelete->getParent()->getLeft() == nodeToDelete) {
					nodeToDelete->getParent()->setLeft(nodeToDelete->getRight());
				}

				// If nodeToDelete is the right child
				else {
					nodeToDelete->getParent()->setRight(nodeToDelete->getRight());
				}

				delete nodeToDelete;
			}
		}
	}
}

/**
* A method to remove all contents of the tree and reset the values in the tree
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	// Create iterator to traverse through the tree
	BinarySearchTree<Key, Value>::iterator it = begin();

	Node<Key, Value>* nodeToDelete;

	// Delete all nodes in the BST
	while (it != end()) {
		// Get pointer to nodeToDelete
		nodeToDelete = internalFind((*it).first);

		++it;
		// Delete node
		delete nodeToDelete;
	}
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
	// If BST is empty
	if (mRoot == NULL) {
		return NULL;
	}

	Node<Key, Value>* currNode = mRoot;

	// Find the left-most(smallest) node
	while (currNode->getLeft() != NULL) {
		currNode = currNode->getLeft();
	}

	// Return smallest node
	return currNode;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
	// If BST is empty
	if (mRoot == NULL) {
		return NULL;
	}

	Node<Key, Value>* currNode = mRoot;

	while (currNode != NULL) {
		// If we have found the node
		if (currNode->getKey() == key) {
			return currNode;
		}

		// If key is less than currNode go to left
		// child
		else if (key < currNode->getKey()) {
			currNode = currNode->getLeft();
		}

		// If key is greater than currNode go to right
		// child
		else {
			currNode = currNode->getRight();
		}
	}

	return NULL;
}

/**
* Helper function to print the tree's contents
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::printRoot (Node<Key, Value>* root) const
{
	if (root != NULL)
	{
		std::cout << "[";
		printRoot (root->getLeft());
		std::cout << " (" << root->getKey() << ", " << root->getValue() << ") ";
		printRoot (root->getRight());
		std::cout << "]";
	}
}

/* 
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif
