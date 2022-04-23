#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include "Classes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <experimental/random>

//void print(std::vector<task> scheduleBook);
// introduce some task-splitting one-time-use functions to help organize code
// put converters in a namespace
// use more hierachy task organization
// better use of lambda functions
// consider rewrite
// std::vector<scheduleProgress> progress(output of linearization)
// fulfillRequirements()
// put throwError into its own namespace
std::string stdTimeRep(int timeRep);
verStr* nav(std::string fullpath, verStr* gRoot);
int ultraStoi(std::string str, int def);
std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes);
void scheduler(MegaStr *mStr);
void schedule(verStr *gRoot, verStr *sPath, Calendar caltime);
bool collide(
    verStr *gRoot,
    verStr *sPath,
    int startCeiling,
    scheduleProgress item,
    std::vector<task> scheduleBook,
    int start
);
std::vector<scheduleProgress> linearization(
    verStr *vStr,
    verStr *sPath,
    Calendar caltime,
    std::string parentPath,
    std::vector<std::array<std::string, 2>> cascadeAttributes
);
std::vector<scheduleProgress> purge(std::vector<scheduleProgress>);

// maybe shift to embedded function in megaStr
// or return a whole tail to be attached
void scheduler(MegaStr *mStr){
    bool ctr{false};
    Calendar caltime;
    for (int i{0}; i < 3; i++){
        caltime.init(i);
        //currently not fp/hybrid style
        for (auto &c : *mStr->vStr.getChildrenList()){
            schedule(
                c.child("Goals", ctr),
                c.child("Schedule", ctr)->child(caltime.strDate, ctr),
                caltime
            );
        }
    }
}


void schedule(verStr *gRoot, verStr *sPath, Calendar caltime){
    // cascadeAttributes = off, rescheduling = off, autoDeclarations = off
    // decides when to schedule in a day
    bool scheduled;
    int autoStart;
    std::vector<task> scheduleBook;
    auto itemList = purge(
        linearization(
            gRoot,
            sPath,
            caltime,
            std::string{},
            std::vector<std::array<std::string, 2>>{}
        ))
    ;

    // Auto mark past tasks as "done"
    // log to history
    // mark chore as done -prevent rescheduling (something like "done"="<date>") tells
    // atm to ignore task
    // Wipe unwanted scheduled tasks (first?)

    while(!itemList.empty()){
        for (auto item: itemList){ // go through the list of items to schedule
            // initialize all the variables
            scheduleBook = initAttributes(sPath->sortAttributes()->getAttributesList());
            //print(scheduleBook);
            scheduled = false;
            autoStart = 1;
            
            if (nav(item.fpath, gRoot)->get("start") != "NULL"){
                scheduled = collide(
                    gRoot,
                    sPath,
                    std::max( //hDev
                        caltime.minute_t + 1,
                        std::stoi(nav(item.fpath, gRoot)->get("start"))
                    ),
                    item,
                    scheduleBook,
                    std::stoi(nav(item.fpath, gRoot)->get("start"))
                );
            } else { // set autoStart
                for (auto t : scheduleBook){
                    if (autoStart > caltime.minute_t){
                        // condition checks.. behaviour and dependancies
                        // check if scheduling before the next task's start time
                        // if true, that means autoStart is in open time
                        if (autoStart < t.getStart()){
                            scheduled = collide(
                                gRoot,
                                sPath,
                                caltime.minute_t + 1,
                                item,
                                scheduleBook,
                                autoStart
                            );
                            if (scheduled) break;
                        }
                    }
                    autoStart =
                        t.getEnd() +
                        ultraStoi(nav(t.getName(), gRoot)->get("timeMarginEnd"), 0);
                }
                if (!scheduled){
                    scheduled = collide(
                        gRoot,
                        sPath,
                        caltime.minute_t + 1,
                        item,
                        scheduleBook,
                        autoStart
                    );
                }
            }
        }
        // reinitialize itemList -list of tasks left to be scheduled
        itemList = linearization(
            gRoot,
            sPath,
            caltime,
            std::string{},
            std::vector<std::array<std::string, 2>>{}
        );
    }
}

