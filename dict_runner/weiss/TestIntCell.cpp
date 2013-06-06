        #include <iostream>
        #include "IntCell.h"

        int main( )
        {
            IntCell m;   // Or, IntCell m( 0 ); but not IntCell m( );

            m.write( 5 );
            std::cout << "Cell contents: " << m.read( ) << std::endl;

            return 0;
        }
