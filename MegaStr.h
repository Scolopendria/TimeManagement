#include <string>
#include <fstream>

class MegaStr {
    public:
        MegaStr(){
            file.open("System.txt");
            if (file.is_open()){
               ;
            }
        }
    private:
        std::string str{""};
        std::ifstream  file;
};