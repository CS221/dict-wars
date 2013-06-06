#include <iostream>

using namespace std;

// Trying to test the various semantics.  Options are:
// - allow multiple
// - overwrite on insert
// - remove all on delete (functionally: overwrite on insert)

// Also trying to test move to front vs. non-move-to-front variants

int main(int argc, char * argv[]) {
  const int LIMIT = 5000;

  // Dupe seeding.  This will be very fast for all, but memory
  // allocation issues may make it a bit faster for the
  // non-move-to-front overwrite on insert (since it allocates no new
  // nodes).  (The MTF version removes the old node and allocs a new
  // one.)
  for (int i = 1; i <= LIMIT; i++)
    cout << "I 1" << endl;

  // Length testing.
  // This will be:
  // - very fast on overwrite on insert
  // - very slow otherwise
  for (int i = 1; i <= LIMIT; i++)
    cout << "F 2" << endl;

  // Dupe removal.  
  // This will be:
  // - very slow ON FIRST OPERATION for remove all on delete
  // - very fast otherwise
  for (int i = 1; i <= LIMIT; i++)
    cout << "R 1" << endl;

  // Now, to test MTF.
  // Insert 1 .. n
  // Find 1 repeatedly.
  // The final finds will be:
  // - very slow ON FIRST OPERATION for MTF
  // - very slow ON ALL OPERATIONS for non-MTF
  for (int i = 1; i <= LIMIT; i++)
    cout << "I " << i << endl;
  for (int i = 1; i <= LIMIT; i++)
    cout << "F 1" << endl;
}
