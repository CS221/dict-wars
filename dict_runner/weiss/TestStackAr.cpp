        #include <iostream>
        #include "StackAr.h"

        int main( )
        {
            Stack<int> s;

            for( int i = 0; i < 10; i++ )
                s.push( i );
        
            while( !s.isEmpty( ) )
                std::cout << s.topAndPop( ) << std::endl;

            return 0;
        } 
