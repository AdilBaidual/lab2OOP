#include "work_with_file.h"

void Work_with_file::read_file(const std::string& filename) {
    std::ifstream fs;
    fs.open(filename);
    if (!fs.is_open()) {
        throw System_errors("File was not opened");
    }
    std::string line;
    while (!fs.eof()) {
        getline(fs, line, '\r');
        file_text += line;
    }
}

const std::string& Work_with_file::parse_file(const std::string& filename) {
    if (!file_text.empty()) {
         file_text.clear();
         read_file(filename);
         return file_text;
    }
    else {
        read_file(filename);
        return file_text;
    }
}
