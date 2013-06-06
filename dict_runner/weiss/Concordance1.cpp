#pragma warning (disable: 4786)

#include <iostream>
#include <fstream>
#ifdef __GNUC__
    #include <sstream>
#else
#ifdef __BORLANDC__
    #include <strstrea.h>
#else
    #include <sstream>
#endif
#endif
#include <map>
#include <string>
#include <vector>

using namespace std;

std::ostream & operator<<( std::ostream & out, const pair<string, vector<int> > & rhs )
{
    out << rhs.first << ": " << '\t' << rhs.second[ 0 ];
    for( int i = 1; i < rhs.second.size( ); i++ )
        out << ", " << rhs.second[ i ];
    return out;
}

int main( int argc, char *argv[ ] )
{
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[ 0 ] << " filename" << std::endl;
        return 1;
    }

    std::ifstream inFile( argv[ 1 ] );
    if( !inFile )
    {
        std::cerr << "Cannot open " << argv[ 1 ] << std::endl;
        return 1;
    }

    string                              oneLine;
    map<string, vector<int> >           concordance;

        // Read the words; add them to wordMap
    for( int lineNum = 1; getline( inFile, oneLine ); lineNum++ )
    {
#if defined( __GNUC__ ) || defined ( __BORLANDC__ )
        std::stringstream st( oneLine.c_str( ), oneLine.length( ) );
#else
        istringstream st( oneLine );
#endif
        string word;

        while( st >> word )
            concordance[ word ].push_back( lineNum );
    }

    map<string, vector<int> >::iterator itr;
    for( itr = concordance.begin( ); itr != concordance.end( ); itr++ )
        std::cout << *itr << std::endl;

    return 0;
}

