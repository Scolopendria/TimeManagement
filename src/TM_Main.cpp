// Main of TM

#include <iostream>
#include "Headers.h"

int main(){
    MegaStr mStr;
    if (mStr.vStr.getName() != "Automatic Time Management"){
        // Name check
        std::string input;
        std::cout << "Detected name is not: \"Automatic Time Management\". "
            << "Do you wish to proceed? (y/n): " << std::endl;
        std::cin >> input;
        if (input != "y"){
            std::cout << "Program aborted" << std::endl;
            exit(EXIT_SUCCESS);
        }
    }

    mStr.putFile();
    return 0;
}