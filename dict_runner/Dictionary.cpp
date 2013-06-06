#include "Dictionary.h"

/*

	// Concrete method. Checks containment before removing. If not in 
	// the dictionary, it does nothing.
	void checked_remove(const KeyType & key);

	// Concrete method; checks containment of key; if an entry with
	// key key already exists, do nothing. If not, calls insert().
	void checked_insert(const KeyType & key, const ValueType & value);
*/


// see header
template <class KeyType, class ValueType> 
void Dictionary<KeyType,ValueType>::checked_remove(const KeyType & key)
{
	if(Dictionary<KeyType,ValueType>::contains(key))
		Dictionary<KeyType,ValueType>::remove(key);
}

// see header
template <class KeyType, class ValueType>
void Dictionary<KeyType,ValueType>::checked_insert(const KeyType & key, const ValueType 
& value)
{
	if(!Dictionary<KeyType,ValueType>::contains(key))
		Dictionary<KeyType,ValueType>::insert(key, value);
}


