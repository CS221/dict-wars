//
//  Trie.h
//  PA2_Trie
//
//  Created by Kimberly Voll on 11-06-09.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef TRIE_H
#define TRIE_H

#include <cmath>
#include <cstdlib>
#include <iostream> // for debugging!

#include "Dictionary.h"

typedef uint64_t Key;
typedef uint64_t Value;
typedef int Label;

class Trie : public Dictionary<Key, Value>
{
    
    struct Node 
    {
        Node()
        {
            label = 0; 
            value = 0; 
            key = 0;
            numChildren = 0;
	    for (int i = 0; i < 10; i++)
	      childArray[i] = 0;
        }
        ~Node()
        {
            // **TODO** loop through to delete nodes
	  //std::cout << "num children? " << numChildren << std::endl;
	  //std::cout << "deleting child array\n";
	    for (int i = 0; i < 10; i++)
	      if (childArray[i] != 0) {
		delete childArray[i];
		childArray[i] = 0;
	      }
        }
        Value value;
        Key key;
        Label label;
        int numChildren;
        Node *childArray[10];
    };
    
public:
    Trie();
    virtual ~Trie() { delete root; root = 0; }
    
    virtual void insert(const Key & key, const Value & value);
    virtual const Value & find(const Key & key) throw (KeyNotFoundException);

    virtual void remove(const Key & key);

    // added 2011W2 (see Dictionary.h) -- won't usually get inlined anyway. Oh well.
    virtual inline bool contains(const Key & key) {
        return 0 != Trie::findNode(key, calcKeySize(key));
    }

    virtual void makeEmpty(); 
    
    void printTree();
    
    Node* getRoot();
    
    Node* findNode(const Key & key, const int keySize); // TODO: public for debugging purposes??

private:

    // Number of base 10 digits in (non-negative) key.  Note that 0
    // counts as having zero digits.
    int calcKeySize(const Key & key) {
      return (int)ceil(log10(key + 1));
    }

    // The digit-th digit in key, counting from the RIGHT.
    int pickDigit(const Key & key, int digit) {
      return (int)(((Key)(key / pow(10.0, digit))) % 10);
    }
    
    Node* root;
    
    void makeEmptyAux( Node* root );
    void printTreeAux( Node* root );
    
};

#endif
