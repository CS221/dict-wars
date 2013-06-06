#ifndef QUADRATIC_PROBING_H_
#define QUADRATIC_PROBING_H_

#include <vector>
//        #include "mystring.h" // WOLF: rather use regular strings!

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an initialization for ITEM_NOT_FOUND
//               and an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Hashable find( x )     --> Return item that matches x
// void makeEmpty( )      --> Remove all items

template <class HashedObj>
class HashTable
{
public:
  explicit HashTable( const HashedObj & notFound, bool useQP = true, uint64_t size = 101 );
  HashTable( const HashTable & rhs )
    : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
      currentPrimeIndex( rhs.currentPrimeIndex ), array( rhs.array ), currentSize( rhs.currentSize ), QUADRATIC_PROBING(rhs.QUADRATIC_PROBING) { }
	  
  const HashedObj & find( const HashedObj & x ) const;

  void makeEmpty( );
  void insert( const HashedObj & x );
  void remove( const HashedObj & x );

  const HashTable & operator=( const HashTable & rhs );

  enum EntryType { ACTIVE, EMPTY, DELETED };
private:
  unsigned int nextPrime(int n);

  struct HashEntry
  {
    HashedObj element;
    EntryType info;

    HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
      : element( e ), info( i ) { }
  };
            
  const HashedObj ITEM_NOT_FOUND;
  int currentPrimeIndex;
  std::vector<HashEntry> array;
  uint64_t currentSize;

  bool isActive( uint64_t currentPos ) const;
  uint64_t findPos( const HashedObj & x ) const;
  void rehash( );
  uint64_t next_probe( const HashedObj & value, uint64_t slot, uint64_t prev_probe, uint64_t probe_count ) const;

  const bool QUADRATIC_PROBING;
};

uint64_t weiss_hash( const std::string & key, uint64_t tableSize );
uint64_t weiss_hash( uint64_t key, uint64_t tableSize );

#include "QuadraticProbing.cpp"
#endif
