        #include "StackLi.h"
        #include <iostream>

        int main( )
        {
            Stack<int> s, s1;

            for( int i = 0; i < 10; i++ )
                s.push( i );
        
            s1 = s;

            std::cout << "s" << std::endl;
            while( !s.isEmpty( ) )
                std::cout << s.topAndPop( ) << std::endl;

            std::cout << std::endl << "s1" << std::endl;
            while( !s1.isEmpty( ) )
                std::cout << s1.topAndPop( ) << std::endl;

            return 0;
        } 
