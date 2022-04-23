#ifndef STRSWITCH_CPP
#define STRSWITCH_CPP

#include "Classes.h"

// to be improved

strSwitch::strSwitch(std::function<void()> defaultFunction){
    func = defaultFunction;
}

strSwitch* strSwitch::add(switchPair element){
    list.push_back(element);
    return this;
}

void strSwitch::match(std::string term){
    for (auto element : list)
        if (element.query(term)) return;
    func();
}

switchPair::switchPair(std::string term, std::function<void()> command){
    name = term;
    func = command;
}

bool switchPair::query(std::string term){
    if (term == name){ func(); return true; }
    return false;
}

#endif