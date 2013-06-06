 
        #include <iostream>

        /**
         * A class for simulating an integer memory cell.
         */
        class IntCell
        {
          public:
/* 1*/      explicit IntCell( int initialValue = 0 )
/* 2*/        : storedValue( initialValue ) { }
/* 3*/      int read( ) const
/* 4*/        { return storedValue; }
/* 5*/      void write( int x )
/* 6*/        { storedValue = x; }
          private:
/* 7*/      int storedValue;
        };

        int main( )
        {
            IntCell m;

            m.write( 5 );
            std::cout << "Cell contents: " << m.read( ) << std::endl;

            return 0;
        }
