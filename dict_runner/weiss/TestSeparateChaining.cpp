        #include <iostream>
        #include "SeparateChaining.h"

            // Simple main
        int main( )
        {
            int ITEM_NOT_FOUND = -9999;
            HashTable<int> H( ITEM_NOT_FOUND );

            const int NUMS = 4000;
            const int GAP  =   37;
            int i;

            std::cout << "Checking... (no more output means success)" << std::endl;

            for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
                H.insert( i );
            for( i = 1; i < NUMS; i += 2 )
                H.remove( i );

            for( i = 2; i < NUMS; i += 2 )
                if( H.find( i ) != i )
                    std::cout << "Find fails " << i << std::endl;

            for( i = 1; i < NUMS; i += 2 )
            {
                if( H.find( i ) != ITEM_NOT_FOUND )
                    std::cout << "OOPS!!! " <<  i << std::endl;
            }

            return 0;
        }
