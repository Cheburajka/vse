#include <cstdlib>
#include <cstring>
#include <cassert>
#include <queue>
#include <vector>
#include <iostream>
#include "Node.h"

template<class K, class T>
class RBTree
{
private:
	int size;
	RBNode<K, T>* Root;
	int cmp(const K& a, const K& b) const;
	void leftRotate(RBNode<K, T>* node);
	void rightRotate(RBNode<K, T>* node);
	void removeNode(RBNode<K, T>* node);
public:
	RBTree()
	{
		Root = nullptr;
		size = 0;
	}
	~RBTree()
	{
		if (Root != nullptr)
		{
			delete Root;
		}
	};
	
	void Insert(K& key, T& value);
	bool remove(const K& key);
	bool search(const K& key, T& value) const;
	void Clear();
	void printTree() const;
	int getSize() const;
};

// insert (key, val) pair to appropriate position, then
// update the tree to keep its properties if needed.
// this->size will increase by 1.
template<class K, class T>
void RBTree<K, T>::Insert(K& key, T& value)
{
	auto* node = new RBNode<K, T>;
	node->key = key;
	node->value = value;
	// whether first node
	if (Root == nullptr) 
	{
		Root = node;
		node->color = BLACK;
		return;
	}
	// navigate to the desired positon of new node
	RBNode<K, T>* current = Root;
	while (current->left != nullptr | current->right != nullptr)
	{
		if (cmp(key, current->key) == -1)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}
	// link new node and its parent
	node->parent = current;
	if (cmp(key, current->key) == -1)
	{
		current->left = node;
	}
	else
	{
		current->right = node;
	}
	// update the tree recursively to keep its properties if needed
	while (current->color == RED && current->parent != nullptr)
	{
		bool isRight = (current == current->parent->right);
		RBNode<K, T>* uncle;
		if (isRight)
		{
			uncle = current->parent->left;
		}
		else
		{
			uncle = current->parent->right;
		}
		if (uncle == nullptr) 
		{
			current->color = BLACK;
			current->parent->color = RED;
			if (uncle == current->parent->right) 
			{
				rightRotate(current->parent);
			}
			else 
			{
				leftRotate(current->parent);
			}
			break;
		}
		else if (uncle->color == RED) 
		{
			current->color = BLACK;
			uncle->color = BLACK;
			current->parent->color = RED;
			current = current->parent;
		}
		else
		{
			current->color = BLACK;
			current->parent->color = RED;

			if (isRight)
			{
				if (node == current->left) 
				{
					rightRotate(current);
					current = node;
				}
				leftRotate(current->parent);
			}
			else
			{
				if (node == current->right) 
				{
					leftRotate(current);
					current = node;
				}
				rightRotate(current->parent);
			}
		}
		Root->color = BLACK;
	}
	this->size++;
}

// return 1 if success
// return 0 if not found
// remove the node with 'key' from tree, then
// update the tree to keep its properties if needed.
// this->size will decrease by 1
template<class K, class T>
bool RBTree<K, T>::remove(const K& key)
{
	// find the node to be deleted
	auto curr = Root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key)
		{
			break;
		}
		if (cmp(key, curr->key) >= 0)
		{
			curr = curr->right;
		}
		else
		{
			curr = curr->left;
		}
	}
	if (curr->key != key)
	{
		return 0;
	}
	
	this->removeNode(curr);
	(this->size)--;
	
	return 1;
}

// remove given node from this tree. 'node' must
// be in this tree
template<class K, class T>
void RBTree<K, T>::removeNode(RBNode<K, T>* node)
{
	if (node->color == RED) 
	{
		if (node->left != nullptr && node->right != nullptr)
		{
			// child x 2
			// find successor, then fill 'node' with successor
			auto successor = node->right;
			while (successor->left != nullptr)
			{
				successor = successor->left;
			}
			node->key = successor->key;
			node->value = successor->value;
			this->removeNode(successor);
		}
		else if (node->left != nullptr)
		{
			// only left child
			// fill 'node' with left child
			node->key = node->left->key;
			node->value = node->left->value;
			node->color = node->left->color;
			this->removeNode(node->left);
		}
		else if (node->right != nullptr)
		{
			// only right child
			// fill 'node' with right child
			node->key = node->right->key;
			node->value = node->right->value;
			node->color = node->right->color;
			this->removeNode(node->right);
		}
		else {
			// no child
			if (node->parent == nullptr) 
			{
				free(node);
				Root = nullptr;
				return;
			}

			if (node->parent->left == node)
			{
				node->parent->left = nullptr;
			}
			else
			{
				node->parent->right = nullptr;
			}
			free(node);
		}
	}
	else 
	{
		if (node->left != nullptr && node->right != nullptr)
		{
			// child x 2
				// find successor, then fill 'node' with successor
			auto successor = node->right;
			while (successor->left != nullptr)
			{
				successor = successor->left;
			}
			node->key = successor->key;
			node->value = successor->value;
			this->removeNode(successor);
		}
		else if (node->left != nullptr) 
		{
			// only left child
			// fill 'node' with left child
			node->key = node->left->key;
			node->value = node->left->value;
			this->removeNode(node->left);
		}
		else if (node->right != nullptr) 
		{
			// only right child
			// fill 'node' with right child
			node->key = node->right->key;
			node->value = node->right->value;
			this->removeNode(node->right);
		}
		else 
		{
			// no child. As the black node deleted is a leaf, fixup
			// is neccesary after delete.

			if (node->parent == nullptr) 
			{
				// 'node' is root
				free(node);
				Root = nullptr;
				return;
			}

			if (node->parent->left == node) 
			{
				node->parent->left = nullptr;
				if (node->parent->right != nullptr && node->parent->right->color == RED) 
				{
					node->parent->right->color = BLACK;
					leftRotate(node->parent);
				}
			}
			else
			{
				node->parent->right = nullptr;
				if (node->parent->left != nullptr && node->parent->left->color == RED) 
				{
					node->parent->left->color = BLACK;
					rightRotate(node->parent);
				}
			}
			if (node->parent->left == nullptr && node->parent->right == nullptr && node->parent->parent != nullptr) 
			{
				// you can guess, 'node->parent->parent' must be RED
				rightRotate(node->parent->parent);
			}

			free(node);
		}
	}
}

