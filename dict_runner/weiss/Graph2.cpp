#include <iostream>
#include <fstream>
#include <climits>
#ifdef unix
    #include <sstream>
#else
    #include <strstrea.h>   // <strstream.h> on UNIX machines
#endif

#include "mystring.h"
#include "SeparateChaining.h"
#include "LinkedList.h"
#include "QueueAr.h"

int INFINITY = INT_MAX;

struct Vertex
{
    string         name;   // Vertex name
    List<Vertex *> adj;    // Adjacent vertices
    int            dist;   // Cost
    Vertex        *path;   // Previous vertex on shortest path

    Vertex( const string & nm ) : name( nm )
      { reset( ); }

    void reset( )
      { dist = INFINITY; path = NULL; }
};

struct MapEntry
{
    string  vertexName;
    Vertex *storedVertex;

    MapEntry( const string & name = "", Vertex * v = NULL )
      : vertexName( name ), storedVertex( v ) { }
    bool operator!=( const MapEntry & rhs ) const
      { return vertexName != rhs.vertexName; }
    bool operator==( const MapEntry & rhs ) const
      { return vertexName == rhs.vertexName; }
};

int hash( const MapEntry & x, int tableSize )
{
    return hash( x.vertexName, tableSize );
}

class Graph
{
  public:
    Graph( ) : vertexMap( MapEntry( ) ), numVertices( 0 )  { }
    ~Graph( );
    void addEdge( const string & sourceName, const string & destName );
    void printPath( const string & destName ) const;
    void unweighted( const string & startName );
      
  private:
    Vertex * getVertex( const string & vertexName );
    void printPath( const Vertex & dest ) const;
    void clearAll( );

    HashTable<MapEntry> vertexMap;
    List<Vertex *> allVertices;
    int numVertices;
    const MapEntry ITEM_NOT_FOUND;
};



void Graph::addEdge( const string & sourceName, const string & destName )
{
    Vertex * v = getVertex( sourceName );
    Vertex * w = getVertex( destName );
    v->adj.insert( w, v->adj.zeroth( ) );
}

void Graph::printPath( const string & destName ) const
{
    const MapEntry & match = vertexMap.find( MapEntry( destName ) );
    if( match == ITEM_NOT_FOUND )
    {
        std::cout << "Destination vertex not found" << std::endl;
        return;
    }

    const Vertex & w = *match.storedVertex;
    if( w.dist == INFINITY )
        std::cout << destName << " is unreachable";
    else
        printPath( w );
    std::cout << std::endl;
}

// If vertexName is not present, add it to vertexMap
// In either case, return the Vertex
Vertex * Graph::getVertex( const string & vertexName )
{
    static MapEntry entry;
    entry.vertexName = vertexName;
    
    const MapEntry & match = vertexMap.find( entry );
    if( match == ITEM_NOT_FOUND )
    {
        entry.storedVertex = new Vertex( vertexName );
        allVertices.insert( entry.storedVertex, allVertices.zeroth( ) );
        numVertices++;
        vertexMap.insert( entry );
        return entry.storedVertex;
    }
    return match.storedVertex;
}

void Graph::printPath( const Vertex & dest ) const
{
    if( dest.path != NULL )
    {
        printPath( *dest.path );
        std::cout << " to ";
    }
    std::cout << dest.name;
}

void Graph::clearAll( )
{
    ListItr<Vertex *> itr;
    for( itr = allVertices.first( ); !itr.isPastEnd( ); itr.advance( ) )
        itr.retrieve( )->reset( );
}

Graph::~Graph( )
{
    ListItr<Vertex *> itr;
    for( itr = allVertices.first( ); !itr.isPastEnd( ); itr.advance( ) )
        delete itr.retrieve( );
}


void Graph::unweighted( const string & startName )
{
    clearAll( );

    const MapEntry & match = vertexMap.find( MapEntry( startName ) );
    if( match == ITEM_NOT_FOUND )
    {
        std::cout << startName << " is not a vertex in this graph" << std::endl;
        return;
    }

    Vertex *start = match.storedVertex;
    Queue<Vertex *> q( numVertices );
    q.enqueue( start ); start->dist = 0;

    while( !q.isEmpty( ) )
    {
        Vertex *v = q.dequeue( );

        ListItr<Vertex *> itr;
        for( itr = v->adj.first( ); !itr.isPastEnd( ); itr.advance( ) )
        {
            Vertex *w = itr.retrieve( );
            if( w->dist == INFINITY )
            {
                w->dist = v->dist + 1;
                w->path = v;
                q.enqueue( w );
            }
        }
    }
}

/**
 * Process a request; return false if end of file.
 */
bool processRequest( std::istream & in, Graph & g )
{
    string startName;
    string destName;

    std::cout << "Enter start node: ";
    if( !( in >> startName ) )
        return false;
    std::cout << "Enter destination node: ";
    if( !( in >> destName ) )
        return false;

    g.unweighted( startName );
    g.printPath( destName );

    return true;
}


/**
 * A simple main that reads the file given by argv[1]
 * and then calls processRequest to compute shortest paths.
 * Skimpy error checking in order to concentrate on the basics.
 */
int main( int argc, char *argv[ ] )
{
    Graph g;

    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[ 0 ] << " graphfile" << std::endl;
        return 1;
    }

    std::ifstream inFile( argv[ 1 ] );
    if( !inFile )
    {
        std::cerr << "Cannot open " << argv[ 1 ] << std::endl;
        return 1;
    }

    string oneLine;

        // Read the words; add them to wordMap
    while( getline( inFile, oneLine ) )
    {
        string source, dest;

        std::stringstream st( (char *) oneLine.c_str( ) );   // Deprecated form of string streams
        st >> source;
        st >> dest;
        g.addEdge( source, dest );
    }

    std::cout << "File read" << std::endl;
    while( processRequest( std::cin, g ) )
        ;

    return 0;
}

