#include "json_checker.h"
#include <QDebug>

#define ERROR_IN_STRING "incorrect string"
#define ERROR_IN_SPECIAL "incorrect special type"
#define ERROR_IN_NUMBER "incorrect number"

Json_checker::Json_checker() {
    rollback();
}

void Json_checker::rollback() {
    this->string_index = 0;
    this->pos_in_file = 1;
    this->pos_in_line = 1;
    this->is_okay = true;
    error_message = "File is correct!";
}

void Json_checker::check_json(const std::string& json_text) {
    skip_empty_space(json_text);
    if (json_text[string_index] == '"')
        check_string(json_text);
    else if (json_text[string_index] == '[')
        check_array(json_text);
    else if (json_text[string_index] == '{')
        check_object(json_text);
    else if (json_text[string_index] == 'f' || json_text[string_index] == 't' || json_text[string_index] == 'n')
        check_special_type(json_text);
    else if (correct_symbols.find(json_text[string_index]) != std::string::npos)
        check_number(json_text);
    else {
        is_okay = false;
        throw Json_errors("incorrect elements", "correct elements", pos_in_file, pos_in_line);
    }
    if (is_okay == true) {
        skip_empty_space(json_text);
        if (json_text[string_index])
            throw Json_errors("more than 1 root element", "EOF", pos_in_file, pos_in_line);
    }
}

void Json_checker::check_string(const std::string& json_text) {
    string_index++;
    pos_in_line++;
    while (json_text[string_index] != '"') {
        if (!json_text[string_index] || json_text[string_index] == '\n') {
            is_okay = false;
            throw Json_errors(ERROR_IN_STRING, "\"", pos_in_file, pos_in_line);
            break;
        }
        string_index++;
        pos_in_line++;
    }
    string_index++;
    pos_in_line++;
}

void Json_checker::check_special_type(const std::string& json_text) {
    std::string expected_word;
    if (json_text[string_index] == 't')
        expected_word = "true";
    else if (json_text[string_index] == 'f')
        expected_word = "false";
    else
        expected_word = "null";
    for (int i = 0; expected_word[i]; i++) {
        if (expected_word[i] != json_text[string_index]) {
            is_okay = false;
            throw Json_errors(ERROR_IN_SPECIAL, expected_word, pos_in_file, pos_in_line);
            break;
        }
        string_index++;
        pos_in_line++;
    }
    string_index++;
    pos_in_line++;
}

void Json_checker::check_number(const std::string& json_text) {
    int count_of_points = 0;
    int count_of_minus = 0;
    std::string prev;
    while (correct_symbols.find(json_text[string_index]) != std::string::npos) {
        if(!prev.empty() && ((prev + json_text[string_index]) == ".-" || (prev + json_text[string_index]) == "-.")){
            is_okay = false;
            throw Json_errors("incorrect number", "number with correct signs", pos_in_file, pos_in_line);
            break;
        }

        if (json_text[string_index] == '.')
            count_of_points++;
        if (count_of_points > 1) {
            is_okay = false;
            throw Json_errors("incorrect number", "number with one point", pos_in_file, pos_in_line);
            break;
        }
        if (json_text[string_index] == '-')
            count_of_minus++;
        if (count_of_minus > 1) {
            is_okay = false;
            throw Json_errors("incorrect number", "number with one minus", pos_in_file, pos_in_line);
            break;
        }
        prev = json_text[string_index];
        string_index++;
        pos_in_line++;
    }
}

