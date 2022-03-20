#ifndef VERSTR_CPP
#define VERSTR_CPP

#include "Classes.h"
#include <iostream>

verStr::verStr(std::string s){
    String = s;
    strip()->objectify();
    std::cout << "Created verStr Object" << std::endl;
}

std::string verStr::getString(){
    return String;
}

std::string verStr::getName(){
    return Name;
}

std::string verStr::format(){
    std::string pStr;
    std::string::size_type i{0};
    int depth{0};
    bool gate{false};
    while(i < String.length()){
        switch (String[i]){
            case '"': pStr += String[i++];
            // Handling identifiers
            // use read()
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){
                std::cerr << "verStr::format(): File Overran" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += String[i++];
            if (gate){
                pStr += '\n';
                for (int ctr = 0; ctr < depth; ctr++) pStr += '\t';
                gate = false;
            }
            //
            break;
            case '{': pStr += "{\n"; i++; depth++;
            for (int ctr = 0; ctr < depth; ctr++) pStr += '\t';
            break;
            case '}': pStr.pop_back(); pStr += "}\n"; i++; depth--;
            for (int ctr = 0; ctr < depth; ctr++) pStr += '\t';
            break;
            case '=': pStr += String[i++]; gate = true;
            break;
            default: i++;
        }
    }

    return pStr;
}

verStr* verStr::createAttribute(std::string identifier[2]){
    attribute.push_back({identifier[0], identifier[1]});
    return this;
}

std::string verStr::read(std::string s, std::string::size_type &i){
    // if (String[0] != '"'){ // Possible redundant guard since all input will be filtered through strip()
    //     std::cerr << "Missing symbol: (\") at position 0: ***"
    //         << String << "***" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    std::string identifier;
    i++;
    while (i < s.length() && s[i] != '"') identifier += s[i++];
    i++;
    return identifier;
}

verStr* verStr::strip(){
    std::string pStr;
    std::string::size_type i{0};
    int depth{0};
    while (i < String.length()){
        switch (String[i]){
            // use read()
            case '"': pStr += String[i++];
            while (String[i] != '"' && i < String.length()) pStr += String[i++];
            if (i == String.length()){
                std::cerr << "verStr::strip(): File Overran" << std::endl;
                exit(EXIT_FAILURE);
            }
            pStr += String[i++];
            break;
            case '{': depth++; pStr += String[i++];
            break;
            case '}': depth--; pStr += String[i++];
            if (depth == 0){
                String = pStr;
                return this;
            }
            break;
            case '=': pStr += String[i++];
            break;
            default: i++;
        }
    }

    String = pStr;
    return this;
}

verStr* verStr::objectify(){
    std::string pStr[2]{"", ""};
    char p;
    std::string::size_type i{0};
    int depth{0};
    Name = read(String, i);
    i ++;
    
    while (i < String.length()){
        pStr[0] = read(String, i);
        p = String[i++];
        if (p == '='){
            pStr[1] = read(String, i);
            createAttribute(pStr);
        }
        // Add child support
    }

    for (auto &ID_pair : attribute){
        for (auto &s: ID_pair){
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    return this;
}

#endif