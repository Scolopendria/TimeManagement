#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>

int startTime(std::string bookedString);
int endTime(std::string bookedString);
void scheduler(MegaStr *mStr);
void schedule(
    verStr *gRoot,
    verStr *sPath, 
    Calendar caltime
);
std::vector<scheduleProgress> scheduleProgressCheck(
    verStr *vStr,
    verStr *sPath,
    Calendar caltime,
    std::string fullpath,
    std::vector<std::array<std::string, 2>> cascadeAttributes
);

void scheduler(MegaStr *mStr){
    bool ctr{false};
    Calendar caltime;
    for (int i{0}; i < 14; i++){
        caltime.init(i);
        for (auto &c : *mStr->vStr.getChildrenList()){
            schedule(
                c.child("Goals", ctr),
                c.child("Schedule", ctr)->child(caltime.strDate, ctr),
                caltime
            );
        }
    }
}

void schedule(
    verStr *gRoot,
    verStr *sPath, 
    Calendar caltime
){
    // cascadeAttributes = off, rescheduling = off, "proper classing and automatic declarations" = off
    int startSearchTime, tail;
    std::size_t iter;
    std::stringstream ss;
    std::vector<std::array<std::string, 2>> bookedTime;
    auto itemList = scheduleProgressCheck(
        gRoot,
        sPath,
        caltime,
        "",
        std::vector<std::array<std::string, 2>>{}
    );

    while(!itemList.empty()){
        for (auto &item: itemList){
            startSearchTime = caltime.minute_t;
            tail = 0;
            iter = 0;
            bookedTime = sPath->sortAttributes()->getAttributesList();

            // condition checks..
            while (true){
                if (iter == bookedTime.size()){
                    if (startSearchTime + item.timeLeft < 1440){
                        ss << startSearchTime << "-" << startSearchTime + item.timeLeft;
                        sPath->attribute(ss.str(), item.fpath);
                        std::cout << "Successful scheduling." << std::endl;
                    } else std::cout << "Unable to schedule." << std::endl;
                    break;
                }

                if (startSearchTime > tail && startSearchTime < startTime(bookedTime[iter][0])){
                    if (startSearchTime + item.timeLeft < startTime(bookedTime[iter][0])){
                        ss << startSearchTime << "-" << startSearchTime + item.timeLeft;
                        sPath->attribute(ss.str(), item.fpath);
                        std::cout << "Successful scheduling." << std::endl;
                    } else startSearchTime = endTime(bookedTime[iter][0]);
                }
                
                if (
                    startSearchTime >= startTime(bookedTime[iter][0]) &&
                    startSearchTime <= endTime(bookedTime[iter][0])
                ) startSearchTime = endTime(bookedTime[iter][0]) + 1;
                
                tail = endTime(bookedTime[iter][0]);
                iter++;
            }
        }

        itemList = scheduleProgressCheck(
            gRoot,
            sPath,
            caltime,
            "",
            std::vector<std::array<std::string, 2>>{}
        );
    }
}

std::vector<scheduleProgress> scheduleProgressCheck(
    verStr *vStr,
    verStr *sPath,
    Calendar caltime,
    std::string fullpath,
    std::vector<std::array<std::string, 2>> cascadeAttributes
){
    if (fullpath != "") fullpath += ":";
    fullpath += vStr->getName();
    scheduleProgress pItem{fullpath};
    std::vector<scheduleProgress> itemList, pList;
    if (vStr->get("time") != "NULL") pItem.timeLeft = std::stoi(vStr->get("time"));   // assign time to stated value

    auto attributes{sPath->getAttributesList()};
    for (auto a : attributes){
        if (a[1] == fullpath){
            if (pItem.timeLeft + startTime(a[0]) - endTime(a[0]) < 0)
                sPath->deleteAttribute(a[0]);
            else pItem.timeLeft = pItem.timeLeft + startTime(a[0]) - endTime(a[0]);
        }
    }
    if (pItem.timeLeft) itemList.push_back(pItem);
    
    for (auto &c : *vStr->getChildrenList()){
        pList = scheduleProgressCheck(&c, sPath, caltime, fullpath, cascadeAttributes);
        itemList.insert(itemList.end(), pList.begin(), pList.end());
    }

    return itemList;
}

int startTime(std::string bookedString){
    std::string::size_type i{0};
    while (bookedString[i] != '-') i++;
    return std::stoi(bookedString.substr(0, i));
}

int endTime(std::string bookedString){
    std::string::size_type i{0};
    while (bookedString[i] != '-') i++;
    return std::stoi(bookedString.substr(++i));
}

// void print(std::vector <std::array<std::string, 2>> const &a){
//    std::cout << "The vector elements are : ";
//    for(int i=0; i < a.size(); i++) std::cout << startTime(a[i][0]) << ' ';
// }

#endif