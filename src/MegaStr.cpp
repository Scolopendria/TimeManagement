#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "Classes.h"
#include <iostream>
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    std::cout << "Created MegaStr Object" << std::endl;
}

std::string MegaStr::getFile(){
    std::string pStr{""};
    auto ss = std::ostringstream{};
    std::ifstream file("system.txt");
    if (!file.is_open()) return "\"Automatic Time Management\"{}";
    ss << file.rdbuf();
    pStr = ss.str();
    if (pStr == "") return "\"Automatic Time Management\"{}";
    return pStr;
}

void MegaStr::putFile(){
    std::ofstream file("system.txt");
    file << vStr.format();
    file.close();
}

#endif