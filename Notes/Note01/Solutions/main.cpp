#include <iostream>
#include "Set.h"

int main()
{
    tcp::Set<std::string> Q, F;
    tcp::Set<char> S;
    tcp::Set<tcp::Triple> d;
    std::string q0, file;

    std::cout << "Enter a file: ";
    std::cin >> file;
    
    std::cout << tcp::FileReader(file,Q,S,d,q0,F) << "\n";
    std::cout << "Q = " << Q << "\n";
    std::cout << "S = " << S << "\n";
    std::cout << "d = " << d << "\n";
    std::cout << "q0 = " << q0 << "\n";
    std::cout << "F = " << F << "\n";
    return 0;
}