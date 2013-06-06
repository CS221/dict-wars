// This is implemented using a sorted ARRAY (C++ vector).  
// The word "list" does NOT refer to a linked list.
//
// Presently, insert and remove search linearly from end for good best
// case. Removing an element that's not there is O(lg n) w/binary
// search but O(n) w/linear.  (This is a likely common case.)
//
// If duplicates are ignored on insert, then insert has the same
// problem.
//
// To switch, just call findIterBSearch rather than
// findIterLSearchFromEnd in insert nad/or remove.

#ifndef _SORTEDLISTDICT_H
#define _SORTEDLISTDICT_H

#include "Dictionary.h"

#include <utility>
#include <string>
#include <iostream>
#include <vector>
//#include <cstdint>

using namespace std;

template <class KeyType, class ValueType>
class SLDComparator {
public:
  bool operator()(const std::pair<KeyType, ValueType> & a,
		  const std::pair<KeyType, ValueType> & b) {
    return a.first < b.first;
  }
};

template <class KeyType, class ValueType, class ListStore = std::vector<std::pair<KeyType, ValueType> > >
class SortedListDict : public Dictionary<KeyType, ValueType> {

public:
  SortedListDict();
  ~SortedListDict();

  // Make the vector empty
  void makeEmpty();
	
  // Return the number of elements in the dictioanry
  uint64_t size() const;

  // Remove the given key/value pair from the tree. 
  virtual void remove(const KeyType & key);

  // Insert the data "value" indexed by "key" into the table. 
  // Inserts the key if the key is not already in the
  // table.  Does not insert the key otherwise.
  virtual void insert(const KeyType & key, const ValueType & value);

  // Check if the dictionary contains an entry indexed by "key".
  virtual bool contains(const KeyType & key);

  // Return a (changeable) reference to the value indexed by the given
  // key. It is an error to call find with a key that is not in the
  // tree.
  virtual const ValueType & find(const KeyType & key)
    throw(KeyNotFoundException);
	    
  void printDict();

private:
  // Using binary search to find the position in the vector where 
  // the key is or where the key is supposed to be next to
  //int findPosition(const KeyType & key, int start, int end);
  // Replaced with STL operations.

  typedef typename ListStore::iterator iterator;
  iterator findIterBSearch(const KeyType & key);
  iterator findIterLSearchFromEnd(const KeyType & key);

  // This is the vector from the c++ STL
  // The element in the list is stored in increasing order
  ListStore sortedList;
};

#include "SortedListDict.cpp"

#endif
