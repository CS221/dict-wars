// Wrapper class around Weiss's binary search tree.

#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <string>

#include "Dictionary.h"   // For the Dictionary abstract base type

typedef uint64_t AKeyType;
typedef uint64_t AValueType;

class ABSTNode {
 public:
 ABSTNode(AKeyType k = 0, AValueType v = 0, bool d = false) : key(k), value(v), deleted(d) { }
  AKeyType key;
  AValueType value;
  bool deleted;
  friend bool operator< (const ABSTNode & n1, const ABSTNode & n2);
  friend bool operator== (const ABSTNode & n1, const ABSTNode & n2);
  friend bool operator!= (const ABSTNode & n1, const ABSTNode & n2);
};

#include "weiss/AvlTree.h"

using namespace std;

bool operator< (const ABSTNode & n1, const ABSTNode & n2) {
  return n1.key < n2.key;
}

bool operator== (const ABSTNode & n1, const ABSTNode & n2) {
  return n1.key == n2.key;
}

bool operator!= (const ABSTNode & n1, const ABSTNode & n2) {
  return n1.key != n2.key;
}

class DAvlTree : public Dictionary<AKeyType, AValueType> {
  const bool TOMBSTONED;
  AvlTree<ABSTNode> tree;
 public:
 DAvlTree(bool tombstoned = false) : TOMBSTONED(tombstoned), tree(ABSTNode()) { }
  ~DAvlTree() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { tree.makeEmpty(); }

  virtual void remove(const AKeyType & key) { 
    if (TOMBSTONED)
      tree.find(ABSTNode(key)).deleted = true; 
    else
      tree.remove(ABSTNode(key));
  }

  virtual void insert(const AKeyType & key, const AValueType & value) { 
    ABSTNode toInsert(key, value);
    ABSTNode & node = tree.find(toInsert);
    if (node != ABSTNode()) {
      node.deleted = false;
      node.value = value;
    }
    else {
      tree.insert(toInsert); 
    }
  }

  virtual bool contains(const AKeyType & key) { 
    ABSTNode node = tree.find(key); 
    return node.key != ABSTNode().key && !node.deleted; 
  }

  virtual const AValueType & find(const AKeyType & key) throw(KeyNotFoundException) { 
    static ABSTNode NOT_FOUND;
    NOT_FOUND = ABSTNode();

    ABSTNode & node = tree.find(ABSTNode(key));
    if (node.deleted)
      return NOT_FOUND.value;
    else
      return node.value;
  }
};

#endif
