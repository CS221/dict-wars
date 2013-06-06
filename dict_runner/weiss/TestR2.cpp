
        #include "Random.h"
        #include <iostream>

            // Test program
        int main( )
        {
            Random r( 1 );

            for( int i = 0; i < 20; i++ )
                std::cout << r.random0_1( ) << std::endl;

            return 0;
        }
