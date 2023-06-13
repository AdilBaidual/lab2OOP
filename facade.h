#ifndef FACADE_H
#define FACADE_H
#include <iostream>
#include <memory>
#include "work_with_file.h"
#include "json_checker.h"

class Facade {
private:
    Work_with_file *file_worker;
    Json_checker *json_cheker;
public:
    explicit Facade(Work_with_file *wf, Json_checker *jc);
    ~Facade(){
        if(file_worker != nullptr)
            delete file_worker;
        if(json_cheker != nullptr)
            delete json_cheker;
    }
    std::string fp;
    std::string read_operation(const std::string& fp);
    void check_operation(const std::string& ft);
    void rollback(){json_cheker->rollback();}
};

#endif // FACADE_H
