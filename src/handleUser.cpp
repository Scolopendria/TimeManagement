#ifndef HANDLEUSER_CPP
#define HANDLEUSER_CPP

#include "Classes.h"
#include <iostream>
#include <string>
#include <vector>

void handleUser(MegaStr *mStr){
    bool ctr{false}, gate[2]{true, true};
    std::string str[2];
    std::vector<verStr*> path;

    while (gate[0]){
        gate[1] = true;
        path.clear();
        std::cout << "Your name: ";
        std::cin >> str[0];
        path.push_back(mStr->vStr.child(str[0], ctr));
        path.push_back(path.back()->child("Goals", ctr));

        while (gate[1]){
            std::cout << "Enter command: ";
            std::cin >> str[0];
            if (str[0] == "!quit"){
                gate[1] = false;
                gate[0] = false;
            } else if (str[0] == "!logout"){
                gate[1] = false;
            } else if (str[0] == "!attribute"){
                std::cin >> str[0] >> str[1];
                path.back()->attribute(str[0], str[1]);
            } else if (str[0] == "!child"){
                std::cin >> str[0];
                if (str[0] == "check"){
                    ctr = true;
                    std::cin >> str[0];
                    path.back()->child(str[0], ctr);
                    if (ctr){
                        std::cout << "Task: '" << str[0] << "' exists." << std::endl;
                        ctr = false;
                    } else std::cout << "Task: '" << str[0] << "' does not exist." << std::endl;
                } else if (str[0] == "enter"){
                    std::cin >> str[0];
                    path.push_back(path.back()->child(str[0], ctr));
                } else path.back()->child(str[0], ctr);
            } else if (str[0] == "!out"){
                if (path.back()->getName() == "Goals") std::cout << "Already at Goals." << std::endl;
                else path.pop_back();
            } else if (str[0] == "!delete"){
                std::cin >> str[0] >> str[1];
                if (str[0] == "attribute") path.back()->deleteAttribute(str[1]);
                else if (str[0] == "child") path.back()->deleteChild(str[1]);
                else std::cout << "Invalid type" << std::endl;
            } else if (str[0] == "!view") std::cout << path.back()->get(true, true) << std::endl;
            else if (str[0] == "!help") std::cout << "Go read the source code." << std::endl;
            else std::cout << "Command does not exist. !help for more." << std::endl;
        }
    }
}

#endif