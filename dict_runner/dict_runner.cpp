#include <locale>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/times.h>
#include <sys/stat.h> 
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
//#include <cstdint>
#include "Dictionary.h"
#include "SplayTree.h"
#include "BinarySearchTree.h"
#include "HashTableQP.h"
#include "HashTableChaining.h"
#include "AvlTree.h"
#include "SortedListDict.h"
#include "UnsortedListDict.h"
//#include "RedBlackTree.h"
#include "UsageText.h"
#include <vector>
#include "Timer.h"
#include "BinaryHeapFakeDictionary.h"
//#include "Trie.cpp" // I don't want separate compilation, for simplicity.
#include <chrono>
#include <cassert>

#ifndef RSEED
#define RSEED (srandom(time(NULL)))
#endif

using namespace std;

typedef Dictionary<uint64_t, uint64_t> Dict;
map<string, Dict*> dictionaries;
map<string, string> dictionary_descriptions;
const uint64_t GRANULARITY = 500;


void printUsage(char *name) {
  cout << name << " <input> <timePer> <baseOutputDir> [";
  
  bool first = true;

  for (map<string,Dict*>::iterator i = dictionaries.begin(); i != dictionaries.end(); i++) {
    if (!first)
      cout << "|";
    first = false;
    cout << "-" << i->first;
  }

  cout << "]*" << endl;

#ifdef USERID
  cout << mystery_help_text << endl;
#endif

  cout << "<input>   Either '-' to read from standard input or a filename containing commands." << endl;
  cout << "<timePer> Rough limit on CPU seconds for each implementation's run" << endl;
  cout << "<baseDir> Base directory to dump output (one file per flag w/flag as filename)." << endl;
#ifndef USERID
  for (map<string,string>::iterator i = dictionary_descriptions.begin(); i != dictionary_descriptions.end(); i++) {
    cout << "  -" << i->first << "\tUse " << i->second << " dictionary/set implementation." << endl;
  }
#else
  for(unsigned int i = 1; i <= dictionaries.size(); i++) {
    printf("  -mys%02d\tUses the %d%s mystery dictionary/set implementation.\n", i, i,
               (i==1)?"st":(i==2)?"nd":(i==3)?"rd":"th");
  }
#endif
/*
  //  cout << "-avl      Use AVL Tree dictionary implementation." << endl;
  //  cout << "-avlT     Use AVL Tree dictionary implementation with tombstones for deletion." << endl;
  cout << "-bst      Use Binary Search Tree dictionary implementation." << endl;
  cout << "-bstT     Use Binary Search Tree dictionary implementation with tombstones for delection." << endl;
  //cout << "-hlp      Use Linear Probing Hashtable dictionary implementation." << endl;
  cout << "-hqp      Use Quadratic Probing Hashtable dictionary implementation." << endl;
  cout << "-sl       Use Sorted List (array) dictionary implementation." << endl;
  //  cout << "-sll      Use Sorted List (linked list) dictionary implementation." << endl;
  //UNIMPLEMENTED//cout << "-rbt     Use Red-Black Tree dictionary implementation." << endl;
  cout << "-spt      Use Splay Tree dictionary implementation." << endl;
  cout << "-usl      Use Unsorted List dictionary implementation." << endl;
  //  cout << "-mtf      Use USL dictionary implementation that moves recently accessed items to the front." << endl;
  cout << "-trie     Use Trie (branching over the digits 0-9) dictionary implementation." << endl;
*/
  cout << endl;
  cout << "So, for example, you could run implementations X and Y from standard" << endl;
  cout << "input for up to 10 CPU seconds each, dumping the results to the data" << endl;
  cout << "directory using: (where X and Y are a dictionary listed above)" << endl;
  cout << endl;
  cout << "  " << name << " - 10 data -X -Y" << endl;
  cout << endl;

  cout << help_text << endl;
}

struct Cmd {
  Cmd(int t, uint64_t v) : type(t), value(v), result(0) { }
  int type; // 0 = INSERT, 1 = REMOVE, 2 = FIND
  uint64_t value;
  uint64_t result;
};

