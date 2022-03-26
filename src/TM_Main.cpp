// Main of TM

#include "Headers.h"
#include <iostream>

int main(){
    MegaStr mStr{};
    if (mStr.vStr.getName() != "Automatic Time Management"){ // Guard against entering an unknown file. 
        std::cout << "Object name is not 'Automatic Time Management'." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    handleUser(&mStr);
    scheduler(&mStr);

    return 0;
}