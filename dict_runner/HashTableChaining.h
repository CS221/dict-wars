// Wrapper class around Weiss's quadratic probing/resizing hash table.

#ifndef _HASHTABLECHAINING_H
#define _HASHTABLECHAINING_H


#include "Dictionary.h"   // For the Dictionary abstract base type
#include "UnsortedListDict.cpp"
#include <list>
#include <utility>

using namespace std;

typedef uint64_t HTChainingKeyType;
typedef uint64_t HTChainingValueType;
//typedef pair< HTChainingKeyType, HTChainingValueType > htc_entry_t;
typedef UnsortedListDict<HTChainingKeyType, HTChainingValueType> htc_entry_t;

#define HTC_TABLE_SIZE (10000)

class DHashTableChaining : public Dictionary<HTChainingKeyType, HTChainingValueType> {
  UnsortedListDict<HTChainingKeyType, HTChainingValueType> table[HTC_TABLE_SIZE];
    
  uint64_t hash(const uint64_t & key) {
    return key % HTC_TABLE_SIZE;
  }
 public:
 DHashTableChaining() { }
  ~DHashTableChaining() { }

  // Make the tree empty (frees memory).
  void makeEmpty() { 
    // Change to beautiful C++11 style map/foreach?
    // No: doesn't compile on some older g++'s, even when they have c++0x.
    for (int i = 0; i < HTC_TABLE_SIZE; i++) {
      table[i].makeEmpty(); 
    }
  }

  virtual void remove(const HTChainingKeyType & key) { 
    uint64_t slot = hash(key);
    table[slot].remove(key);

    /*
      // Keeping this code around b/c it's tricky to write correctly.
      // Notice the careful way we do deletes to avoid invalidating the iterator.
      htc_entry_t::iterator i = table[slot].begin();
      while (i != table[slot].end()) {
        list< htc_entry_t >:: iterator j = i;
        ++i;
        if (j->first == key)
          table[slot].erase(j);
        }
      // Notice that we remove ALL instances in case duplicates have been added.
      // IF WE WERE TO RESIZE, we'd first need to remove all duplicates!
    */
  }

  virtual void insert(const HTChainingKeyType & key, const HTChainingValueType & value) { 
    uint64_t slot = hash(key);
    table[slot].insert(key, value);
  }

  virtual bool contains(const HTChainingKeyType & key) { 
    uint64_t slot = hash(key);
    return table[slot].contains(key);

    /*
      // Keeping this code around in case it's useful for the future.
      for (list< htc_entry_t >::iterator i = table[slot].begin(); 
           i != table[slot].end(); 
	   ++i) {
        if (i->first == key)
          return true;
      }
    
    return false;
    */
  }

  virtual const HTChainingValueType & find(const HTChainingKeyType & key) throw(KeyNotFoundException) { 
    uint64_t slot = hash(key);
    return table[slot].find(key);
  }
};


#endif
