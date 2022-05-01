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
// fulfillRequirements()
// put throwError into its own namespace
// big naming scheme change
bool collide(
    verStr *gRoot,
    verStr *sPath,
    int startCeiling,
    scheduleProgress item,
    std::vector<task> scheduleBook,
    int start
);
int ultraStoi(std::string str, int def); //deprecate??
std::string stringify(int x);
std::string stdTimeRep(int timeRep);
verStr* nav(std::string fullpath, verStr* gRoot); //deprecate??
std::vector<task> initAttributes(std::vector<std::array<std::string, 2>> Attributes); //type of converter
void scheduler(MegaStr *mStr);
void schedule(verStr *gRoot, verStr *sPath, Calendar caltime);
std::vector<scheduleProgress> demote(std::vector<scheduleProgress> itemList, int loopCount);
std::array<std::vector<scheduleProgress>, 2> smartPass(std::array<std::vector<scheduleProgress>, 2> itemList);
std::array<std::vector<scheduleProgress>, 2> smartPass(std::vector<scheduleProgress> itemList, std::vector<scheduleProgress> container);
std::vector<scheduleProgress> linearization(
    verStr *vStr,
    std::string parentPath,
    attributeContainer cascadeAttributes,
    Calendar caltime //needed for the date and related info
);
std::vector<scheduleProgress> purge(std::vector<scheduleProgress> itemList, verStr* sPath);
std::vector<scheduleProgress> trim(std::vector<scheduleProgress> itemList, verStr* sPath);
void decontainerize(std::vector<scheduleProgress> itemList, verStr* sPath);

// or return a whole tail to be attached
void scheduler(MegaStr *mStr){
    bool ctr{false};
    Calendar caltime;
    for (int i{0}; i < 3; i++){
        caltime.init(i);
        //currently not fp/hybrid style
        //use mStr.forge(new vStr{} or something)
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
    //linearization, trim //last, demote, purge, smartPass, (re)Schedule, collide --implement hybrid paradigm
    //demote and smartPass need reverse tree assistance or some other mechanism probably
    //embed necessary requirement checkers into each function and a strict requirement checker


    // functions roles and responsibilities
    //linearization: to convert tree to straight vector information // done // only need to be used once
    //trim: to handle events past                                   // last // only need to be called once
    //demote: to deal with impossible requirements and prioities    // done simplified  // deals with priorities and reqs
    //purge: scrap tasks tat don't meet requirements                // done simplified  // deals with modified reqs and sPath
    //smartPass: organize tasks into easier schedulable pieces      // current focus    //avoid the ..plus marginEnd shenanigans // demote can undo this (known as shatter)// breakdown() for view
    //                                                              // deals with modified reqs // resolved by resolve
    //schedule: actually schedule the task                          // refactor         // deals with modified reqs and sPath
    //collide: use smart time alloc to maximize success rate instead of looping through until sudden success // done simplified
    bool scheduled;
    int autoStart, loopCount = 0;
    std::vector<task> scheduleBook;
    auto itemList = smartPass(
        purge(
            trim(
                linearization(
                    gRoot,
                    "",
                    attributeContainer{},
                    caltime
                ),
                sPath
            ),
            sPath
        ),
        std::vector<scheduleProgress>{}
    );

    while(!itemList[0].empty()){
        for (auto item: itemList[0]){ // go through the list of items to schedule
            // initialize all the variables
            scheduleBook = initAttributes(sPath->sortAttributes()->getAttributesList());
            //print(scheduleBook);
            scheduled = false;
            autoStart = std::stoi(item.attributes.get("start"));
            for (auto t : scheduleBook){
                // condition checks.. behaviour and dependancies
                // check if scheduling before the next task's start time
                // if true, that means autoStart is in open time
                if (autoStart > caltime.minute_t && autoStart < t.getStart()){
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
                autoStart = std::max(t.getEnd(), autoStart);
            }
            if (!scheduled){
                scheduled = collide( //scheduled unused here
                    gRoot,
                    sPath,
                    caltime.minute_t + 1,
                    item,
                    scheduleBook,
                    autoStart
                );
            }
        }
        // reinitialize itemList -list of tasks left to be scheduled
        //loopcount and sPath needs to be oncorporated into a giant object to be passed around
        itemList = smartPass(purge(demote(itemList[0], loopCount++), sPath), itemList[1]);
    }
    decontainerize(itemList[1], sPath);
}

bool collide(
    verStr *gRoot,
    verStr *sPath,
    int startCeiling,
    scheduleProgress item,
    std::vector<task> scheduleBook,
    int start
){//needs hDev and other paradigm alignment
    //initializations
    int ceilValue{startCeiling}, floorValue{1440},
        totalUsedTime{std::stoi(item.attributes.get("time")) + 1}, allocatedTime, nextHead;
    std::size_t iter{}, ceiling, floor, slider;
    // set iters
    while (iter != scheduleBook.size()){
        if (start >= scheduleBook[iter].getEnd()) iter++; //trying to schedule before
        else break;
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
            nav(scheduleBook[ceiling].getName(), gRoot)->get("strict") == "NULL" &&
            ceilValue < scheduleBook[ceiling].getStart()
        ) ceiling--;
        else break;
    }
    if (ceiling != std::string::npos)
        ceilValue = std::max(ceilValue, scheduleBook[ceiling].getEnd());

    //find bottom boulder
    while (floor != scheduleBook.size()){
        if (nav(scheduleBook[floor].getName(), gRoot)->get("strict") == "NULL") floor++;
        else break;
    }
    if (floor != scheduleBook.size()) floorValue = scheduleBook[floor].getStart();
    //calculate allocatedTime and totalUsedTime
    allocatedTime = floorValue - ceilValue;
    for (slider = ceiling + 1; slider < floor; slider++){
        totalUsedTime += 1 + scheduleBook[slider].getTimeUsed();
    }
    // if time is not enough, find LP element and chuck it
    // if lowest element is boulder, reschedule disregarding the boulder
    // chuck not implemented 
    if (allocatedTime > totalUsedTime){
        for (slider = ceiling + 1; slider < iter; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTimeUsed()),
                scheduleBook[slider].getName()
            );
            ceilValue += 1 + scheduleBook[slider].getTimeUsed();
        }

        sPath->attribute(
            stdTimeRep(ceilValue) + "-" + stdTimeRep(ceilValue + std::stoi(item.attributes.get("time"))),
            item.fullpath
        );
        ceilValue += 1 + std::stoi(item.attributes.get("time"));

        for (slider = iter; slider < floor; slider++){
            sPath->deleteAttribute(scheduleBook[slider].getFullStdTime());
            sPath->attribute(
                stdTimeRep(ceilValue) + "-" +
                stdTimeRep(ceilValue + scheduleBook[slider].getTimeUsed()),
                scheduleBook[slider].getName()
            );
            ceilValue += 1 + scheduleBook[slider].getTimeUsed();
        }
         return true;
    }
    //recalculate, if item is lowest prioirty return false
    return false;
} // 100 lines total, 45 lines long without comments and formatting

