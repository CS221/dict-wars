// Wrapper class around Weiss's binary search tree.

#ifndef _SPLAYTREE_H
#define _SPLAYTREE_H

#include "Dictionary.h"   // For the Dictionary abstract base type
#include "weiss/SplayTree.h"

typedef uint64_t SKeyType;
typedef uint64_t SValueType;

using namespace std;

class SBSTIterator;

class SBSTNode {
 public:
 SBSTNode(SKeyType k = 0, SValueType v = 0) : key(k), value(v) { }
  SKeyType key;
  SValueType value;
  friend bool operator< (const SBSTNode & n1, const SBSTNode & n2);
  friend bool operator== (const SBSTNode & n1, const SBSTNode & n2);
  friend bool operator!= (const SBSTNode & n1, const SBSTNode & n2);
};

bool operator< (const SBSTNode & n1, const SBSTNode & n2) {
  return n1.key < n2.key;
}

bool operator== (const SBSTNode & n1, const SBSTNode & n2) {
  return n1.key == n2.key;
}

bool operator!= (const SBSTNode & n1, const SBSTNode & n2) {
  return n1.key != n2.key;
}

class DSplayTree : public Dictionary<SKeyType, SValueType> {
  SplayTree<SBSTNode> tree;
 public:
 DSplayTree() : tree(SBSTNode()) { }
  ~DSplayTree() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { tree.makeEmpty(); }

  virtual void remove(const SKeyType & key) { tree.remove(key); }

  virtual void insert(const SKeyType & key, const SValueType & value) { 
    tree.insert(SBSTNode(key, value)); 
  }

  virtual bool contains(const SKeyType & key) { 
    SBSTNode node = tree.find(key); 
    return node.key != SBSTNode().key; 
  }

  virtual const SValueType & find(const SKeyType & key) throw(KeyNotFoundException) { 
    return tree.find(SBSTNode(key, 0)).value; 
  }
};

#endif
