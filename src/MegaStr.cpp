#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "Classes.h"
#include <fstream>
#include <sstream>

MegaStr::~MegaStr(){
    std::ofstream file("system.txt");
    file << vStr.format();
    file.close();
}

std::string MegaStr::getFile(){
    std::ifstream file("system.txt");
    if (!file.is_open()) return "\"Automatic Time Management\"{}";

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string pStr{ss.str()};
    file.close();

    if (pStr.find_first_not_of(" \t\n\v\r") != std::string::npos) return pStr;
    return "\"Automatic Time Management\"{}";
}

#endif