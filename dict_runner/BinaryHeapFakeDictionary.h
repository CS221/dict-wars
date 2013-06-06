// TODO!!! test, incorporate into dict_runner.cpp 

// Wrapper class around Weiss's quadratic probing/resizing hash table.

#ifndef _BINARYHEAPFAKEDICTIONARY_H
#define _BINARYHEAPFAKEDICTIONARY_H


#include "Dictionary.h"   // For the Dictionary abstract base type
#include <algorithm>
#include <vector>

using namespace std;

typedef uint64_t BHeapKeyType;
typedef uint64_t BHeapValueType;
typedef pair< BHeapKeyType, BHeapValueType > bheap_entry_t;

class BinaryHeapFakeDictionary;

// The comparator is for a MAX heap; so, reverse it to get the desired min heap.
bool bheapEntryCompare(bheap_entry_t & bhel, 
		       bheap_entry_t & bher) {
  // intentionally reversed
  return bhel.first > bher.first;
}

class BinaryHeapFakeDictionary : public Dictionary<BHeapKeyType, BHeapValueType> {
  vector<bheap_entry_t> array;
    
 public:
 BinaryHeapFakeDictionary() { }
  ~BinaryHeapFakeDictionary() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { 
    // Change to beautiful C++11 style map/foreach?
    array.clear();
  }

  virtual void remove(const BHeapKeyType & key) { 
    // This is NOT dictionary remove.  It IGNORES the given key and
    // deletes the minimum element (if there is one).
    if (array.size() == 0)
      return;

    pop_heap(array.begin(), array.end(), bheapEntryCompare);
    array.pop_back();
  }

  virtual void insert(const BHeapKeyType & key, const BHeapValueType & value) { 
    array.push_back(make_pair(key, value));
    push_heap(array.begin(), array.end(), bheapEntryCompare);
  }

  virtual bool contains(const BHeapKeyType & key) { 
    // This is NOT dictionary contains.  It IGNORES the given key and
    // returns true, simulating the quick "findmin" operation.
    return true;
  }

  virtual const BHeapValueType & find(const BHeapKeyType & key) throw(KeyNotFoundException) { 
    // This is NOT dictionary contains.  It IGNORES the given key and
    // returns true, simulating the quick "findmin" operation.
    if (array.size() == 0)
      throw KeyNotFoundException("");

    return array[0].second;
  }
};


#endif
