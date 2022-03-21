#ifndef CLASSES_H
#define CLASSES_H

#include "Classes.h"
#include <string>
#include <vector>
#include <array>
class verStr{
    //destruct/back-translate and command interface support still needed
    // 5 classes: command, view, standard, utilities, initializer
    public:
        // initializer class
        verStr(std:: string s);
        ~verStr();
        // view class
        //getName
        //getAllMembers
        // command class
        //child
        //value
        //delete member
        // standard class
        std::string getString(); //translate support                    //view
        std::string getName();                                          //view
        std::string format();                                           //view
        std::string getValue(std::string attrName);                     //view
        verStr* child(std::string childName);                           //view
        void setAttribute(std::string attrName, std::string attrValue); //command
        verStr* createAttribute(std::string identifier[2]);             //command
        verStr* createChildren(std::string childName);                  //command
        void deleteAttribute(std::string attrName);                     //command
        void deleteChildren(std::string childName);                     //command
    private:
        // utilities class
        void throwError(std::string err);
        std::string read(std::string s, std::string::size_type &i, bool t);
        std::string cut(std::string::size_type i[2]);
        // initializer class
        verStr* strip();
        verStr* objectify();
        verStr* translate(); // needs to be defined
        // members
        std::string String;
        std::string Name;
        std::vector<std::array<std::string, 2>> attribute;
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