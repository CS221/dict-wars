        #include <iostream>

/* START: Fig10_43.txt */
        double eval( int n )
        {
            if( n == 0 )
                return 1.0;
            else
            {
                double sum = 0.0;
                for( int i = 0; i < n; i++ )
                    sum += eval( i );
                return 2.0 * sum / n + n;
            }
        }
/* END */

        int main( )
        {
            std::cout << "eval( 10 ) = " << eval( 10 ) << std::endl;
            return 0;
        }
