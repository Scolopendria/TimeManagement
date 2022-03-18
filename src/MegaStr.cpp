#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "MegaStr.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    std::cout<< "Creating MegaString Object" <<std::endl;
}

std::string MegaStr::getFile(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" <<std:: endl;
        exit(EXIT_FAILURE);
    }
    ss << file.rdbuf();
    return ss.str();
}

std::string MegaStr::strip(std::string s){
    std::string pStr;
    for (std::string::size_type i = 0; i < s.length(); i++){
        switch (s[i]){
            case 13: break;
            case 10: break;
            case '\t': break;
            case ' ': break;
            case '"': pStr += s[i++];
                while (s[i] != '"') pStr += s[i++]; // implement safety net later
                pStr += s[i];
                break;
            default: pStr += s[i];
        }
    }
    
    //
    std::cout<< pStr <<std::endl;
    return pStr;
}

#endif