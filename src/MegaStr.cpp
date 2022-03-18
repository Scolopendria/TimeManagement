#ifndef MEGASTR_CPP
#define MEGASTR_CPP

#include "MegaStr.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MegaStr::MegaStr(){
    std::cout << "Creating MegaString Object" << std::endl;
}

std::string MegaStr::getFile(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream file(path);
    if (!file.is_open()) {// Guard for: File Not Found
        std::cout << "Creating new file" << std::endl;
        return "";
    }
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

std::string MegaStr::strip(std::string s){
    std::string pStr;
    for (std::string::size_type i = 0; i < s.length(); i++){
        switch (s[i]){
            case 13:
            case 10:
            case '\t':
            case ' ': break;
            case '"': pStr += s[i++];
            //
            while (s[i] != '"' && i < s.length()) pStr += s[i++];
            if (i == s.length()){// Guard for: Incorrect Formatting
                std::cerr << "Corrupted file" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += '"';
            break;
            //
            default: pStr += s[i];
        }
    }
    
    std::cout << pStr << std::endl;
    return pStr;
}

std::string MegaStr::format(std::string s){
    std::string pStr;
    int indent{0};
    for (std::string::size_type i = 0; i < s.length(); i++){
        switch (s[i]){
            case '"': pStr += s[i++];
            //
            while (s[i] != '"' && i < s.length()) pStr += s[i++];
            if (i == s.length()){// Guard for: Incorrect Formatting
                std::cerr << "Corrupted file 2" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += '"'; 
            break;
            //
            case '{': pStr += "{\n";
            indent++;
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            case '}': pStr.pop_back(); pStr += "}\n";
            indent--;
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            case '=': pStr += "=\""; i += 2;
            //
            while (s[i] != '"' && i < s.length()) pStr += s[i++];
            if (i == s.length()){// Guard for: Incorrect Formatting
                std::cerr << "Corrupted file 3" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += "\"\n";
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            //
            default: pStr += s[i];
        }
    }

    std::cout << pStr << std::endl;
    return pStr;
}

void MegaStr::putFile(){
    std::ofstream file("system.txt");
    file << format(str);
    file.close();
}

#endif