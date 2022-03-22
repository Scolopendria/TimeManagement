#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <array>

class verStr{
    public:
        // initializer class
        verStr(std:: string s);
        ~verStr();
        // view class
        std::string getName();
        std::string updString();
        std::string format();
        std::string get(bool __ATTRIBUTES, bool __CHILDREN);
        std::string get(std::string attrName);
        // command class
        void attribute(std::string attrName, std::string attrValue);
        verStr* child(std::string childName, bool &t);
        void deleteAttribute(std::string attrName);
        void deleteChild(std::string childName);
    private:
        // utilities class
        void throwError(std::string err);
        std::string read(std::string s, std::string::size_type &i, bool t);
        std::string cut(std::string::size_type i[2]);
        // standard class
        verStr* createAttribute(std::string ID_pair[2]);
        verStr* createChild(std::string childString);
        // initializer class
        verStr* strip();
        verStr* objectify();
        // members
        std::string String;
        std::string Name;
        std::vector<std::array<std::string, 2>> attributes;
        std::vector<verStr> children;
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