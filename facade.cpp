#include "facade.h"

std::string Facade::read_operation(const std::string& fp) {
    return file_worker->parse_file(fp);
}

void Facade::check_operation(const std::string& ft) {
    json_cheker->check_json(ft);
}

Facade::Facade(Work_with_file *wf, Json_checker *jc){
    this->file_worker = wf;
    this->json_cheker = jc;
}
