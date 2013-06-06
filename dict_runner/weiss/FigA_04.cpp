        #include <iostream>
        #include <vector>
        #include <algorithm>
        using namespace std;

        int main( )
        {
            vector<int> v;    // Initial size is 0
            int x;

            while( std::cin >> x )
                v.push_back( x );

            sort( v.begin( ), v.end( ) );

            for( int i = 0; i < v.size( ); i++ )
                std::cout << v[ i ] << std::endl;
            return 0;
        }
