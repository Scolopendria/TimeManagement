// Main of TM

#include <iostream>
#include <fstream>
#include <sstream>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;

string readFileIntoString2(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int main(){
    string filename("system.txt");
    string file_contents;

    file_contents = readFileIntoString2(filename);
    cout << file_contents << endl;

    exit(EXIT_SUCCESS);
}