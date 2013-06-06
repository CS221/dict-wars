// The splay tree is having a double-free problem.  This is to show
// and resolve it.  FIXED!  It was due to the convenience mod of
// nullNode to match the target element in SplayTree::splay.
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
  const int LIMIT = 1;

  // Repro'd on this and FIXED! 
  for (int i = 1; i <= LIMIT; i++)
    cout << "R 1" << endl;


  // Original version:
  const int LIMIT = 5000;

  for (int i = 1; i <= LIMIT; i++)
    cout << "I 1" << endl;
  for (int i = 1; i <= LIMIT; i++)
    cout << "F 2" << endl;
  for (int i = 1; i <= LIMIT; i++)
    cout << "R 1" << endl;
  for (int i = 1; i <= LIMIT; i++)
    cout << "I " << i << endl;
  for (int i = 1; i <= LIMIT; i++)
    cout << "F 1" << endl;
}
