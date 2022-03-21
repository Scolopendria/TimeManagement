#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "Classes.h"
#include <iostream>
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    //std::cout << "Created MegaStr Object" << std::endl;
}

MegaStr::~MegaStr(){
    putFile();
    //std::cout << "Destroyed MegaStr Object" << std::endl;
}

std::string MegaStr::getFile(){
    std::ifstream file("system.txt");
    if (!file.is_open()) return "\"Automatic Time Management\"{}";
    auto ss = std::ostringstream{};
    ss << file.rdbuf();
    std::string pStr{ss.str()};
    if (pStr == "") return "\"Automatic Time Management\"{}";
    return pStr;
}

void MegaStr::putFile(){
    std::ofstream file("system.txt");
    file << vStr.format();
    file.close();
}

#endif