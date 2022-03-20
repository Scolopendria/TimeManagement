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


verStr* verStr::createChildren(std::string childName){
    children.push_back(verStr{childName});
    return this;
}

verStr* verStr::createAttribute(std::string ID_pair[2]){
    attribute.push_back({ID_pair[0], ID_pair[1]});
    return this;
}

std::string verStr::read(std::string s, std::string::size_type &i){
    // if (String[0] != '"'){ // Guard deactivated beacuse all input will be purified through strip
    //     std::cerr << "Missing symbol: (\") at position 0: ***"
    //         << String << "***" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    std::string identifier;
    i++;
    while (i < s.length() && s[i] != '"') identifier += s[i++];
    i++;
    if (i == String.length()){
        std::cerr << "verStr::read(): File Overran" << std::endl;
        exit(EXIT_FAILURE);
    }
    return identifier;
}

std::string verStr::cut(std::string::size_type i[2]){
    std::string pStr;
    while (i[0] < i[1]) pStr += String[i[0]++];
    return pStr;
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
    std::string ID_pair[2];
    std::string pStr;
    std::string::size_type i{0}, pincer[2];
    int depth;
    char p;
    Name = read(String, i);
    i ++;
    
    while (i < (String.length() - 1)){
        pincer[0] = i;
        ID_pair[0] = read(String, i);
        p = String[i++];
        if (p == '='){
            ID_pair[1] = read(String, i);
            createAttribute(ID_pair);
        } else if (p == '{'){
            depth = 1;
            while (i < String.length() && depth != 0){
                switch (String[i]){
                    case '"': read(String, i);
                    break;
                    case '{': depth++; i++;
                    break;
                    case '}': depth--; i++;
                    break;
                    case '=': i++;
                    break;
                    default: std::cerr << "Unexpected Character: ("<< String[i] << ")" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            if (depth == 0){
                pincer[1] = i;
                createChildren(cut(pincer));
            } else if (i == String.length()){
                std::cerr << "verStr::objectify(): File Overran" << std::endl;
                exit(EXIT_FAILURE);
            } else {
                std::cerr << "verStr::objectify(): Unknown Error" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Unexpected character: (" << p << ")" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << Name << std::endl;
    for (auto &ID_pair : attribute){
        for (auto &s: ID_pair){
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    return this;
}

#endif