std::array<std::vector<scheduleProgress>, 2> smartPass(std::vector<scheduleProgress> itemList, std::vector<scheduleProgress> container){
    return smartPass(std::array<std::vector<scheduleProgress>, 2>{itemList, container});
}

std::array<std::vector<scheduleProgress>, 2> smartPass(std::array<std::vector<scheduleProgress>, 2> itemList){
    //need a way to resolve and find dependencies like child(), find(), etc... 
    for (auto& item : itemList[0]){
        if (item.attributes.get("timeMarginEnd") != "NULL"){
            itemList[1].push_back(item);
            item.attributes.set(
                "time",
                stringify(
                    std::stoi(item.attributes.get("time")) +
                    std::stoi(item.attributes.get("timeMarginEnd"))
                )
            );
            item.attributes.set("timeMarginEnd", "NULL");
        }
    }
    return itemList;
}

void decontainerize(std::vector<scheduleProgress> itemList, verStr* sPath){
    auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};
    auto find = [](std::vector<scheduleProgress> itemList, std::string fullpath){ //expensive!
        int iter{};
        while (iter < itemList.size()){
            if (itemList[iter].fullpath == fullpath) return iter;
            iter++;
        }
        return iter;
    };
    int iter{};

    for (auto item : itemList){
        iter = find(itemList, item.fullpath);
        sPath->deleteAttribute(scheduleBook[iter].getFullStdTime());
        sPath->attribute(stringify(scheduleBook[iter].getStart() + std::stoi(item.attributes.get("time"))), item.fullpath);
    }
}

std::vector<scheduleProgress> demote(std::vector<scheduleProgress> itemList, int loopCount){// deals with priorities and requirements
    for (int iter{0}; iter != itemList.size(); iter++){
        itemList[iter].attributes.set("priority", stringify(std::stoi(itemList[iter].attributes.get("priority")) - loopCount*36));
    }

    return itemList;
}

