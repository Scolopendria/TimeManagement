#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <array>
#include <functional>

class verStr{
    private:
        // utilities class
        void throwError(std::string err);
        std::string read(std::string s, std::string::size_type &i, bool t);
        std::string cut(std::string::size_type i[2]);
        // standard class
        verStr* createAttribute(std::string ID_pair[2]);
        verStr* createChild(std::string childString);
        // initializer class
        verStr* strip();
        verStr* objectify();
        // members
        std::string String{};
        std::string Name{};
        std::vector<verStr> children{};
        std::vector<std::array<std::string, 2>> attributes{};
        verStr* parent{};
    public:
        verStr(std:: string s);
        verStr(std:: string s, verStr* parentNode);
        // view class
        std::string getName();
        std::string format();
        std::string getView();
        // command class
        verStr* attribute(std::string attrName, std::string attrValue);
        verStr* child(std::string childName, bool &t);
        void deleteAttribute(std::string attrName);
        void deleteChild(std::string childName);
        verStr* getParent();
        //internal command class
        std::string updString();
        verStr* sortAttributes();
        std::vector<std::array<std::string, 2>> getAttributesList();
        std::vector<verStr>* getChildrenList();
        std::string get(std::string attrName);
};

class MegaStr{
    private:
        std::string getFile();
    public:
        MegaStr();
        ~MegaStr();
        verStr vStr{getFile()};
};

class Calendar{
    public:
        Calendar();
        void init(int offset);
        int year{};
        int month{};
        int date{};
        int weekday{};
        int minute_t{};
        std::string strDate{};
        std::string baseDate{};
};

class attributeContainer{
    private:
        std::vector<std::array<std::string, 2>> cascadeAttributes{};
    public:
        std::string get(std::string name);
        attributeContainer* set(std::array<std::string, 2> ID_pair);
        attributeContainer* set(std::string name, std::string value); //Overload
};

class scheduleProgress{
    public:
        scheduleProgress(std::string fullpath__);
        std::string fullpath{};
        attributeContainer attributes{};
};

class task{
    private:
        int startTime;
        int endTime;
        int timeUsed;
        std::string name;
        std::string stdTime;
    public:
        task(std::string taskName, int start, int end);
        int getStart();
        int getEnd();
        int getTimeUsed();
        std::string getName();
        std::string getFullStdTime();
};

class switchPair{
    private:
        std::string name{};
        std::function<void()> func{nullptr};
    public:
        switchPair(std::string term, std::function<void()> command);
        bool query(std::string term);
};

class strSwitch{
    private:
        std::function<void()> func{nullptr};
        std::vector<switchPair> list{};
    public:
        strSwitch(std::function<void()> defaultFunction);
        strSwitch* add(switchPair element);
        void match(std::string term);
};



#include "VerStr.cpp"
#include "MegaStr.cpp"
#include "SchedClasses.cpp"
#include "strSwitch.cpp"

#endif