// return 1 if success
// return 0 if not found
// find the node with 'key' in this tree, then set
// 'val' to the val of this node
template<class K, class T>
bool RBTree<K, T>::search(const K& key, T& val) const
{
	auto curr = Root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key)
		{
			val = curr->value;
			break;
		}

		if (cmp(key, curr->key) < 0)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}

	if (curr->key != key)
	{
		return 0;
	}
	return 1;
}

// compare 'a' and 'b'
// return -1 if a < b
// return 0 if a = b
// return 1 if a > b
template<class K, class T>
int RBTree<K, T>::cmp(const K& a, const K& b) const
{
	if (typeid(a) == typeid(char*)) {
		// string type
		char* x = (char*)a;
		char* y = (char*)b;
		return strcmp(x, y);
	}
	else {
		if (a < b) return -1;
		if (a == b) return 0;
		if (a > b) return 1;
	}
	return 0;
}

// take 'node' as the center, make a left rotation
template<class K, class T>
void RBTree<K, T>::leftRotate(RBNode<K, T>* node)
{
	assert(node->right != nullptr);
	auto temp = node->right;

	// update the two nodes
	node->right = temp->left;
	if (temp->left != nullptr)
		temp->left->parent = node;
	temp->left = node;
	temp->parent = node->parent;
	node->parent = temp;

	// update the parent
	if (Root == node) {
		Root = temp;
		return;
	}
	if (temp->parent->left == node)
		temp->parent->left = temp;
	else
		temp->parent->right = temp;
}

// take 'node' as the center, make a right rotation
template<class K, class T>
void RBTree<K, T>::rightRotate(RBNode<K, T>* node)
{
	auto temp = node->left;

	// update the two nodes
	node->left = temp->right;
	if (temp->right != nullptr)
		temp->right->parent = node;
	temp->right = node;
	temp->parent = node->parent;
	node->parent = temp;

	// update the parent
	if (Root == node)
	{
		Root = temp;
		return;
	}
	if (temp->parent->left == node)
	{
		temp->parent->left = temp;
	}
	else
	{
		temp->parent->right = temp;
	}
}

// From the root to the leaf, send 'node',
// 'node's left sub-tree,'node's right sub-tree to stdout
template<class K, class T>
void RBTree<K, T>::printTree() const
{
	std::cout << "----------------" << std::endl;
	std::queue<RBNode<K, T>*> q;
	q.push(Root);
	while (!q.empty())
	{
		auto top = q.front();
		q.pop();
		if (top->color == RED)
			std::cout << "R";
		else
			std::cout << "B";
		std::cout << top->key;
		std::cout << " ";
		if (top->left != nullptr) {
			q.push(top->left);
			if (top->left->color == RED)
				std::cout << "R";
			else
				std::cout << "B";
			std::cout << top->left->key;
			std::cout << " ";
		}
		else {
			std::cout << "NULL" << " ";
		}
		if (top->right != nullptr) {
			q.push(top->right);
			if (top->right->color == RED)
				std::cout << "R";
			else
				std::cout << "B";
			std::cout << top->right->key;
			std::cout << " ";
		}
		else {
			std::cout << "NULL" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "----------------" << std::endl;
}

// Return the number of nodes
template<class K, class T>
int RBTree<K, T>::getSize() const
{
	return this->size;
}

// delete all nodes in this tree, recycle the memory
template<class K, class T>
void RBTree<K, T>::Clear()
{
	delete this->Root;
	this->Root = nullptr;
}