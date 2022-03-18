#ifndef MEGASTR_H
#define MEGASTR_H

#include "getFile.cpp"

class MegaStr {
    public:
        MegaStr();
        void filter();
    private:
        std::string str = getFile("system.txt");
};

#include "MegaStr.cpp"
#endif