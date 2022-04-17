#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>


std::string stdTimeRep(int timeRep);
verStr* nav(std::string fullpath, verStr* gRoot);
std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes);
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
    // decides when to schedule in a day
    int startSearchTime, tail;
    std::size_t iter;
    auto itemList = scheduleProgressCheck(
        gRoot,
        sPath,
        caltime,
        "",
        std::vector<std::array<std::string, 2>>{}
    );

    // Auto mark past tasks as "done"
    // log to history
    // mark chore as done -prevent rescheduling (something like "done"="<date>") tells atm to ignore task
    // Wipe unwanted scheduled tasks (first?)
    while(!itemList.empty()){
        for (auto &item: itemList){
            // initialize all the varibles
            startSearchTime = caltime.minute_t;
            tail = 0;
            iter = 0;
            auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};

            if(true);
            while (true){
                // condition checks.. behaviour and dependancies
                // check if scheduling after last task
                std::cout << nav(item.fpath, gRoot)->getName() << std::endl;
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
                // check if scheduling in open time
                if (startSearchTime > tail && startSearchTime < scheduleBook[iter].getStart()){
                    if (startSearchTime + item.timeLeft < scheduleBook[iter].getStart()){
                        sPath->attribute(
                            stdTimeRep(startSearchTime) + "-" + stdTimeRep(startSearchTime + item.timeLeft),
                            item.fpath
                        );
                        std::cout << "Successful scheduling." << std::endl;
                    } else startSearchTime = scheduleBook[iter].getEnd();
                }
                // check if scheduling in another task's time
                if (
                    startSearchTime >= scheduleBook[iter].getStart() &&
                    startSearchTime <= scheduleBook[iter].getEnd()
                ) startSearchTime = scheduleBook[iter].getEnd() + 1;
                // increment
                tail = scheduleBook[iter].getEnd();
                iter++;
            }
        }
        // reinitialize itemList -list of tasks left to be scheduled
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
){ // decides whether to schedule for a day
    // initialize pItem and placeholder values
    if (fullpath != "") fullpath += ":";
    fullpath += vStr->getName();
    scheduleProgress pItem{fullpath};
    std::vector<scheduleProgress> itemList, pList;
    // default values initialization
    bool ctr{false};
    if (vStr->get("time") != "NULL") pItem.timeLeft = std::stoi(vStr->get("time"));
    if (vStr->get("timeMarginEnd") != "NULL"){
        vStr->child("timeMarginEnd", ctr)->
            attribute("time", vStr->get("timeMarginEnd"))->
            attribute("startCondition", "immediatelyAfter parent");
    }
    // condition checks.. figure whether to schedule task for given date
    
    // schedule progress (time) calculations
    auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};
    for (auto t : scheduleBook){
        if (t.getName() == fullpath){
            if (pItem.timeLeft + t.getStart() - t.getEnd() < 0)
                sPath->deleteAttribute(t.getFullStdTime());
            else pItem.timeLeft += (t.getStart() - t.getEnd());
        }
    }
    // store Item into itemList and insert itemList of child Itemlists
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

verStr* nav(std::string fullpath, verStr* gRoot){
    bool ctr{false};
    std::string path{fullpath};
    std::string::size_type i{0};
    verStr* vStr{gRoot};
    //Special Adjustment for root ("Goals")
    while (path[i] != ':' && i < path.length()) i++;
    if (i == path.length()) return vStr;
    path = path.substr(i + 1);

    while (true){
        i = 0;
        while (path[i] != ':' && i < path.length()) i++;
        if (i == path.length()) return vStr->child(path, ctr);
        vStr = vStr->child(path.substr(0, i), ctr);
        path = path.substr(i + 1);
    }

    return vStr;
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