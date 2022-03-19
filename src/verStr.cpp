#ifndef VERSTR_CPP
#define VERSTR_CPP

#include "Classes.h"
#include <iostream>

verStr::verStr(std::string s){
    String = s;
    strip();
    std::cout << "Created verStr Object" << std::endl;
}

std::string verStr::getString(){
    return String;
}

std::string verStr::getName(){
    std::string name;
    if (String[0] != '"') {
        std::cerr << "Missing symbol: (\") at position 0: ***"
            << String << "***" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (std::string::size_type i = 1; i < String.length() && String[i] != '"'; i++) name += String[i];
    return name;
}

verStr* verStr::strip(){
    std::string pStr;
    std::string::size_type i = 0;
    while (i < String.length()){
        switch (String[i]){
            case '"': pStr += String[i++];
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){
                std::cerr << "verStr::strip(): File Overran" << std::endl;
                exit(EXIT_FAILURE);
            }
            case '{': case '}': case '=':
            pStr += String[i++];
            break;
            default: i++;
        }
    }

    String = pStr;
    return this;
}

std::string verStr::format(){
    std::string pStr;
    std::string::size_type i = 0;
    int indent{0};
    bool gate{false};
    while(i < String.length()){
        switch (String[i]){
            case '"': pStr += String[i++];
            // Handling identifiers
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){
                std::cerr << "verStr::format(): File Overran" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += String[i++];
            if (gate){
                pStr += '\n';
                for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
                gate = false;
            }
            //
            break;
            case '{': pStr += "{\n"; i++; indent++;
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            case '}': pStr.pop_back(); pStr += "}\n"; i++; indent--;
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            case '=': pStr += String[i++]; gate = true;
            break;
            default: i++;
        }
    }

    return pStr;
}

#endif