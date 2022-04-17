#ifndef VERSTR_CPP
#define VERSTR_CPP

#include "Classes.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>

//format with commmas
verStr::verStr(std::string s, verStr* parentNode){
    parent = parentNode;
    String = s;
    strip()->objectify();
}

verStr::verStr(std::string s){
    parent = this;
    String = s;
    strip()->objectify();
}

void verStr::throwError(std::string err){
    std::cerr << err << std::endl;
    exit(EXIT_FAILURE);
}

std::string verStr::read(std::string s, std::string::size_type &i, bool t){
    std::string identifier;
    if (t) identifier += s[i];
    i++;
    while (i < s.length() && s[i] != '"') identifier += s[i++];
    if (i == String.length()) throwError("verStr::read(): File Overran");
    if (t) identifier += s[i];
    i++;
    return identifier;
}

std::string verStr::cut(std::string::size_type i[2]){
    std::string pStr;
    while (i[0] < i[1]) pStr += String[i[0]++];
    return pStr;
}

verStr* verStr::createChild(std::string childString){
    children.push_back(verStr{childString});
    return this;
}

verStr* verStr::createAttribute(std::string ID_pair[2]){
    attributes.push_back({ID_pair[0], ID_pair[1]});
    return this;
}


std::string verStr::getName(){
    return Name;
}

std::string verStr::updString(){
    auto encapsulate = [](std::string identifier){return std::string{"\"" + identifier + "\""};};
    String = (encapsulate(Name) + "{");
    for (auto &ID_pair : attributes) ((String += encapsulate(ID_pair[0])) += "=") += encapsulate(ID_pair[1]);
    for (auto &c : children)  String += c.updString();
    String += "}";
    return String;
}

std::string verStr::format(){
    updString();
    bool gate{false};
    int depth{0};
    std::string pStr;
    std::string::size_type i{0};
    
    while(i < String.length()){
        switch (String[i]){
            case '"': pStr += read(String, i, true);
            if (gate){
                pStr += '\n';
                for (int ctr = 0; ctr < depth; ctr++) pStr += '\t';
                gate = false;
            }
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

std::string verStr::get(bool __ATTRIBUTES, bool __CHILDREN){
    std::string pStr{Name + ":\n"};

    if (__ATTRIBUTES){
        for (auto &ID_pair : attributes)
            (((pStr += ID_pair[0]) += ": ") += ID_pair[1]) += "\n";
    }

    if (__CHILDREN){
        for (auto &c : children)
            (pStr += c.getName()) += "\n";
    }

    return pStr;
}

std::string verStr::get(std::string attrName){
    for (auto &ID_pair : attributes) if (ID_pair[0] == attrName) return ID_pair[1];
    return std::string{"NULL"};
}

std::vector<verStr>* verStr::getChildrenList(){
    return &children;
}

std::vector<std::array<std::string, 2>> verStr::getAttributesList(){
    return attributes;
}

verStr* verStr::getParent(){
    return parent;
}

verStr* verStr::attribute(std::string attrName, std::string attrValue){
    std::string pStr[2]{attrName, attrValue};
    deleteAttribute(attrName);
    createAttribute(pStr);
    return this;
}

verStr* verStr::child(std::string childName, bool &t){
    for (auto &c : children) if (c.getName() == childName) return &c;
    if (t){ t = false; return this; }

    std::string childString{"\"" + childName + "\"{}"};
    createChild(childString);
    return &children.back();
}

void verStr::deleteAttribute(std::string attrName){
    auto iter = attributes.begin();
    for (auto &ID_pair: attributes){
        if (ID_pair[0] == attrName){
            attributes.erase(iter);
            return;
        }
        iter++;
    }
}

void verStr::deleteChild(std::string childName){
    auto iter = children.begin();
    for (auto &c: children){
        if (c.getName() == childName){
            children.erase(iter);
            return;
        }
        iter++;
    }
}

verStr* verStr::sortAttributes(){
    std::sort(attributes.begin(), attributes.end(),
        [](const std::array<std::string, 2> &a, const std::array<std::string, 2> &b){
            auto startTime = [](std::string bookedString){
                std::string::size_type i{0};
                while (bookedString[i] != '-') i++;
                return std::stoi(bookedString.substr(0, i));
            };
            return (startTime(a[0]) < startTime(b[0]));
        }
    );

    return this;
}

verStr* verStr::strip(){
    int depth{0};
    std::string pStr;
    std::string::size_type i{0};

    while (i < String.length()){
        switch (String[i]){
            case '"': pStr += read(String, i, true);
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
    char p;
    int depth;
    std::string pStr, ID_pair[2];
    std::string::size_type i{0}, pincer[2];
    Name = read(String, i, false);
    
    i++;
    while (i < (String.length() - 1)){
        pincer[0] = i;
        ID_pair[0] = read(String, i, false);
        p = String[i++];
        if (p == '='){
            ID_pair[1] = read(String, i, false);
            createAttribute(ID_pair);
        } else if (p == '{'){
            depth = 1;
            while (i < String.length() && depth != 0){
                switch (String[i]){
                    case '"': read(String, i, false);
                    break;
                    case '{': depth++; i++;
                    break;
                    case '}': depth--; i++;
                    break;
                    case '=': i++;
                    break;
                    default: throwError("verStr::objectify(): Unexpected character: " + String[i]);
                }
            }
            if (depth == 0){
                pincer[1] = i;
                createChild(cut(pincer));
            } else if (i == String.length()) throwError("verStr::objectify(): File Overran");
            else throwError("verStr::objectify(): Unknown Error");
        } else throwError("verStr::objectify(): Unexpected character: " + p);
    }

    return this;
}

#endif