void outputCommands(vector<Cmd> & commands) {
  for (vector<Cmd>::iterator i = commands.begin(); i != commands.end(); ++i) {
    if (i->type == 0) {
      cout << "INSERT " << i->value << ": " 
	   << (i->result ? "succeeded" : "FAILED") << endl;
    }
    else if (i->type == 1) {
      cout << "REMOVE " << i->value << ": " 
	   << (i->result ? "succeeded" : "FAILED") << endl;
    }
    else {
      cout << "FIND " << i->value << ": ";
      if (i->result) {
	cout << "succeeded with " << i->result << endl;
      }
      else {
	cout << "FAILED" << endl;
      }
    }
  }
}

void inputCommands(istream & in, vector<Cmd> & commands) {
  char cmd;
  while (in >> cmd) {
    uint64_t key;
    in >> key;
    char eolChar;
    if (in.get(eolChar) == 0 || 
	(eolChar == '\r' && in.get(eolChar) == 0) ||  // DOS-style EOL.
	eolChar != '\n') {
      cerr << "Input is malformed at command #" << (commands.size()+1) << "." 
	   << endl;
      cerr << "After the command type and number, the input did not " << endl
	   << "have an end-of-line character." << endl;
      cerr << "Ignoring remainder of line." << endl;
      while (in.get(eolChar) != 0 && eolChar != '\n') {
	// nothing to do
      }
    }
    if (cmd == 'I') {
      commands.push_back(Cmd(0, key));
    } else if (cmd == 'F') {
      commands.push_back(Cmd(2, key));
    } else if (cmd == 'R') {
      commands.push_back(Cmd(1, key));
    } else {
      cerr << "Invalid command." << endl;
    }
  }
}

class CommandRunner {
protected:
  Dict & dict;
private: // dict/val decls ordered to avoid -Wreorder warning
  uint64_t val;
protected:
  virtual void insert(uint64_t key, uint64_t value) {
    dict.insert(key, value);
  }
  virtual void remove(uint64_t key) {
    dict.remove(key);
  }
  virtual bool find(uint64_t key) {
    return dict.find(key);
  }
public:
  CommandRunner(Dict & _dict) 
    : dict(_dict), val(1) {
    
  }

  void reset() {
    val = 1;
    dict.makeEmpty();
  }
  
  void run(Cmd & cmd) {
    switch (cmd.type) {
    case 0: insert(cmd.value, val++); cmd.result = 1; break;
    case 1: 
      try {
	remove(cmd.value);
	cmd.result = 1;
      } catch (KeyNotFoundException e) {
	cmd.result = 0;
      }
      break;
    case 2:
      try {
	cmd.result = find(cmd.value);
      } catch (KeyNotFoundException e) {
	cmd.result = 0;
      }
      break;
    }
  }
};

class CheckedCommandRunner : public CommandRunner {
protected:
  void insert(uint64_t key, uint64_t value) {
    dict.checked_insert(key, value);
  }
  void remove(uint64_t key) {
    dict.checked_remove(key);
  }
  bool find(uint64_t key) {
    return dict.find(key);
  }
};



