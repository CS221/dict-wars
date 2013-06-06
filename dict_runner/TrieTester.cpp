#include "Trie.h"
#include <iostream>

int main (int argc, const char * argv[])
{
    
    Trie test;
    test.printTree();
    int keysize = 3;
    int key1 = 123;
    test.insert(key1, 12);
    std::cout << "Printing first tree: \n";
    test.printTree();
    
    int key2 = 124;
    test.insert(key2, 21);
    
    std::cout << "Printing second tree: \n";
    test.printTree();
    
    int key3 = 12;
    test.insert(key3, 144);
    
    std::cout << "Printing third tree: \n";
    test.printTree();
    
    std::cout << test.find(key1) << std::endl;
    test.remove(key1);
 //   std::cout << test.find(key2)->value;

    test.printTree();
    
    if( !test.findNode(key3,2) ) std::cout << "not found";
    
    std::cout << "Testing make empty: \n";
    test.makeEmpty();
    
  //  std::cout << test.getRoot()->numChildren << std::endl;
    
  //  delete [] test.getRoot()->childArray;
 //   test.insert(key1,keysize,10);
    
    test.printTree();
    
    return 0;
}
