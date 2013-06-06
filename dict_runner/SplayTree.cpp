// This file contains the implementation of the SplayTree class.
// See "SplayTree.h" for its declaration.

#ifndef _SPLAYTREE_CPP
#define _SPLAYTREE_CPP

#include <algorithm>
#include "SplayTree.h"

/////////////////////////////////////////////////
// SPLTree methods.                            //
/////////////////////////////////////////////////

TEMPLATE
bool SPLT::insert(const KeyType & key, const ValueType & value) {
  // Perform a regular insert.
  // Note: given that insert doesn't return something more useful (such
  //       as a pointer to the node that was just inserted) we can either
  //       reproduce the entire function in BinarySearchTree.cpp with the
  //       one added splay(target) line, or we can just find the node
  //       after calling the parent, which makes for much cleaner code.
  int ret = BST::insert(key, value);

  // target will be either the sought after node or the parent
  // of where the sought after node should be, or NULL if empty tree
  BSTN * target = BST::findNode(key);

  // Splay the target up to root.
  splay(target);

  return ret;
}

// Remove the given key/value pair from the tree. Return true iff the
// key was in the tree.
TEMPLATE
bool SPLT::remove(const KeyType & key) {
  // TODO, although some splays might not necessarily do this:
  // After deletion, splay the parent of the removed node to the top of the tree.
  return BST::remove(key);
}

TEMPLATE
bool SPLT::contains(const KeyType & key) {
  // Try to find the target node.
  BSTN * target = findNode(key);
 
  // Only happens if the root is NULL
  if (target == NULL) {
    return false;
  } 
  // Otherwise, return true only if the target's key matches key. (If
  // it doesn't, target is the potential parent for key's new node,
  // were we inserting.)
  else {
    // Before returning, splay the target up to root.
    splay(target);
    return target->key == key;
  }
}

TEMPLATE
ValueType & SPLT::find(const KeyType & key) 
	 throw (KeyNotFoundException) {
  // target will be either the sought after node or the parent
  // of where the sought after node should be, or NULL if empty tree
  BSTN * target = BST::findNode(key);

  // It is an error to find the value of a node which is not in the
  // tree. If target is NULL or target is not actually the target node
  // (it is the parent of where the target node _should_ be), throw an
  // error.
  if (target != NULL && target->key == key) {
    // Splay the target up to root.
    splay(target);

  // Return the node value.
    return target->value;
  }

  throw KeyNotFoundException("Key not found in dictionary.");
}

TEMPLATE
void SPLT::splay(BSTN *& node) {
  // If the tree is not empty:
  if (node != NULL) {
    // Rotate the node up repeatedly until it is the root node.
    while (this->root != node) {
      BST::rotate(node);
    }
    this->root->parent = NULL;
  }
}

#endif
