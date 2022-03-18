// read file into string
#ifndef GETFILE_CPP
#define GETFILE_CPP

#include <iostream>
#include <fstream>
#include <sstream>

std::string getFile(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" <<std:: endl;
        exit(EXIT_FAILURE);
    }
    ss << file.rdbuf();
    return ss.str();
}

#endif