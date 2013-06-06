//
//  Trie.cpp
//  PA2_Trie
//
//  Created by Kimberly Voll on 11-06-09.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


// **TODO**
// Grab keysize (rather than rely on it beingn passed in) to interface with Dictionary.h


#include "Trie.h"
#include <iostream>

const int MAXSIZE=10; // Maximum number of children (assuming integer keys, therefore 10)

// Default constructor:
// --
// Creates a default node as the root of the trie
// and an empty array of child pointers
Trie::Trie() 
{
    root = new Node;
}


// insert (key, keysize, value): inserts new node in trie
// --
// Traverses trie via key path.
// If node does not exist at end of path, create and increase
// the child count of the parent to the new node.
void Trie::insert(const Key & key, const Value & value) 
{
    const int & keysize = calcKeySize(key);
    int keyElem = 0;
    Node* tmp = root;
    while( keyElem < keysize ) 
    {
        int index = pickDigit(key, keysize - keyElem - 1); // head of current key
        // Check to see if path has already been traversed (i.e. exists):
        if(tmp->childArray[index] == 0) 
        {
            tmp->childArray[index] = new Node;
            tmp->numChildren++;  // Only increase when adding new children/nodes
        }
        // Traverse:
        tmp = tmp->childArray[index];
        // Label path from key elements while traversing:
        tmp->label = index;
        keyElem++;
    } // end while
    
    //  Store the data:
    tmp->value = value;
    tmp->key = key;
}


// find(key,keysize): lookup and return value of node in trie
// --
// Calls findNode.  For interfacing with Dictionary.h
const Value & Trie::find(const Key & key) throw (KeyNotFoundException)
{
    Node* tmp_v = Trie::findNode(key, calcKeySize(key));
    if(tmp_v == 0) 
        throw KeyNotFoundException("Node was not found");
    else return tmp_v->value;
}

// findNode(key,keysize): lookup and return node in trie
// --
// Traverses the trie based on the elements in the key
// A pointer to the node is returned if found, NULL o/w.
Trie::Node* Trie::findNode(const Key & key, const int keysize) 
{
    int keyElem = 0;
    Node* tmp = root;

    while( keyElem < keysize ) 
    {
        int index = pickDigit(key, keysize - keyElem - 1); // head of current key
        // Check to see if path has already been traversed (i.e. exists):
        if(tmp->childArray[index] == 0) return 0; // not found
        // Traverse:
        tmp = tmp->childArray[index];
        keyElem++;
    } // end while
    
    //  **TODO** 
    //  Right now I return the node stored here w/o checking
    //  the key to see if it matches.  This is to allow for
    //  for searches that return the parent of the node, not 
    //  the actual node, but feels a bit kludgy
    /*if( tmp )&& tmp->key == key ) 
     {
        return tmp;
    }*/
    
    return tmp;
}

// remove(key,keysize): deletes key from trie
// --
// Finds the parent of key node.  
// If key node has no children) delete and decrement parent's child count.  
// Else delete data stored at node but keep node
void Trie::remove(const Key & key)
{
    int second2lastIndex = calcKeySize(key) - 1; // Root is 0
    // Find the parent node:    
    Node* parent= findNode(key, second2lastIndex);    
    if( !parent ) return; // TODO: throw exception?
    
    // Fetch relevant node from parent:
    Node* node = parent->childArray[pickDigit(key, 0)];    
    if( !node ) return; // TODO: throw exception?
        
    // If node has no children, safe to delete:
    if( node->numChildren <= 0 ) 
    {
        delete node;
        parent->childArray[pickDigit(key,0)] = 0;
    } 
    else // Otherwise leave in place but remove stored information, 
         // except for label: 
    { 
        node->value = 0;
       // if(node->key) delete[] node->key;  **TODO** too tired to think, ha!
        node->key = 0;
    }
}


// makeEmpty: prints the trie associated with this object
void Trie::makeEmpty() 
{
    if( !root ) return;
    delete root;
    root = new Node;
}


// printTree: prints the trie associated with this object
void Trie::printTree() 
{
    printTreeAux(root);
}

// printTreeAux (root): prints the trie at root
// --
// Print the contents of the current node and
// recursively traverse node's existing children.
void Trie::printTreeAux( Node* root ) 
{
    if( !root ) return;
    std::cout << "Node Label: " << root->label << " Value: " << root->value << " Num Child: " << root->numChildren << " || ";
    for( int i=0; i<MAXSIZE; i++ ) 
    {
        if( root->childArray[i] != 0 ) printTreeAux(root->childArray[i]);
    } // end for
    std::cout << std::endl;
}


// getRoot: returns root of trie
Trie::Node* Trie::getRoot() {
    return root;
}


