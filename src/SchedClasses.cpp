#ifndef SCHEDCLASSES_CPP
#define SCHEDCLASSES_CPP

#include "Classes.h"
#include <iostream>
#include <chrono>

Calendar::Calendar(){
    std::stringstream ss;
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    )};
    auto ltm{std::localtime(&now)};
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    date = ltm->tm_mday;
    ss << year << ":" << month << ":" << date;
    baseDate = ss.str();
};

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

std::string attributeContainer::get(std::string name){
    for(auto&& ID_pair : cascadeAttributes)
        if (name == ID_pair[0]) return ID_pair[1];
    return "NULL";
}

attributeContainer* attributeContainer::set(std::string name, std::string value){
    return set(std::array<std::string, 2>{name, value});
}
// all deletions done in reverse order
attributeContainer* attributeContainer::set(std::array<std::string, 2> ID_pair){
    for (int iter{(int)cascadeAttributes.size()-1};  iter > -1; iter--)
        if (ID_pair[0] == cascadeAttributes[iter][0]) cascadeAttributes.erase(cascadeAttributes.begin() + iter);
    cascadeAttributes.push_back(ID_pair);
    return this;
}

scheduleProgress::scheduleProgress(std::string fullpath__){
    fullpath = fullpath__;
}

task::task(std::string taskName, int start, int end){
    std::stringstream ss;

    name = taskName;
    startTime = start;
    endTime = end;
    timeUsed = end - start;

    ss << startTime / 60 << ":" << startTime % 60 << "-" << endTime / 60 << ":" << endTime % 60;
    stdTime = ss.str();
}

std::string task::getName(){
    return name;
}

std::string task::getFullStdTime(){
    return stdTime;
}

int task::getStart(){
    return startTime;
}

int task::getEnd(){
    return endTime;
}

int task::getTimeUsed(){
    return timeUsed;
}

#endif