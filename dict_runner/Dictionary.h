// NOTE: modified abstract data type to simplify for PP#3 CPSC 221 2010W2.

//
// This file declares an abstract Dictionary class which maps (templated)
// KeyType objects to ValueType objects.
//
// This requires a default constructor from ValueType and (unless NDEBUG is
// defined) operator << from both KeyType and ValueType. KeyTypes must at least
// support == and !=. Many subtypes will require more.  Almost any concrete
// subtype will also require a copy constructor, assignment operator, and/or
// default constructor from KeyType and ValueType.
//

#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <stdexcept>

using namespace std;

// Forward references
class KeyNotFoundException;
template <class KeyType, class ValueType> class Dictionary;
template <class KeyType, class ValueType> class DictionaryIterator;

class KeyNotFoundException : public std::runtime_error {
public:
	KeyNotFoundException(const string & message)
		: std::runtime_error(message) { };
};

template <class KeyType, class ValueType>
class Dictionary {
public:
	// Virtual destructor required since there are virtual methods.
	virtual ~Dictionary() {};

	// Insert the data "value" indexed by "key" into the table. 
	// Behaviour is undefined if the key is already in the dictionary.
	virtual void insert(const KeyType & key, const ValueType & value) = 0;

	// Remove the entry from the table indexed by "key".
	// Behaviour is undefined if the key is not already in the
	// dictionary.
	virtual void remove(const KeyType & key) = 0;

	// Checks containment before removing. If not in 
	// the dictionary, it does nothing.
	inline void checked_remove(const KeyType & key) {
	        if(this->contains(key))
        	        this->remove(key);
	}

	// Checks containment of key; if an entry with
	// key key already exists, do nothing. If not, calls insert().
	inline void checked_insert(const KeyType & key, const ValueType & value) {
	        if(!this->contains(key))
	                this->insert(key, value);
	}

	// Return true if key is in the dictionary.
	virtual bool contains(const KeyType & key) = 0;

	// Return the value indexed by the given key. Behaviour is
	// undefined if the key is not in the dictionary.
	virtual const ValueType & find(const KeyType & key)
		throw(KeyNotFoundException) = 0;

	// Remove all entries from the dictionary (make the size 0). PURE
	// VIRTUAL.
	virtual void makeEmpty() = 0;
};

// An iterator class for looking through (but not changing) a Dictionary.
template <class KeyType, class ValueType>
class DictionaryIterator {
public:
	virtual ~DictionaryIterator() { }

	// Get the current key. PURE VIRTUAL.
	virtual const KeyType & getKey() const = 0;

	// Get the current value. PURE VIRTUAL.
	virtual const ValueType & getValue() const = 0;

	// Returns true if the iterator has already past the last valid
	// entry in the dictionary. PURE VIRTUAL.
	virtual bool pastEnd() const = 0;

	// Advances the iterator to the next entry. PURE VIRTUAL.
	virtual void next() = 0;
};

#endif
