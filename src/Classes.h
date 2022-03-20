#ifndef CLASSES_H
#define CLASSES_H

#include "Classes.h"
#include <vector>
#include <array>
class verStr{
    //child support, destruct/back-translate and command interface support still needed
    public:
        verStr(std:: string s);
        ~verStr(){};
        std::string getString();
        std::string getName();
        std::string format();
        std::vector<std::string> listChildren();
        std::vector<std::string> listAttribute();
        std::string getValue(std::string attrName);
        verStr* child(std::string childName);
        void setAttribute(std::string attrName, std::string attrValue);
        verStr* createChildren(std::string childName);
        verStr* createAttribute(std::string identifier[2]);
        void deleteChildren(std::string childName);
        void deleteAttribute(std::string attrName);
    private:
        std::string read(std::string s, std::string::size_type &i);
        verStr* strip();
        verStr* objectify();
        verStr* translate();
        std::string String;
        std::string Name;
        std::vector<verStr> children;
        std::vector<std::array<std::string, 2>> attribute;
};

class MegaStr{
    public:
        MegaStr();
        ~MegaStr();
        verStr vStr{getFile()};
    private:
        std::string getFile();
        void putFile();
};

#include "verStr.cpp"
#include "MegaStr.cpp"

#endif