bool collide(
    verStr *gRoot,
    verStr *sPath,
    int startCeiling,
    scheduleProgress item,
    std::vector<task> scheduleBook,
    int start
){
    //initializations
    int ceilValue{startCeiling}, floorValue{1440},
        totalUsedTime{item.timeLeft + 1}, allocatedTime, nextHead;
    std::size_t iter{0}, ceiling, floor, slider;
    // set iters
    while (iter != scheduleBook.size()){
        if (
            start >
            scheduleBook[iter].getEnd() +
            ultraStoi(nav(scheduleBook[iter].getName(), gRoot)->get("timeMarginEnd"), 0)
        ) { iter++; } else break;
    }
    floor = iter;
    ceiling = iter;
    if (iter == scheduleBook.size()) nextHead = 1440;
    else nextHead = scheduleBook[ceiling].getStart();
    // check to see if nextHead is greater than start
    // if true, that means scheduling is in open space
    // and ceiling is set to the item above it
    if (nextHead > start) ceiling--;
    //find top boulder
    while (ceiling != std::string::npos){
        if (
            nav(scheduleBook[ceiling].getName(), gRoot)->get("start") == "NULL" &&
            ceilValue < scheduleBook[ceiling].getStart()
        ) ceiling--;
        else break;
    }
    if (ceiling != std::string::npos){
        ceilValue = std::max( //hDev
            ceilValue,
            scheduleBook[ceiling].getEnd() +
            ultraStoi(nav(scheduleBook[ceiling].getName(), gRoot)->get("timeMarginEnd"), 0)
        );
    }
    //find bottom boulder
    while (floor != scheduleBook.size()){
        if (nav(scheduleBook[floor].getName(), gRoot)->get("start") == "NULL") floor++;
        else break;
    }
    if (floor != scheduleBook.size()) floorValue = scheduleBook[floor].getStart();
    //calculate allocatedTime and totalUsedTime
    allocatedTime = floorValue - ceilValue;
    for (slider = ceiling + 1; slider < floor; slider++){
        totalUsedTime +=
            1 + scheduleBook[slider].getTime() +
            ultraStoi(nav(scheduleBook[slider].getName(), gRoot)->get("timeMarginEnd"), 0);
    }
    // if time is not enough, find LP element and chuck it
    // if lowest element is boulder, reschedule disregarding the boulder
    // chuck not implemented 
    if (allocatedTime > totalUsedTime){
        for (slider = ceiling + 1; slider < iter; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTime()),
                scheduleBook[slider].getName()
            );
            ceilValue +=
                1 + scheduleBook[slider].getTime() +
                ultraStoi(nav(scheduleBook[slider].getName(), gRoot)->get("timeMarginEnd"), 0);
        }

        sPath->attribute(
            stdTimeRep(ceilValue) + "-" + stdTimeRep(ceilValue + item.timeLeft),
            item.fpath
        );
        ceilValue +=
            1 + item.timeLeft +
            ultraStoi(nav(item.fpath, gRoot)->get("timeMarginEnd"), 0);

        for (slider = iter; slider < floor; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTime()),
                scheduleBook[slider].getName()
            );
            ceilValue +=
                1 + scheduleBook[slider].getTime() +
                ultraStoi(nav(scheduleBook[slider].getName(), gRoot)->get("timeMarginEnd"), 0);
        }
        return true;
    }
    //recalculate, if item is lowest prioirty return false
    return false;
} // 100 lines total, 45 lines long without comments and formatting

std::vector<scheduleProgress> linearization(
    verStr *vStr,
    verStr *sPath,
    Calendar caltime,
    std::string parentPath,
    std::vector<std::array<std::string, 2>> cascadeAttributes
){ // decides whether to schedule for a day
    // lambda func initialization
    auto pass = [](std::string str, std::string defStr){
        if (str == "NULL") return defStr;
        return str;
    };
    // initialize fullpath
    std::string fullpath{
        [parentPath, vStr]{
            if (parentPath == "") return vStr->getName();
            return parentPath + ":" + vStr->getName();
        }()
    };
    // general initializations
    scheduleProgress item{fullpath};
    item.timeLeft = ultraStoi(vStr->get("time"), 30);
    bool 
        repeatCondition{false},
        schedule{ultraStoi(vStr->get("priority"), 95) > std::experimental::randint(0, 99)};
        
    std::string 
        taskType{pass(vStr->get("type"), "task")},
        taskDate{pass(vStr->get("date"), caltime.baseDate)};
    std::vector<scheduleProgress> itemList, pList;
    auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};

    /*
    * Refactor all things that require calculation to purge
    * Set and pass auto declarations and also pass special dependency trees and ghost declarations
    *
    * 
    */
    // schedule progress (timeLeft) calculations also functions as wipeout currently
    for (auto t : scheduleBook){
        if (t.getName() == fullpath){
            if (item.timeLeft < t.getTime()) sPath->deleteAttribute(t.getFullStdTime());
            else item.timeLeft -= t.getTime();
        }
    }
    
    // condition checks.. figure whether to schedule task for given date
    if (schedule){
        if (taskType == "task"){
            if (taskDate == caltime.strDate){
                if (item.timeLeft > 0) itemList.push_back(item);
            }
        } else if (taskType == "chore") {
            if (repeatCondition){
                if (item.timeLeft > 0) itemList.push_back(item);
            }
        } else {
            std::cout << "Error type" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    // insert child Itemlist into itemList 
    for (auto &c : *vStr->getChildrenList()){
        pList = linearization(&c, sPath, caltime, fullpath, cascadeAttributes);
        itemList.insert(itemList.end(), pList.begin(), pList.end());
    }

    return itemList;
}

std::vector<scheduleProgress> purge(std::vector<scheduleProgress>){

}

std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes){
    std::vector<task> scheduleBook;
    std::string::size_type i;
    auto minuteTimeRep = [](std::string timeRep){
        std::string::size_type iter{0};
        while (timeRep[iter] != ':') iter++;
        return std::stoi(timeRep.substr(0, iter)) * 60 + std::stoi(timeRep.substr(iter + 1));
    };

    for (auto ID_pair : Attributes){
        i = 0;
        while (ID_pair[0][i] != '-') i++;
        scheduleBook.push_back(
            task{
                ID_pair[1],                             //fullpath
                minuteTimeRep(ID_pair[0].substr(0, i)), //start
                minuteTimeRep(ID_pair[0].substr(i + 1)) //end
            }
        );
    }
    
    return scheduleBook;
}

int ultraStoi(std::string str, int def){
    if (str == "NULL") return def;
    return std::stoi(str);
};

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

// void print(std::vector<task> scheduleBook){
//     for (auto t : scheduleBook)
//         std::cout << t.getName() << ": " << t.getFullStdTime() << "\n";
//     std::cout << "\n";
// }

#endif