void Json_checker::check_array(const std::string& json_text) {
    if (is_okay == false) {
        return;
    }
    string_index++;
    pos_in_line++;
    bool expect_value = true;
    skip_empty_space(json_text);
    if (json_text[string_index] == ']') {
        string_index++;
        pos_in_line++;
        return;
    }
    while (json_text[string_index] && is_okay) {
        skip_empty_space(json_text);
        if (!expect_value && json_text[string_index] == ']')
            break;
        else if (!expect_value && json_text[string_index] == ',') {
            string_index++;
            pos_in_line++;
            expect_value = true;
        }
        else if (expect_value && json_text[string_index] == '"') {
            check_string(json_text);
            expect_value = false;
        }
        else if (expect_value && json_text[string_index] == '[') {
            check_array(json_text);
            expect_value = false;
        }
        else if (expect_value && json_text[string_index] == '{') {
            check_object(json_text);
            expect_value = false;
        }
        else if (expect_value && (json_text[string_index] == 'f' || json_text[string_index] == 't' || json_text[string_index] == 'n')) {
            check_special_type(json_text);
            expect_value = false;
        }
        else if (expect_value && correct_symbols.find(json_text[string_index]) != std::string::npos) {
            check_number(json_text);
            expect_value = false;
        }
        else if (expect_value && (json_text[string_index] == ',' || json_text[string_index] == ']')) {
            is_okay = false;
            throw Json_errors("incorrect array", "array element", pos_in_file, pos_in_line);
        }
        else if (!expect_value && json_text[string_index] != ',') {
            is_okay = false;
            throw Json_errors("incorrect array", "]", pos_in_file, pos_in_line);
        }
        else {
            is_okay = false;
            throw Json_errors("incorrect array", "end of array", pos_in_file, pos_in_line);
        }
    }
    if (is_okay == true) {
        string_index++;
        pos_in_line++;
    }
}

void Json_checker::check_object(const std::string& json_text) {
    if (is_okay == false) {
        return;
    }
    string_index++;
    skip_empty_space(json_text);
    Expected_part part;
    part.current = part.key;
    if (json_text[string_index] == '}') {
        string_index++;
        pos_in_line++;
        return;
    }
    while (json_text[string_index] && is_okay == true) {
        skip_empty_space(json_text);
        if (part.current == part.key && json_text[string_index] == '"') {
            check_string(json_text);
            part.current = part.colon;
        }
        else if (part.current == part.colon && json_text[string_index] == ':') {
            part.current = part.value;
            string_index++;
            pos_in_line++;
        }
        else if (part.current == part.value && json_text[string_index] == '"') {
            check_string(json_text);
            part.current = part.end_of_line;
        }
        else if (part.current == part.value && correct_symbols.find(json_text[string_index]) != std::string::npos) {
            check_number(json_text);
            part.current = part.end_of_line;
        }
        else if (part.current == part.value && (json_text[string_index] == 'f' || json_text[string_index] == 't' || json_text[string_index] == 'n')) {
            check_special_type(json_text);
            part.current = part.end_of_line;
        }
        else if (part.current == part.value && json_text[string_index] == '{') {
            check_object(json_text);
            part.current = part.end_of_line;
        }
        else if (part.current == part.value && json_text[string_index] == '[') {
            check_array(json_text);
            part.current = part.end_of_line;
        }
        else if (part.current == part.end_of_line && json_text[string_index] == ',') {
            string_index++;
            pos_in_line++;
            part.current = part.key;
        }
        else if (part.current == part.end_of_line && json_text[string_index] == '\n') {
            string_index++;
            pos_in_file++;
            pos_in_line = 1;
            part.current = part.end_of_object;
        }
        else if (part.current == part.end_of_object && json_text[string_index] == '}') {
            break;
        }
        else if (part.current == part.end_of_line && json_text[string_index] == '}') {
            break;
        }
        else {
            is_okay = false;
            throw Json_errors("incorrect object", part.current, pos_in_file, pos_in_line);
        }
    }
    if (is_okay == false)
        return;
    string_index++;
    pos_in_line++;
}

void Json_checker::skip_empty_space(const std::string& json_text) {
    while (json_text[string_index] == ' ' || json_text[string_index] == '\n' || json_text[string_index] == '\t') {
        if (json_text[string_index] == '\n') {
            pos_in_file++;
            pos_in_line = 1;
        }
        else {
            pos_in_line++;
        }
        string_index++;
    }
}




