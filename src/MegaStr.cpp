#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "Classes.h"
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    if (vStr.getName() != "Automatic Time Management"){ // Guard against entering an unknown file. 
        std::cout << "Object name is not 'Automatic Time Management'." << std::endl;
        exit(EXIT_FAILURE);
    }
}

MegaStr::~MegaStr(){
    std::ofstream file("system.txt");
    file << vStr.format();
    file.close();
}

std::string MegaStr::getFile(){
    std::ifstream file("system.txt");
    if (!file.is_open()) return "\"Automatic Time Management\"{}";

    std::ostringstream ss{};
    ss << file.rdbuf();
    file.close();
    std::string pStr{ss.str()};

    if (pStr.find_first_not_of(" \t\n\v\r") != std::string::npos) return pStr;
    return "\"Automatic Time Management\"{}";
}

#endif