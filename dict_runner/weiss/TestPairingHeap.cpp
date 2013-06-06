        #include <iostream>
        #include "vector.h"
        #include "dsexceptions.h"
        #include "PairingHeap.h"

            // Test program
        int main( )
        {
            PairingHeap<int> h;

            int numItems = 4000;
            int i = 37;
            int j;

            std::cout << "Checking; no bad output is good" << std::endl;
            for( i = 37; i != 0; i = ( i + 37 ) % numItems )
                h.insert( i );

            for( i = 1; i < numItems; i++ )
            {
                int x;
                h.deleteMin( x );
                if( x != i )
                    std::cout << "Oops! " << i << std::endl;
            }

            vector<PairNode<int> * > p( numItems );
            for( i = 0, j = numItems / 2; i < numItems; i++, j =(j+71)%numItems )
                p[ j ] = h.insert(j + numItems );
            for( i = 0, j = numItems / 2; i < numItems; i++, j =(j+53)%numItems )
                h.decreaseKey( p[ j ], j );
            i = -1;
            
            PairingHeap<int> h2;

            h2 = h;
            while( !h2.isEmpty( ) )
            {
                int x;
                h2.deleteMin( x );
                if( x != ++i )
                    std::cout << "Oops! " << i << std::endl;
            }

            std::cout << "Check completed" << std::endl;
            return 0;
        }