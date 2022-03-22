// Main of TM

#include <iostream>
#include "Headers.h"

int main(){
    bool ctr{false};
    std::string pStr1, pStr2;
    std::vector<verStr*> path;
    
    MegaStr mStr{};
    if (mStr.vStr.getName() != "Automatic Time Management"){ // Guard against entering an unknown file. 
        std::cout << "Object name is not 'Automatic Time Management'." << std::endl;
        exit(EXIT_FAILURE);
    }

    path.push_back(mStr.vStr.child("Goals", ctr));
    while (true){
        pStr1 = "";
        std::cout << "Enter command: ";
        std::cin >> pStr1;
        if (pStr1 == "!quit") break;
        else if (pStr1 == "!attribute"){
            pStr1 = ""; pStr2 = "";
            std::cin >> pStr1;
            std::cin >> pStr2;
            path.back()->attribute(pStr1, pStr2);
        } else if (pStr1 == "!child"){
            pStr1 = ""; pStr2 = "";
            std::cin >> pStr1;
            if (pStr1 == "check"){
                ctr = true;
                std::cin >> pStr2;
                path.back()->child(pStr2, ctr);
                if (ctr){
                    std::cout << "Task: '" << pStr2 << "' exists." << std::endl;
                    ctr = false;
                } else std::cout << "Task: '" << pStr2 << "' does not exist." << std::endl;
            } else if (pStr1 == "enter"){
                std::cin >> pStr2;
                path.push_back(path.back()->child(pStr2, ctr));
            } else path.back()->child(pStr1, ctr);
        } else if (pStr1 == "!out"){
            if (path.back()->getName() == "Goals") std::cout << "Already at Goals." << std::endl;
            else path.pop_back();
        } else if (pStr1 == "!delete"){
            pStr1 = ""; pStr2 = "";
            std::cin >> pStr1;
            if (pStr1 == "attribute"){
                std::cin >> pStr2;
                path.back()->deleteAttribute(pStr2);
            } else if (pStr1 == "child"){
                std::cin >> pStr2;
                path.back()->deleteChildren(pStr2);
            } else std::cout << "Invalid type" << std::endl;
        } else if (pStr1 == "!help"){
            std::cout << "Go read the source code." << std::endl;
        } else std::cout << "Command does not exist. !help for more." << std::endl;
    }
}