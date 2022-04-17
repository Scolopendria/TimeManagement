// Main of TM

#include "headers.h"
#include <iostream>

int main(){ // Does not handle corrupted file contents while
            // Like expecting a number, but receives junk
            // Improve corrupted file handling
    MegaStr mStr{};
    handleUser(&mStr);
    scheduler(&mStr); // refactor scheduler to be able to be user called and modular

    return 0;
}