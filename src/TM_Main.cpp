// Main of TM

#include "headers.h"
#include <iostream>

int main(){
    MegaStr mStr{};
    // incorporate check into mStr
    if (mStr.vStr.getName() != "Automatic Time Management"){ // Guard against entering an unknown file. 
        std::cout << "Object name is not 'Automatic Time Management'." << std::endl;
        exit(EXIT_FAILURE);
    }
    //testtesttest
    handleUser(&mStr); // refactor scheduler to be able to be user called and modular
    scheduler(&mStr);

    return 0;
}