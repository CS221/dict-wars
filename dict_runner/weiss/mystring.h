#ifndef MY_STRING_H_
#define MY_STRING_H_

#include <iostream>


/*
 * The next line is used because Codewarrior has a conflict with
 * the STL string. Make sure to put the #include of this file
 * AFTER all the system includes.
 */


class StringIndexOutOfBounds { };

class String
{
  public:
    String( const char *cstring = "" );               // Constructor
    String( const String & str );                     // Copy constructor
    ~String( )                                        // Destructor
      { delete [ ] buffer; }

    const String & operator= ( const String & rhs );  // Copy
    const String & operator+=( const String & rhs );  // Append

    const char *c_str( ) const        // Return C-style string
      { return buffer; }
    int length( ) const               // Return string length
      { return strLength; }

    char   operator[]( int k ) const; // Accessor operator[]
    char & operator[]( int k );       // Mutator  operator[]

    enum { MAX_LENGTH = 1024 };  // Maximum length for input string

  private:
    char *buffer;                  // storage for characters
    int strLength;                 // length of string (# of characters)
    int bufferLength;              // capacity of buffer
};

std::ostream & operator<< ( std::ostream & out, const String & str );    // Output
std::istream & operator>> ( std::istream & in, String & str );           // Input
std::istream & getline( std::istream & in, String & str );              // Read line

bool operator==( const String & lhs, const String & rhs );    // Compare ==
bool operator!=( const String & lhs, const String & rhs );    // Compare !=
bool operator< ( const String & lhs, const String & rhs );    // Compare <
bool operator<=( const String & lhs, const String & rhs );    // Compare <=
bool operator> ( const String & lhs, const String & rhs );    // Compare >
bool operator>=( const String & lhs, const String & rhs );    // Compare >=

#endif
