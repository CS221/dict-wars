        #include <iostream>
        #include "DSL.h"

            // Test program
        int main( )
        {
            const int ITEM_NOT_FOUND = 99999999;
            DSL<int> t( ITEM_NOT_FOUND );
            int NUMS = 3900;
            const int GAP  =   37;
            int i;

            std::cout << "Checking... (no more output means success)" << std::endl;

            for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
                t.insert( i );

            if( NUMS < 40 )
                t.printList( );
            if( t.findMin( ) != 1 || t.findMax( ) != NUMS - 1 )
                std::cout << "FindMin or FindMax error!" << std::endl;

            for( i = 1; i < NUMS; i++ )
                if( t.find( i ) != i )
                    std::cout << "Find error1!" << std::endl;
            if( t.find( 0 ) != ITEM_NOT_FOUND )
                std::cout << "ITEM_NOT_FOUND failed!" << std::endl;

            return 0;
        }
