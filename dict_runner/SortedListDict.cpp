#ifndef _SORTEDLISTDICT_CPP
#define _SORTEDLISTDICT_CPP

#include "Dictionary.h"
#include "SortedListDict.h"
#include <algorithm>
#include <list>

using namespace std;

template <class KeyType, class ValueType, class ListStore>
SortedListDict<KeyType, ValueType, ListStore>::SortedListDict()
{

}

template <class KeyType, class ValueType, class ListStore>
SortedListDict<KeyType, ValueType, ListStore>::~SortedListDict()
{
  makeEmpty();
}

template <class KeyType, class ValueType, class ListStore>
uint64_t SortedListDict<KeyType, ValueType, ListStore>::size() const {
  return sortedList.size();
}

template <class KeyType, class ValueType, class ListStore>
typename SortedListDict<KeyType, ValueType, ListStore>::iterator 
SortedListDict<KeyType, ValueType, ListStore>::findIterBSearch(const KeyType & key) {
  SLDComparator<KeyType, ValueType> sldcomp;
  std::pair<KeyType, ValueType> pair(key, ValueType());

  iterator iter = lower_bound(sortedList.begin(), sortedList.end(), pair, sldcomp);
  return iter;
}



template <class KeyType, class ValueType, class ListStore>
typename SortedListDict<KeyType, ValueType, ListStore>::iterator 
SortedListDict<KeyType, ValueType, ListStore>::findIterLSearchFromEnd(const KeyType & key) {
  for (iterator iter = sortedList.end(); iter != sortedList.begin(); ) {
    --iter;
    if (iter->first < key) {
      ++iter;
      return iter;
    }
  }
  return sortedList.begin();
}

template <class KeyType, class ValueType, class ListStore>
void SortedListDict<KeyType, ValueType, ListStore>::remove(const KeyType & key) {
  iterator iter = findIterLSearchFromEnd(key);

  if (iter != sortedList.end() && 
      iter->first == key)
    sortedList.erase(iter);
}

template <class KeyType, class ValueType, class ListStore>
void SortedListDict<KeyType, ValueType, ListStore>::insert(const KeyType & key, const ValueType & value) {
  iterator iter = findIterLSearchFromEnd(key);

  if (iter != sortedList.end() &&
      iter->first == key) {
    iter->second = value;
  }
  else {
    sortedList.insert(iter, std::pair<KeyType, ValueType>(key, value));
  }
}

template <class KeyType, class ValueType, class ListStore>
bool SortedListDict<KeyType, ValueType, ListStore>::contains(const KeyType & key) {
  iterator iter;
  try {
    iter = findIterBSearch(key);
  } catch (KeyNotFoundException ex) {
    return false;
  }
//  return iter != sortedList.end(); // 2011W2 comment: this code from 2010W2didn't work, added try/catch above and the stmt below.
  if(iter == sortedList.end()) return false;
  return iter->first == key;
}


template <class KeyType, class ValueType, class ListStore>
const ValueType & SortedListDict<KeyType, ValueType, ListStore>::find(const KeyType & key) throw(KeyNotFoundException) {
  iterator iter = findIterBSearch(key);
  
  if (iter == sortedList.end() ||
      iter->first != key) {
    throw KeyNotFoundException("Key Not Found");
  }
  
  return iter->second;
}

template <class KeyType, class ValueType, class ListStore>
void SortedListDict<KeyType, ValueType, ListStore>::makeEmpty() {
  sortedList.clear();
}


template <class KeyType, class ValueType, class ListStore>
void SortedListDict<KeyType, ValueType, ListStore>::printDict() {
  iterator iter;
    
  for (iter = sortedList.begin(); iter != sortedList.end(); iter++ ) {
    pair<KeyType, ValueType> pair = *iter;
    cout << pair.first << "::" << pair.second << " ,";
  }
    
  cout << endl;
}


// #include <list>
// int main(int argc, char *argv[]) {
//   Dictionary<int, int> *dictionary2;
//   dictionary2 = new SortedListDict<int, int, std::list< std::pair<int, int> > >;
    
//   SortedListDict<int, int, std::list< std::pair< int, int > > > * dictionary = dynamic_cast< SortedListDict<int, int, std::list< std::pair<int, int> > >* >(dictionary2);
    
//   dictionary->insert(1, 1);
//   dictionary->insert(2, 2);
//   dictionary->insert(3, 3);
//   dictionary->insert(4, 4);
//   dictionary->insert(5, 5);
    
//   dictionary->remove(4);
//   dictionary->remove(2);
//   dictionary->remove(1);
    
//   dictionary->printDict();
// }

#endif
