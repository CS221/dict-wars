// Wrapper class around Weiss's quadratic probing/resizing hash table.

#ifndef _HASHTABLEQP_H
#define _HASHTABLEQP_H


#include "Dictionary.h"   // For the Dictionary abstract base type
#include "weiss/QuadraticProbing.h"

typedef uint64_t HTQPKeyType;
typedef uint64_t HTQPValueType;

using namespace std;

class HQPNode {
 public:
 HQPNode(HTQPKeyType k = 0, HTQPValueType v = 0) : key(k), value(v) { }
  HTQPKeyType key;
  HTQPValueType value;
  friend bool operator< (const HQPNode & n1, const HQPNode & n2);
  friend bool operator== (const HQPNode & n1, const HQPNode & n2);
  friend bool operator!= (const HQPNode & n1, const HQPNode & n2);
};

unsigned int weiss_hash( const HQPNode & key, uint64_t tableSize ) {
  return weiss_hash(key.key, tableSize);
}

bool operator< (const HQPNode & n1, const HQPNode & n2) {
  return n1.key < n2.key;
}

bool operator== (const HQPNode & n1, const HQPNode & n2) {
  return n1.key == n2.key;
}

bool operator!= (const HQPNode & n1, const HQPNode & n2) {
  return n1.key != n2.key;
}

class DHashTableQP : public Dictionary<HTQPKeyType, HTQPValueType> {
  HashTable<HQPNode> tree;
 public:
 DHashTableQP(bool useQP = true) : tree(HQPNode(), useQP) { }
  ~DHashTableQP() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { tree.makeEmpty(); }

  virtual void remove(const HTQPKeyType & key) { tree.remove(key); }

  virtual void insert(const HTQPKeyType & key, const HTQPValueType & value) { 
    tree.insert(HQPNode(key, value)); 
  }

  virtual bool contains(const HTQPKeyType & key) { 
    HQPNode node = tree.find(key); 
    return node.key != HQPNode().key; // BUG: found 2011W2 -- contains() call fails if key is 0
  }

  virtual const HTQPValueType & find(const HTQPKeyType & key) throw(KeyNotFoundException) { 
    return tree.find(HQPNode(key, 0)).value; 
  }
};


#endif
