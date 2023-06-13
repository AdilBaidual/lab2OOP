#ifndef JSON_ERRORS_H
#define JSON_ERRORS_H
#include <iostream>

class Json_errors: public std::exception {
private:
    mutable std::string error_message;
    std::string info;
    std::string expected_str;
    int pos_in_file;
    int pos_in_line;
    void set_error_message(const std::string& message) const {
        error_message = message;
    }
public:
    explicit Json_errors(const std::string& info, const std::string& expected_str, const int pos_in_file, const int pos_in_line):
        std::exception(), info{info}, expected_str{expected_str}, pos_in_file{pos_in_file}, pos_in_line{pos_in_line}{}
    const char* what() const noexcept {
        std::string message = "Error: " + info + " in position: " + std::to_string(pos_in_file) + ", " + std::to_string(pos_in_line) + ". " + "Expected: " + expected_str + " .";
        set_error_message(message);
        return error_message.c_str();
    }
};

#endif // JSON_ERRORS_H
