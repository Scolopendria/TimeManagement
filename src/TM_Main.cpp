// Main of TM

#include "headers.h"
#include <iostream>

int main(){
    MegaStr mStr{};
    handleUser(&mStr); // refactor scheduler to be able to be user called and modular
    scheduler(&mStr);

    return 0;
}