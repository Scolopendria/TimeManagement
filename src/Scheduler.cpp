#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes);
std::string stdTimeRep(int timeRep);
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
    auto itemList = scheduleProgressCheck(
        gRoot,
        sPath,
        caltime,
        "",
        std::vector<std::array<std::string, 2>>{}
    );

    // Wipe unwanted scheduled tasks
    while(!itemList.empty()){
        for (auto &item: itemList){
            startSearchTime = caltime.minute_t;
            tail = 0;
            iter = 0;
            auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};

            while (true){
                // condition checks.. behaviour and dependancies
                if (iter == scheduleBook.size()){
                    if (startSearchTime + item.timeLeft < 1440){
                        sPath->attribute(
                            stdTimeRep(startSearchTime) + "-" + stdTimeRep(startSearchTime + item.timeLeft),
                            item.fpath
                        );
                        std::cout << "Successful scheduling." << std::endl;
                    } else std::cout << "Unable to schedule." << std::endl;
                    break;
                }
                
                if (startSearchTime > tail && startSearchTime < scheduleBook[iter].getStart()){
                    if (startSearchTime + item.timeLeft < scheduleBook[iter].getStart()){
                        sPath->attribute(
                            stdTimeRep(startSearchTime) + "-" + stdTimeRep(startSearchTime + item.timeLeft),
                            item.fpath
                        );
                        std::cout << "Successful scheduling." << std::endl;
                    } else startSearchTime = scheduleBook[iter].getEnd();
                }
                
                if (
                    startSearchTime >= scheduleBook[iter].getStart() &&
                    startSearchTime <= scheduleBook[iter].getEnd()
                ) startSearchTime = scheduleBook[iter].getEnd() + 1;
                
                tail = scheduleBook[iter].getEnd();
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
    if (vStr->get("time") != "NULL") pItem.timeLeft = std::stoi(vStr->get("time"));
    // assign time to stated value
    // condition checks.. figure whether to schedule task for given date
    
    // schedule progress (time) calculations
    auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};
    for (auto t : scheduleBook){
        if (t.getName() == fullpath){
            if (pItem.timeLeft + t.getStart() - t.getEnd() < 0)
                sPath->deleteAttribute(t.getFullStdTime());
            else pItem.timeLeft = pItem.timeLeft + t.getStart() - t.getEnd();
        }
    }
    
    if (pItem.timeLeft) itemList.push_back(pItem);
    for (auto &c : *vStr->getChildrenList()){
        pList = scheduleProgressCheck(&c, sPath, caltime, fullpath, cascadeAttributes);
        itemList.insert(itemList.end(), pList.begin(), pList.end());
    }

    return itemList;
}

std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes){
    std::vector<task> scheduleBook;
    auto minuteTimeRep = [](std::string timeRep){
        std::string::size_type iter{0};
        while (timeRep[iter] != ':') iter++;
        return std::stoi(timeRep.substr(0, iter)) * 60 + std::stoi(timeRep.substr(iter + 1));
    };

    for (auto &ID_pair : Attributes){
        std::string::size_type i{0};
        while (ID_pair[0][i] != '-') i++;
        const int start{minuteTimeRep(ID_pair[0].substr(0, i))};
        const int end{minuteTimeRep(ID_pair[0].substr(i + 1))};
        scheduleBook.push_back(task{ID_pair[1], start, end});
    }
    
    return scheduleBook;
}

std::string stdTimeRep(int timeRep){
    std::stringstream ss;
    ss << timeRep / 60 << ":" << timeRep % 60;
    return ss.str();
}

// void print(std::vector <std::array<std::string, 2>> const &a){
//    std::cout << "The vector elements are : ";
//    for(int i=0; i < a.size(); i++) std::cout << startTime(a[i][0]) << ' ';
// }

#endif