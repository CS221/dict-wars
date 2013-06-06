#ifndef _UNSORTEDLISTDICT_CPP
#define _UNSORTEDLISTDICT_CPP

#include "Dictionary.h"
#include "UnsortedListDict.h"

#include <utility>
#include <string>
#include <iostream>
#include <list>

using namespace std;

template <class KeyType, class ValueType>
UnsortedListDict<KeyType, ValueType>::UnsortedListDict(enum ULDDupeHandlingType insertSemantics, bool moveToFront) : moveToFront(moveToFront), insertSemantics(insertSemantics)
{

}

template <class KeyType, class ValueType>
UnsortedListDict<KeyType, ValueType>::~UnsortedListDict()
{
  makeEmpty();
}

template <class KeyType, class ValueType>
int UnsortedListDict<KeyType, ValueType>::size() const {
  return unsortedList.size();
}

template <class KeyType, class ValueType>
void UnsortedListDict<KeyType, ValueType>::remove(const KeyType & key) {
  if (insertSemantics == OVERWRITE_VIA_REMOVE_ALL) {
    llistIterator iter = find_if(unsortedList.begin(), unsortedList.end(), KeyFinder(key));
    while (iter != unsortedList.end()) {
      llistIterator old = iter;
      ++iter;
      unsortedList.erase(old);
      iter = find_if(iter, unsortedList.end(), KeyFinder(key));
    }
  }
  else {
    llistIterator iter = find_if(unsortedList.begin(), unsortedList.end(), KeyFinder(key));
    if (iter != unsortedList.end())
      unsortedList.erase(iter);
  }
}

template <class KeyType, class ValueType>
void UnsortedListDict<KeyType, ValueType>::insert(const KeyType & key, const ValueType & value) {
  pair<KeyType, ValueType> pair(key, value);
  if (insertSemantics == OVERWRITE_VIA_REPLACE_ON_INSERT) {
    // Must eliminate the old "binding".
    if (moveToFront) {
      // Easy; can just call remove.
      remove(key);
      unsortedList.push_front(pair);
    }
    else {
      // Find the old copy and tweak it.
      llistIterator iter = find_if(unsortedList.begin(), unsortedList.end(), KeyFinder(key));
      if (iter == unsortedList.end()) {
	// No old copy; just push the new value in.
	unsortedList.push_front(pair);
      }
      else {
	// Alter old copy.
	iter->second = value;
      }
    }
  }
  else
    // Just put at front regardless.
    unsortedList.push_front(pair);
}

template <class KeyType, class ValueType>
bool UnsortedListDict<KeyType, ValueType>::contains(const KeyType & key) {
  llistIterator iter = find_if(unsortedList.begin(), unsortedList.end(), KeyFinder(key));

  if (iter == unsortedList.end())
    return false;
  else {
    if (moveToFront) {
      unsortedList.push_front(*iter);
      unsortedList.erase(iter);
    }
    return true;
  }
}


template <class KeyType, class ValueType>
const ValueType & UnsortedListDict<KeyType, ValueType>::find(const KeyType & key) throw(KeyNotFoundException) {
  llistIterator iter = find_if(unsortedList.begin(), unsortedList.end(), KeyFinder(key));

  if (iter == unsortedList.end())
    throw KeyNotFoundException("Key Not Found");

  if (moveToFront) {
    unsortedList.push_front(*iter);
    unsortedList.erase(iter);
    return unsortedList.begin()->second;
  }
  else
    return iter->second;
}

template <class KeyType, class ValueType>
void UnsortedListDict<KeyType, ValueType>::makeEmpty() {
  unsortedList.clear();
}

template <class KeyType, class ValueType>
void UnsortedListDict<KeyType, ValueType>::printDict() {
    llistIterator iter;
    
    for (iter = unsortedList.begin(); iter != unsortedList.end(); iter++ ) {
	pair<KeyType, ValueType> pair = *iter;
	cout << pair.first << "::" << pair.second << " ,";
    }
    
    cout << endl;
}

// int main(int argc, char *argv[]) {
//     Dictionary<int, int> *dictionary2;
//     dictionary2 = new UnsortedListDict<int, int>(true);
    
//     UnsortedListDict<int, int> * dictionary = dynamic_cast< UnsortedListDict<int, int>* >(dictionary2);
    
//     dictionary->insert(1, 1);
//     dictionary->insert(2, 2);
//     dictionary->insert(3, 3);
//     dictionary->insert(4, 4);
//     dictionary->insert(5, 5);
//     dictionary->insert(6, 6);
    
//     dictionary->find(6);
//     dictionary->find(4);

//     dictionary->remove(4);
//     dictionary->remove(2);
//     dictionary->remove(1);
    
//     dictionary->printDict();
// }

#endif
