// Main of TM
//TM is getting rewritten to tm2 for increased functional and declarative programming styles
//Along with better formatting, readability, and modularity

#include "headers.h"
#include <iostream>

// introduece modularity with namespaces

int main(){ // Does not handle corrupted file contents while
            // Like expecting a number, but receives junk
            // Improve corrupted file handling
    MegaStr mStr{};
    handleUser(&mStr);
    scheduler(&mStr); // refactor scheduler to be able to be user called and modular

    return 0;
}