std::vector<scheduleProgress> purge(std::vector<scheduleProgress> itemList, verStr *sPath){// purges tasks that do not fit the requirements
    auto stringify = [](int x){
        std::stringstream ss;
        ss << x;
        return ss.str();
    }; // store converters into a namespace (module)
    auto find = [](std::vector<scheduleProgress> itemList, std::string fullpath){ //expensive!
        int iter{};
        while (iter < itemList.size()){
            if (itemList[iter].fullpath == fullpath) return iter;
            iter++;
        }
        return iter;
    };

    int ctr{};
    std::string str{};
    auto scheduleBook{initAttributes(sPath->sortAttributes()->getAttributesList())};

    for (auto t : scheduleBook){
        ctr = find(itemList, t.getName());
        if (ctr != itemList.size()){
            if (std::stoi(itemList[ctr].attributes.get("time")) < t.getTimeUsed()) sPath->deleteAttribute(t.getFullStdTime());
            else itemList[ctr].attributes.set(
                "time",
                stringify(std::stoi(itemList[ctr].attributes.get("time")) - t.getTimeUsed())
            );
        } else sPath->deleteAttribute(t.getFullStdTime());
    }

    for (int iter{(int)itemList.size() - 1}; iter > -1; iter--){
        if (std::stoi(itemList[iter].attributes.get("time")) == 0) itemList.erase(itemList.begin() + iter);
    }

    for (int iter2{(int)itemList.size() - 1}; iter2 > -1; iter2--){
        if (std::stoi(itemList[iter2].attributes.get("priority")) < 0) itemList.erase(itemList.begin() + iter2);
    }

    return itemList;
}

std::vector<scheduleProgress> trim(std::vector <scheduleProgress> itemList, verStr* sPath){
    return itemList;

    // return pack{itemlist, sPath};
}

std::vector<scheduleProgress> linearization(
    verStr *vStr,
    std::string parentPath,
    attributeContainer cascadeAttributes,
    Calendar caltime
){// supports type, date, time
    //lambda function initialization
    auto pass = [](std::string str, std::string defaultStr){
        if (str == "NULL") return defaultStr;
        return str;
    };
    auto cascadeValueInit = [](
        std::string attributeValue,
        std::string cascadeValue,
        std::string defaultStr
    ){
        if (attributeValue != "NULL") return attributeValue;
        if (cascadeValue == "NULL") return defaultStr;
        return cascadeValue;
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
    std::vector<scheduleProgress> itemList, pList;
    //non-cascade attributes
    std::string
        time{pass(vStr->get("time"), "30")},
        start{pass(vStr->get("start"), "1")},
        timeMarginEnd{pass(vStr->get("timeMarginEnd"), "0")};
    //cascade attributes
    //No trash value handling ***WARNING***
    cascadeAttributes.set(
        "type",
        cascadeValueInit(
            vStr->get("type"),
            cascadeAttributes.get("type"),
            "task"
        )
    )->set(
        "date",
        cascadeValueInit(
            vStr->get("date"),
            cascadeAttributes.get("date"),
            caltime.baseDate
        )
    )->set(
        "priority",
        cascadeValueInit(
            vStr->get("priority"),
            cascadeAttributes.get("priority"),
            "1000"
        )
    );
    //apply attributes to item
    item.attributes.set(std::string{"time"}, time
    )->set(std::string{"start"}, start
    )->set(std::string{"type"}, cascadeAttributes.get("type")
    )->set(std::string{"date"}, cascadeAttributes.get("date")
    )->set(std::string{"priority"}, cascadeAttributes.get("priority")
    )->set(std::string("timeMarginEnd"), cascadeAttributes.get("timeMarginEnd"));
    //store item into itemList
    itemList.push_back(item);
    //attach childLists into itemList
    for (auto &c : *vStr->getChildrenList()){
        pList = linearization(&c, fullpath, cascadeAttributes, caltime);
        itemList.insert(itemList.end(), pList.begin(), pList.end());
    }

    return itemList;
}

int ultraStoi(std::string str, int def){
    if (str == "NULL") return def;
    return std::stoi(str);
};

std::string stringify(int x){
    std::stringstream ss;
    ss << x;
    return ss.str();
};

std::string stdTimeRep(int timeRep){
    std::stringstream ss;
    ss << timeRep / 60 << ":" << timeRep % 60;
    return ss.str();
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
// void print(std::vector<task> scheduleBook){
//     for (auto t : scheduleBook)
//         std::cout << t.getName() << ": " << t.getFullStdTime() << "\n";
//     std::cout << "\n";
// }

#endif