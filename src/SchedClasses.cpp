#ifndef SCHEDCLASSES_CPP
#define SCHEDCLASSES_CPP

#include "Classes.h"
#include <iostream>
#include <chrono>

void Calendar::init(int offset){
    std::stringstream ss;
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now() + std::chrono::days(offset)
    )};
    auto ltm{std::localtime(&now)};
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    date = ltm->tm_mday;
    weekday = ltm->tm_wday;
    minute_t = (ltm->tm_hour * 60) + ltm->tm_min;
    ss << year << ":" << month << ":" << date;
    strDate = ss.str();
    if (offset) minute_t = 0;
};

scheduleProgress::scheduleProgress(std::string fullpath){
    fpath = fullpath;
    name = fullpath;
    std::string::size_type i{fullpath.length()-1};
    while (i != std::string::npos){
        if (fullpath[i] == ':'){
            name = fullpath.substr(i + 1);
            path = fullpath.substr(0, i);
            break;
        }
        i--;
    }
}

#endif