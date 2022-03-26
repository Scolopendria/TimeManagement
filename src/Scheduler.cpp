#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "Classes.h"
#include <iostream>
#include <vector>
#include <chrono>

class Calendar{
    public:
        void init(int offset){
            std::stringstream ss;
            auto now{std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now() + std::chrono::days(offset)
            )};
            auto ltm{std::localtime(&now)};
            year = 1900 + ltm->tm_year;
            month = 1 + ltm->tm_mon;
            date = ltm->tm_mday;
            weekday = ltm->tm_wday;
            if (offset == 0) minute_t = (ltm->tm_hour * 60) + ltm->tm_min;
            else minute_t = 0;
            ss << year << ":" << month << ":" << date;
            strDate = ss.str();
        }
        int year;
        int month;
        int date;
        int weekday;
        int minute_t;
        std::string strDate;
};

void scheduler(MegaStr *mStr);
void schedule(
    verStr *vStr,
    verStr *schedulePath,
    Calendar caltime, 
    std::vector<std::array<std::string, 2>> cascadeAttributes
);
int startTime(std::string bookedString);
int endTime(std::string bookedString);

void scheduler(MegaStr *mStr){
    bool ctr{false};
    Calendar caltime;
    std::vector<std::array<std::string, 2>> cascadeAttributes;
    
    for (int i{0}; i < 14; i++){
        caltime.init(i);
        for (auto &c : *mStr->vStr.getChildrenList())
            schedule(c.child("Goals", ctr), c.child("Schedule", ctr)->child(caltime.strDate, ctr), caltime, cascadeAttributes);
    }
    
    std::cout << mStr->vStr.format() << std::endl;
    return;
}

void schedule(
    verStr *vStr,
    verStr *schedulePath,
    Calendar caltime,
    std::vector<std::array<std::string, 2>> cascadeAttributes
){
    // cascadeAttributes = off, rescheduling = off, "proper classing and automatic declarations" = off
    int startSearchTime = caltime.minute_t;
    auto bookedTime{schedulePath->getAttributesList()};
    std::size_t iter{0};
    std::stringstream ss;
    if (vStr->get("time") == "NULL") vStr->attribute("time", "30");

    if (vStr->get("class") == "task" || vStr->get("class") == "NULL"){
        while (iter < bookedTime.size()){
            if (startSearchTime <= endTime(bookedTime[iter][0])){
                if (startSearchTime >= startTime(bookedTime[iter][0])) startSearchTime = endTime(bookedTime[iter][0]) + 1;
                if (startSearchTime + std::stoi(vStr->get("time")) < startTime(bookedTime[iter + 1][0])){
                    ss << startSearchTime << "-" << startSearchTime + std::stoi(vStr->get("time"));
                    schedulePath->attribute(ss.str(), vStr->getName());
                    std::cout << "Successful scheduling." << std::endl;
                    break;
                } else startSearchTime = endTime(bookedTime[iter + 1][0]);
            }
            iter++;
        }
        if (iter == bookedTime.size()){
            if (startSearchTime + std::stoi(vStr->get("time")) < 1440){
                ss << startSearchTime << "-" << startSearchTime + std::stoi(vStr->get("time"));
                schedulePath->attribute(ss.str(), vStr->getName());
                std::cout << "Successful scheduling." << std::endl;
            } else std::cout << "Unable to schedule." << std::endl;
        }
    }

    for (auto &c : *vStr->getChildrenList()) schedule(&c, schedulePath, caltime, cascadeAttributes);

    return;
}

int startTime(std::string bookedString){
    std::string::size_type i{0};
    std::stringstream ss;
    while (bookedString[i] != '-') i++;
    bookedString.substr(0, i);
    return std::stoi(bookedString);
}

int endTime(std::string bookedString){
    std::string::size_type i{0};
    while (bookedString[i] != '-') i++;
    i++;
    bookedString.substr(i, bookedString.length()-i);
    return std::stoi(bookedString);
}

// std::sort(v.begin(), v.end(), [](auto &left, auto &right) {
//     return left.second < right.second;
// });


#endif