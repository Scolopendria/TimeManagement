#ifndef CLASSES_H
#define CLASSES_H

class verStr{
    public:
        verStr() = default;
        verStr(std:: string s);
        std::string getString();
        std::string format();
        verStr* setString(std::string s);
        verStr* strip();
    private:
        std::string String;
};

class MegaStr{
    public:
        MegaStr();
        void putFile();
    private:
        std::string getFile();
        verStr vStr;
};

#include "verStr.cpp"
#include "MegaStr.cpp"

#endif