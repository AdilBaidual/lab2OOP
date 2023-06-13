#ifndef JSON_CHECKER_H
#define JSON_CHECKER_H

#include <memory>
#include <iostream>
#include "Json_errors.h"
#include <vector>

class Json_checker {
private:
    std::string error_message;
    std::string json_text;
    std::string correct_symbols = "-.0123456789";
    int string_index;
    int pos_in_file;
    int pos_in_line;
    bool is_okay;
    void skip_empty_space(const std::string& json_text);
    void check_object(const std::string& json_text);
    void check_string(const std::string& json_text);
    void check_special_type(const std::string &json_text);
    void check_number(const std::string& json_text);
    void check_array(const std::string& json_text);
public:
    Json_checker();
    Json_checker(const std::string& json_txt);
    void rollback();
    void check_json(const std::string &json_text);
    std::string assign_error();
};

class Expected_part
{
public:
    std::string current;
    std::string key = "ключ";
    std::string colon = "двоеточие";
    std::string value = "значение";
    std::string end_of_line = ",";
    std::string end_of_object = "}";
};

#endif // JSON_CHECKER_H
