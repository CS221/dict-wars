// Wrapper class around Weiss's binary search tree.

#ifndef _BINARYSEARCHTREE_H
#define _BINARYSEARCHTREE_H

#include "Dictionary.h"   // For the Dictionary abstract base type
#include "weiss/BinarySearchTree.h"

using namespace std;

typedef uint64_t BKeyType;
typedef uint64_t BValueType;

class DBSTIterator;

class DBSTNode {
 public:
 DBSTNode(BKeyType k = 0, BValueType v = 0, bool d = false) : key(k), value(v), deleted(d) { }
  BKeyType key;
  BValueType value;
  bool deleted;
  friend bool operator< (const DBSTNode & n1, const DBSTNode & n2);
};

bool operator< (const DBSTNode & n1, const DBSTNode & n2) {
  return n1.key < n2.key;
}

class DBinarySearchTree : public Dictionary<BKeyType, BValueType> {
  const bool TOMBSTONED;
  BinarySearchTree<DBSTNode> tree;
 public:
 DBinarySearchTree(bool tombstoned = false) : TOMBSTONED(tombstoned), tree(DBSTNode()) { }
  ~DBinarySearchTree() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { tree.makeEmpty(); }

  virtual void remove(const BKeyType & key) { 
    if (TOMBSTONED)
      tree.find(DBSTNode(key)).deleted = true; 
    else
      tree.remove(DBSTNode(key));
  }

  virtual void insert(const BKeyType & key, const BValueType & value) { 
    DBSTNode toInsert(key, value);
    DBSTNode & node = tree.find(toInsert);
    if (node.key != DBSTNode().key) {
      node.deleted = false;
      node.value = value;
    }
    else {
      tree.insert(toInsert); 
    }
  }

  virtual bool contains(const BKeyType & key) { 
    DBSTNode node = tree.find(key); 
    return node.key != DBSTNode().key && !node.deleted; 
  }

  virtual const BValueType & find(const BKeyType & key) throw(KeyNotFoundException) { 
    static DBSTNode NOT_FOUND;
    NOT_FOUND = DBSTNode();

    DBSTNode & node = tree.find(DBSTNode(key));
    if (node.deleted)
      return NOT_FOUND.value;
    else
      return node.value;
  }
};

#endif
