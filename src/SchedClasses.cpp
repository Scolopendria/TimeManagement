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

scheduleProgress::scheduleProgress(std::string fullpath){
    fpath = fullpath;
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

int task::getTime(){
    return timeUsed;
}
#endif