int main(int argc, char *argv[]) {
  // dictionaries["bst"] = new DBinarySearchTree(false);
  // dictionary_descriptions["bstT"] = "Binary search tree (without tombstones)";
  dictionaries["bstT"] = new DBinarySearchTree(true);
  dictionary_descriptions["bstT"] = "Binary search tree with tombstones";
  //dictionaries["trie"] = new Trie; // not yet implemented; just not linked in?
  //dictionary_descriptions["trie"] = "Trie (a tree with one branch for each symbol in the key alphabet)";
  //dictionaries["rbt"] = new DRedBlackTree; // not yet implemented; just not linked in?
  //dictionary_descriptions["rbt"] = "Red-black Tree, a self-balancing binary search tree";
  dictionaries["spt"] = new DSplayTree;
  dictionary_descriptions["spt"] = "Splay tree";
  dictionaries["avl"] = new DAvlTree(false);
  dictionary_descriptions["avl"] = "AVL tree";
  dictionaries["avlT"] = new DAvlTree(true);
  dictionary_descriptions["avlT"] = "AVL tree with tombstones";
  dictionaries["hch"] = new DHashTableChaining();
  dictionary_descriptions["hch"] = "Hashtable with chaining (fixed size 10000)";
  // dictionaries["hlp"] = new DHashTableQP(false);
  // dictionary_descriptions["hlp"] = "Hashtable with linear probing";
  dictionaries["hqp"] = new DHashTableQP;
  dictionary_descriptions["hqp"] = "Hashtable with quadratic probing";
  // dictionaries["usl"] = new UnsortedListDict<uint64_t, uint64_t>;
  // dictionary_descriptions["usl"] = "Unsorted linked list";
  // dictionaries["uslOOI"] = new UnsortedListDict<uint64_t, uint64_t>(OVERWRITE_VIA_REPLACE_ON_INSERT);
  // dictionary_descriptions["uslOOI"] = "Unsorted linked list that overwrites old values on duplicate insertions via replacing their values on insert";
  dictionaries["uslORA"] = new UnsortedListDict<uint64_t, uint64_t>(OVERWRITE_VIA_REMOVE_ALL);
  dictionary_descriptions["uslORA"] = "Unsorted linked list that overwrites old values on duplicate insertions via deleting redundant copies on remove";
  // dictionaries["mtf"] = new UnsortedListDict<uint64_t, uint64_t>(ALLOW_MULTIPLE, true);
  // dictionary_descriptions["mtf"] = "Unsorted linked list that moves accessed elts to the front of the list";
  // dictionaries["mtfOOI"] = new UnsortedListDict<uint64_t, uint64_t>(OVERWRITE_VIA_REPLACE_ON_INSERT, true);
  // dictionary_descriptions["mtfOOI"] = "Unsorted linked list that moves accessed elts to the front of the list and overwrites old values on duplicate insertions via replacing their values on insert";
  // dictionaries["mtfORA"] = new UnsortedListDict<uint64_t, uint64_t>(OVERWRITE_VIA_REMOVE_ALL, true);
  // dictionary_descriptions["mtfORA"] = "Unsorted linked list that moves accessed elts to the front of the list and overwrites old values on duplicate insertions via deleting redundant copies on remove";
  // dictionaries["sl"] = new SortedListDict<uint64_t, uint64_t>;
  // dictionary_descriptions["sl"] = "Sorted std::vector";
  // dictionaries["heap"] = new BinaryHeapFakeDictionary;
  // dictionary_descriptions["heap"] = "NOT A DICTIONARY: a heap; find/remove ignore keys and perform findmin/deletemin";

  // ensure there's as many descriptions as dictionaries
  assert(dictionary_descriptions.size() == dictionaries.size());

#ifdef USERID
  {
    // Calculate a random seed based on the students' user ID.  The ID
    // should be given with -DUSERID='"xxxx"' as a compiler flag.
    // Note that the USERID must be in quotation marks (to come out as
    // a string).  Give no ID to get a non-randomized version.
    string salt = "salt";
    string str = salt + USERID;
    const char * cstr = str.c_str();
    locale loc;
    const collate<char> & coll = use_facet<collate<char> >(loc);

    // Seed the random number generator with this (will always produce
    // the same sequence of random values for a given student ID).
    srandom(coll.hash(cstr, cstr + str.length()));
    
    // Randomize the mapping of the dictionaries.  Should really use
    // the built-in permuter in the STL, but I can't get the thing to
    // work.
    string permutation[dictionaries.size()];
    int count = 0;
    for (map<string,Dict*>::iterator i = dictionaries.begin(); i != dictionaries.end(); i++)
      permutation[count++] = i->first;

    for (unsigned int i = dictionaries.size() - 1; i > 0; i--) {
      string temp = permutation[i];
      int swapI = random() % (i + 1);
      permutation[i] = permutation[swapI];
      permutation[swapI] = temp;
    }
    
    // Give new names to the dictionaries.
    map<string, Dict*> newDictionaries;
    for (unsigned int i = 0; i < dictionaries.size(); i++) {
      char newName[20];
      sprintf(newName, "mys%02u", i+1);
      newDictionaries[newName] = dictionaries[permutation[i]];

      // If INTERROGATE is defined (use -DINTERROGATE), then prints
      // out the mapping (e.g., for grading).
#ifdef INTERROGATE
      cout << newName << "->" << permutation[i] << endl;
#endif
    }
    dictionaries = newDictionaries;
  }
#endif
    

  if (argc < 4 || atoi(argv[2]) == 0) {
    printUsage(argv[0]);
    exit(0);
  }

  int arg = 1;

  // Grab the basic command-line arguments:
  string inputFile = argv[arg++];
  std::chrono::seconds targetSecs(atoi(argv[arg++]));
  string baseDir = argv[arg++]; // Should test that baseDir exists??  

  // Check if baseDir exists, if not create it:
  struct stat st;
  if(stat(baseDir.c_str(),&st) != 0) system(("mkdir " + baseDir).c_str()); 

  // Read the dictionary commands.
  vector<Cmd> commands;
  if (inputFile == "-") {
    cout << "Inputting commands from stdin." << endl;
    inputCommands(cin, commands);
    cout << "Done inputting commands from stdin." << endl;
  }
  else {
    cout << "Inputting commands from file: " << inputFile << endl;
    ifstream in(inputFile.c_str());
    inputCommands(in, commands);
    cout << "Done inputting commands from file: " << inputFile << endl;
    in.close();
  }

  Timer timer; 

  // Process the dictionaries requested for runs.
  for ( ; arg < argc; arg++) {
    char * argStr = argv[arg];
    string label;
    if (argStr[0] == '-')
      label = argStr+1; // strip the dash for output.
    else {
      cerr << "Flag \"" << argStr << "\" is missing initial dash and I don't recognize it." << endl;
      if (dictionaries.find(argStr) != dictionaries.end()) {
	cerr << "Perhaps you meant -" << argStr << "?" << endl;
	cerr << "Trying that and continuing." << endl;
	label = argStr;
      }
      else {
	printUsage(*argv);
	continue;
      }
    }
    
    map<string, Dict*>::iterator dictCell = dictionaries.find(label);
    if (dictCell == dictionaries.end()) {
      cerr << "Dictionary type \"" << argStr << "\" unknown; ignoring." << endl;
      printUsage(*argv);
      continue;
    }
    
    Dict *dictionary = dictCell->second;
    dictionary->makeEmpty();

    string filename = baseDir + "/" + label;
    cout << "Outputting to " << filename << "." << endl;
    ofstream out(filename.c_str());

    uint64_t count = 0L;  
    CommandRunner cmdRunner(*dictionary);
    // NOTE: can use CheckedCommandRunner to get semantics that every
    // dictionary ignores duplicates that are inserted and ignores
    // removal on non-existant items (basically, a contains call
    // before every insert/remove).
    auto end = commands.end();

    // Fake first timestamp.
    Timer::duration timeSoFar(0);
    out << count << " " << timeSoFar.count() << endl;
    timer.reset();
    for (auto i = commands.begin(); i != end && timeSoFar < targetSecs; ++i) {
      count++;
      cmdRunner.run(*i);
      if (count % GRANULARITY == 0L) {
	timeSoFar = timer.timeSoFar();
	out << count << " " << timeSoFar.count() << endl;
      }
    }

    // Guaranteed final timestamp.
    if (count % GRANULARITY != 0L) 
      out << count << " " << timer.timeSoFar().count() << endl;

    //delete dictionary;  // do this below to ALL allocated dictionaries.
    dictionary = NULL;

    out.close();
  }

  for (auto it = dictionaries.begin(); it != dictionaries.end(); ++it)
    delete it->second;

  return 0;
}
