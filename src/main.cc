#include "nomp.h"

#include <iostream>

int main (int argc, char *argv[])
{	
    if (argc > 1) {
        std::cout << std::endl;
        std::cout << "A simple Ncurses OpenVAS client (https://github.com/git-rep-src/nomp)" << std::endl;
        std::cout << "Version 0.1" << std::endl;
        std::cout << std::endl;
        return 0;
    }
    
    Nomp nomp;
    
    return 0;
}
