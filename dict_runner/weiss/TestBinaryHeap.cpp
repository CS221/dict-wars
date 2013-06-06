        #include <iostream>
        #include "BinaryHeap.h"
        #include "dsexceptions.h"

            // Test program
        int main( )
        {
            int numItems = 10000;
            BinaryHeap<int> h( numItems );
            int i = 37;
            int x;

            try
            {
                for( i = 37; i != 0; i = ( i + 37 ) % numItems )
                    h.insert( i );
                for( i = 1; i < numItems; i++ )
                {
                    h.deleteMin( x );
                    if( x != i )
                        std::cout << "Oops! " << i << std::endl;
                }
                for( i = 37; i != 0; i = ( i + 37 ) % numItems )
                    h.insert( i );
                h.insert( 0 );
                h.insert( i = 999999 );  // Should overflow
            }
            catch( Overflow )
              { std::cout << "Overflow (expected)! " << i  << std::endl; }

            return 0;
        }
