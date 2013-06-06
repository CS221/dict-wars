        #include "Random.h"
        #include <iostream>

            // Test program
        int main( )
        {
            Random r( 1 );

            for( int i = 0; i < 20; i++ )
                std::cout << r.randomInt( ) << std::endl;

            return 0;
        }
