#ifndef WORK_WITH_FILE_H
#define WORK_WITH_FILE_H
#include <iostream>
#include <fstream>
#include "system_errors.h"


class Work_with_file {
private:
    std::string file_text;
public:
    void read_file(const std::string& filename);
    const std::string& parse_file(const std::string& filename);
};

#endif // WORK_WITH_FILE_H
