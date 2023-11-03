#include <iostream>

enum Color
{
	BLACK,
	RED
};

template<class K, class T>
struct RBNode 
{
	K key;
	T value;
	Color	color;
	RBNode* parent;
	RBNode* left;
	RBNode* right;

	RBNode()
	{
		parent = nullptr;
		left = nullptr; 
		right = nullptr;
		color = RED;
	}
	
	~RBNode()
	{
		if (left != nullptr)
		{
			delete left;
		}
		if (right != nullptr)
		{
			delete right;
	     
		}
	}
};