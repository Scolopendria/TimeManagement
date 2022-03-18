#ifndef MEGASTR_H
#define MEGASTR_H

class MegaStr {
    public:
        MegaStr();
        void putFile();
        std::string format(std::string s);
    private:
        std::string getFile(const std::string& path);
        std::string strip(std::string s);
        std::string str = strip(getFile("system.txt"));
};

#include "MegaStr.cpp"
#endif