        #include <iostream>
        #include "IntCell.h"

        int main( )
        {
/* 1*/      IntCell *m;

/* 2*/      m = new IntCell( 0 );
/* 3*/      m->write( 5 );
/* 4*/      std::cout << "Cell contents: " << m->read( ) << std::endl;

/* 5*/      delete m;
/* 6*/      return 0;
        }
