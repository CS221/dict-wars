#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

#include "BinaryHeapFakeDictionary.h"

using namespace std;

bool isHeapEmpty(BinaryHeapFakeDictionary & heap) {
  // Should give exception on empty heap.
  try {
    // Removes minimum always.
    heap.find(1);
    return false;
  }
  catch (KeyNotFoundException e) {
    return true;
  }
}

bool testCompare(bheap_entry_t l, bheap_entry_t r, bool expected_result) {

  cout << "<" << l.first << "," << l.second << "> bh> <" 
       << r.first << "," << r.second 
       << "> should be " <<  expected_result << " and is: " ;
  bool result = bheapEntryCompare(l, r);
  cout << result << endl;
  return result == expected_result;
}

int main(int argc, char * argv[]) {
  BinaryHeapFakeDictionary heap;

  // Test the comparator.
  cout << "Testing bheapEntryCompare (hereafter bh>)." << endl;

  assert(testCompare(make_pair(1L, 1L),
		     make_pair(1L, 1L),
		     false));

  assert(testCompare(make_pair(1L, 1L),
		     make_pair(2L, 2L),
		     false));

  assert(testCompare(make_pair(5L, 5L),
		     make_pair(2L, 2L),
		     true));

  assert(testCompare(make_pair(1L, 2L),
		     make_pair(1L, 1L),
		     false));

  assert(testCompare(make_pair(1L, 1L),
		     make_pair(1L, 2L),
		     false));

  assert(testCompare(make_pair(1L, 2L),
		     make_pair(2L, 1L),
		     false));

  assert(testCompare(make_pair(5L, 2L),
		     make_pair(2L, 5L),
		     true));

  uint64_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
  vector<uint64_t> v(data, data+30);

  // obtain a time-based seed:
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  shuffle(v.begin(), v.end(), default_random_engine(seed));

  // Always returns true.
  cout << "Testing the (stubbed) contains method on an empty heap." << endl;
  assert(heap.contains(1));

  cout << "Testing that find throws an error on an empty heap." << endl;
  assert(isHeapEmpty(heap));

  cout << "Inserting 40." << endl;
  heap.insert(40, 40);

  cout << "Testing that find does not throw an error on a non-empty heap." << endl;
  assert(!isHeapEmpty(heap));

  // Always returns minimum.
  cout << "Testing that find gives back 40; it gives back: " << heap.find(1000) << endl;
  assert(heap.find(1000) == 40);

  // Removes minimum always.
  cout << "Removing the 40." << endl;
  heap.remove(1);

  cout << "Testing that find throws an error on an empty heap." << endl;
  assert(isHeapEmpty(heap));

  // Insert 1-30 randomly permuted.
  cout << "Inserting 1-30 randomly permuted." << endl;
  for (uint64_t i : v)
    heap.insert(i, i);

  cout << "Testing that the heap is now non-empty." << endl;
  assert(!isHeapEmpty(heap));

  cout << "Removing the 30 elements." << endl;
  int val = 1;
  for (uint64_t i : v) {
    cout << "Checking (before removal) that the " << val << "th elt removed is correct; " 
	 << "should be " << val << " and is " << heap.find(1) << endl;
    assert(heap.find(1) == val);
    heap.remove(1);
    val++;
  }

  cout << "Testing that the heap is now empty." << endl; 
  assert(isHeapEmpty(heap));

  // Insert 1-30 randomly permuted.
  cout << "Reinserting 1-30 (in the same order)." << endl;
  for (uint64_t i : v)
    heap.insert(i, i);

  cout << "Testing that the heap is now non-empty." << endl;
  assert(!isHeapEmpty(heap));

  cout << "Calling makeEmpty." << endl;
  heap.makeEmpty();

  cout << "Testing that the heap is now empty." << endl; 
  assert(isHeapEmpty(heap));
}
