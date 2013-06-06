#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
  const int LIMIT = 5;
  // should be quick
  for (int i = 1; i < LIMIT; i++)
    cout << "I " << i << endl;

  // should be quick
  for (int i = LIMIT; i > 0; i--)
    cout << "R " << i << endl;

  // should be quick
  for (int i = 1; i < LIMIT; i++)
    cout << "I " << i << endl;

  // should be slow!
  for (int i = 1; i < LIMIT; i++)
    cout << "R " << i << endl;
}
