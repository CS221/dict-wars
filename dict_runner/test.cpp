#include <iostream>
#include "weiss/QuadraticProbing.h"

using namespace std;

int main(int argc, char * argv[]) {
  char cmd;
  HashTable<uint64_t> table(0, false);
  while (cin >> cmd) {
    uint64_t key;
    if (!(cin >> key)) {
      return -1;
    }
    bool before;
    switch (cmd) {
    case 'i':
    case 'I': 
      before = (table.find(key) != 0); 
      table.insert(key); 
      std::cout << "Inserting: " << key << ".  Before/after? " << before << "/" << (table.find(key) != 0) << endl; break;
      
    case 'f':
    case 'F': std::cout << "Found for  " << key << "\t" << table.find(key) << endl; break;
      
    case 'r':
    case 'R': before = (table.find(key) != 0); table.remove(key); std::cout << "Removing:  " << key << ".  Before/after? " << before << "/" << (table.find(key) != 0) << endl; break;
      
    default: std::cout << "Command " << cmd << " not understood.  Try I (insert), F (find), or R (remove)." << endl;
    }
  }

  return 0;
}
