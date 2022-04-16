#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <array>

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
        std::string String;
        std::string Name;
        std::vector<verStr> children;
        std::vector<std::array<std::string, 2>> attributes;
        verStr* parent;
    public:
        verStr(std:: string s);
        verStr(std:: string s, verStr* parentNode);
        // view class
        std::string getName();
        std::string format();
        std::string get(bool __ATTRIBUTES, bool __CHILDREN);
        std::string get(std::string attrName);
        // command class
        void attribute(std::string attrName, std::string attrValue);
        verStr* child(std::string childName, bool &t);
        void deleteAttribute(std::string attrName);
        void deleteChild(std::string childName);
        verStr* getParent();
        //internal command class
        std::string updString();
        verStr* sortAttributes();
        std::vector<std::array<std::string, 2>> getAttributesList();
        std::vector<verStr>* getChildrenList();
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
        void init(int offset);
        int year;
        int month;
        int date;
        int weekday;
        int minute_t;
        std::string strDate;
};

class scheduleProgress{
    private:
        std::string name; // deprecate
        std::string path; // deprecate
    public:
        scheduleProgress(std::string fullpath);
        int timeLeft{30};
        std::string fpath{""};
};

class task{
    private:
        int startTime;
        int endTime;
        std::string name;
        std::string stdTime;
    public:
        task(std::string taskName, int start, int end);
        int getStart();
        int getEnd();
        std::string getName();
        std::string getFullStdTime();
};

#include "VerStr.cpp"
#include "MegaStr.cpp"
#include "SchedClasses.cpp"

#endif