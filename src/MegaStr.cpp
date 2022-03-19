#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "Classes.h"
#include <iostream>
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    vStr.setString(getFile())->strip();
    std::cout << "Created MegaStr Object" << std::endl;
}

std::string MegaStr::getFile(){
    auto ss = std::ostringstream{};
    std::ifstream file("system.txt");
    if (!file.is_open()) {// Guard for: File Not Found
        std::cout << "Creating new file" << std::endl;
        return "\"Automatic Time Management\":{}";
    }
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

void MegaStr::putFile(){
    std::ofstream file("system.txt");
    file << vStr.format();
    file.close();
}

#endif