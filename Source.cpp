#include <iostream>
#include "rbtree.h"

int main()
{
    RBTree<int, char> t;
    int key;
    char val;
    
    for (int i = 0; i < 15; i++)
    {
        key = i;
        val = ('a' + i);
        t.Insert(key, val);
    }
    t.printTree();

    key = 7;
    t.remove(key);
    key = 10;
    t.remove(key);

    t.printTree();
    std::cout << t.getSize() << std::endl;
    
    return 0;
}