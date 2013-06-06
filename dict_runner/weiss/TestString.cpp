#include "string.cpp"

int main( )
{
    string a = "this";
    string b = "that";

    a += a;
    string c = a;
    c += "then";
    std::cout << (c>a) << " should be 1" << std::endl;

    string d;
    for( int i = 0; i < 300000; i++ )
        d += "a";
    std::cout << d.length( ) << "\n";

    std::cout << "Enter a string\n" ;
    string e;
    std::cin >> e;
    e += "";
    d = e;
    std::cout << d << std::endl;
    for( int j = 0; j < e.length( ); j++ )
        std::cout << d[ j ];
    std::cout << "\n";
    return 0;
}
    
