        #include <iostream>

        void printDigit( int n )
        {
            std::cout << n;
        }

/* START: Fig01_04.txt */
        void printOut( int n )  // Print nonnegative n
        {
            if( n >= 10 )
                printOut( n / 10 );
            printDigit( n % 10 );
        }
/* END */

        int main(  )
        {
            printOut( 1369 );
            std::cout << std::endl;
            return 0;
        }
