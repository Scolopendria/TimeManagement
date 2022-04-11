// Main of TM

#include "Headers.h"
#include <iostream>

int main(){
    MegaStr mStr{};
    // incorporate check into mStr
    if (mStr.vStr.getName() != "Automatic Time Management"){ // Guard against entering an unknown file. 
        std::cout << "Object name is not 'Automatic Time Management'." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    handleUser(&mStr); // refactor scheduler to be able to be user called
    scheduler(&mStr);

    return 0;
}