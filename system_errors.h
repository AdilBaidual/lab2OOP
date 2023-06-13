#ifndef SYSTEM_ERRORS_H
#define SYSTEM_ERRORS_H
#include <iostream>

class System_errors : public std::exception {
private:
    std::string error_message;
public:
    explicit System_errors(const std::string& message) : std::exception(), error_message{message} {} // throw System_errors("dsadasd")
    const char* what() const noexcept {
        return error_message.c_str();
    }
};

#endif // SYSTEM_ERRORS_H
