#pragma warning (disable: 4786)

#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <list>

using namespace std;

int INFINITY = INT_MAX;

struct Vertex
{
    string           name;   // Vertex name
    vector<Vertex *> adj;    // Adjacent vertices
    int              dist;   // Cost
    Vertex          *path;   // Previous vertex on shortest path

    Vertex( const string & nm ) : name( nm )
      { reset( ); }

    void reset( )
      { dist = INFINITY; path = NULL; }
};


typedef map<string,Vertex *> vmap;
typedef pair<string,Vertex *> vpair;


class Graph
{
  public:
    Graph( ) { }
    ~Graph( );
    void addEdge( const string & sourceName, const string & destName );
    void printPath( const string & destName ) const;
    void unweighted( const string & startName );
      
  private:
    Vertex * getVertex( const string & vertexName );
    void printPath( const Vertex & dest ) const;
    void clearAll( );

    vmap vertexMap;
    vector<Vertex *> allVertices;
};



void Graph::addEdge( const string & sourceName, const string & destName )
{
    Vertex * v = getVertex( sourceName );
    Vertex * w = getVertex( destName );
    v->adj.push_back( w );
}

void Graph::printPath( const string & destName ) const
{
    vmap::const_iterator itr = vertexMap.find( destName );

    if( itr == vertexMap.end( ) )
    {
        std::cout << "Destination vertex not found" << std::endl;
        return;
    }

    const Vertex & w = *(*itr).second;
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
    vmap::iterator itr = vertexMap.find( vertexName );

    if( itr == vertexMap.end( ) )
    {
        Vertex *newv = new Vertex( vertexName );
        allVertices.push_back( newv );
        vertexMap.insert( vpair( vertexName, newv ) );
        return newv;
    }
    return (*itr).second;
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
    for( int i = 0; i < allVertices.size( ); i++ )
        allVertices[ i ]->reset( );
}

Graph::~Graph( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        delete allVertices[ i ];
}


void Graph::unweighted( const string & startName )
{
    clearAll( );

    vmap::iterator itr = vertexMap.find( startName );

    if( itr == vertexMap.end( ) )
    {
        std::cout << startName << " is not a vertex in this graph" << std::endl;
        return;
    }

    Vertex *start = (*itr).second;
    list<Vertex *> q;
    q.push_back( start ); start->dist = 0;

    while( !q.empty( ) )
    {
        Vertex *v = q.front( );	q.pop_front( );

        for( int i = 0; i < v->adj.size( ); i++ )
        {
            Vertex *w = v->adj[ i ];
            if( w->dist == INFINITY )
            {
                w->dist = v->dist + 1;
                w->path = v;
                q.push_back( w );
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

        std::stringstream st( oneLine ); 
        st >> source;
        st >> dest;
        g.addEdge( source, dest );
    }

    std::cout << "File read" << std::endl;
    while( processRequest( std::cin, g ) )
        ;

    return 0;
}

