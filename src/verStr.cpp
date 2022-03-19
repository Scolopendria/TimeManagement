#ifndef VERSTR_CPP
#define VERSTR_CPP

#include "Classes.h"
#include <iostream>

verStr::verStr(std::string s){
    setString(s)->strip();
    std::cout << "Creating verStr Object" << std::endl;
}

std::string verStr::getString(){
    return String;
}

verStr* verStr::setString(std:: string s){
    String = s;
    return this;
}

verStr* verStr::strip(){
    std::string pStr;
    for (std::string::size_type i = 0; i < String.length(); i++){
        switch (String[i]){
            case 13:
            case 10:
            case '\t':
            case ' ': break;
            case '"': pStr += String[i++];
            //
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){// Guard for: Incorrect Formatting
                std::cerr << "Corrupted file" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += '"';
            break;
            //
            default: pStr += String[i];
        }
    }
    
    std::cout << pStr << std::endl;
    String = pStr;
    return this;
}

std::string verStr::format(){
    std::string pStr;
    int indent{0};
    for (std::string::size_type i = 0; i < String.length(); i++){
        switch (String[i]){
            case '"': pStr += String[i++];
            //
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){// Guard for: Incorrect Formatting
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
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){// Guard for: Incorrect Formatting
                std::cerr << "Corrupted file 3" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += "\"\n";
            for (int ctr = 0; ctr < indent; ctr++) pStr += '\t';
            break;
            //
            default: pStr += String[i];
        }
    }

    std::cout << pStr << std::endl;
    return pStr;
}

#endif