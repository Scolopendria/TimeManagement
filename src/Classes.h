#ifndef CLASSES_H
#define CLASSES_H

#include "Classes.h"
#include <vector>
class verStr{
    public:
        verStr(std:: string s);
        ~verStr(){;};
        std::string getString(); // to be deprecated
        std::string format();
        std::string getName();
        std::string getValue(std::string attrName);
        std::vector<std::string> listChildren();
        std::vector<std::string> listAttribute();
        verStr* child(std::string childName);
        void setValue(std::string attrName, std::string attrValue);
    private:
        std::string translate();
        verStr* strip();
        verStr* objectify();
        verStr* createChildren();
        verStr* createAttribute();
        verStr* createChildren(std::string childName);
        verStr* createAttribute(std::string attrName, std::string attrValue);
        std::string String;
        std::vector<verStr> children;
        std::vector<std::string> attribute[2];
};

class MegaStr{
    public:
        MegaStr();
        verStr vStr{getFile()};
        void putFile();
    private:
        std::string getFile();
};

#include "verStr.cpp"
#include "MegaStr.cpp"

#endif