        #include <iostream>
        #include "QueueAr.h"
        
        int main( )
        {
            Queue<int> q;

            for( int j = 0; j < 5; j++ )
            {
                for( int i = 0; i < 5; i++ )
                    q.enqueue( i );
        
                while( !q.isEmpty( ) )
                    std::cout << q.dequeue( ) << std::endl;
            }

            return 0;
        } 
