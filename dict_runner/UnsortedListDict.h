#ifndef _UNSORTEDLISTDICT_H
#define _UNSORTEDLISTDICT_H

#include "Dictionary.h"

#include <utility>
#include <string>
#include <iostream>
#include <list>


using namespace std;

enum ULDDupeHandlingType { OVERWRITE_VIA_REPLACE_ON_INSERT, 
			   OVERWRITE_VIA_REMOVE_ALL,
			   ALLOW_MULTIPLE };			   

template <class KeyType, class ValueType>
class UnsortedListDict : public Dictionary<KeyType, ValueType> {

public:
  UnsortedListDict(enum ULDDupeHandlingType insertSemantics = ALLOW_MULTIPLE, 
		   bool moveToFront = false);
  ~UnsortedListDict();
  
  void makeEmpty();
  int size() const;
  virtual void remove(const KeyType & key);
  virtual void insert(const KeyType & key, const ValueType & value);
  virtual bool contains(const KeyType & key);
  
  // Return a (changeable) reference to the value indexed by the given
  // key. It is an error to call find with a key that is not in the
  // tree.
  virtual const ValueType & find(const KeyType & key)
    throw(KeyNotFoundException);
	    
  void printDict();

private:
  typedef typename std::list< pair<KeyType,ValueType> >::iterator llistIterator;

  // This is the doublly linked list
  list< pair<KeyType,ValueType> > unsortedList;

  // Should we move an element to the front each time it's accessed?
  const bool moveToFront;

  // How should we handle duplicates on insertion?
  const enum ULDDupeHandlingType insertSemantics;

  struct KeyFinder {
    KeyFinder(const KeyType & k) : key(k) { }
    bool operator()(const pair<KeyType, ValueType> & pair) {
      return pair.first == key;
    }
    const KeyType & key;
  };
};

#include "UnsortedListDict.cpp"

#endif
