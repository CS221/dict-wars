// Wrapper class around Weiss's binary search tree.

#ifndef _REDBLACKTREE_H
#define _REDBLACKTREE_H

#include <string>

#include "Dictionary.h"   // For the Dictionary abstract base type
#include "weiss/RedBlackTree.h"

using namespace std;

class RBBSTIterator;

class RBBSTNode {
 public:
 RBBSTNode(string k = " ", int v = 0) : key(k), value(v) { }
  string key;
  int value;
  friend bool operator< (const RBBSTNode & n1, const RBBSTNode & n2);
  friend bool operator== (const RBBSTNode & n1, const RBBSTNode & n2);
  friend bool operator!= (const RBBSTNode & n1, const RBBSTNode & n2);
};

bool operator< (const RBBSTNode & n1, const RBBSTNode & n2) {
  return n1.key < n2.key;
}

bool operator== (const RBBSTNode & n1, const RBBSTNode & n2) {
  return n1.key == n2.key;
}

bool operator!= (const RBBSTNode & n1, const RBBSTNode & n2) {
  return n1.key != n2.key;
}

class DRedBlackTree : public Dictionary<string, int> {
  RedBlackTree<RBBSTNode> tree;
 public:
 DRedBlackTree() : tree(RBBSTNode()) { }
  ~DRedBlackTree() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { tree.makeEmpty(); }

  virtual void remove(const string & key) { tree.remove(key); }

  virtual void insert(const string & key, const int & value) { 
    tree.insert(RBBSTNode(key, value)); 
  }

  virtual bool contains(const string & key) { 
    RBBSTNode node = tree.find(key); 
    return node.key != RBBSTNode().key; 
  }

  virtual const int & find(const string & key) throw(KeyNotFoundException) { 
    return tree.find(RBBSTNode(key, 0)).value; 
  }
};

#endif
