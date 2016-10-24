#ifndef UI_H
#define UI_H

#include <form.h>

#include <vector>
#include <string>

const int KEY_RETURN = 10;
const int KEY_TAB = 9;
const int KEY_ESCAPE = 27;
const int KEY_QUIT = 17;

class Ui
{
public:
    Ui();
    ~Ui();
    
    FORM *form;
    FIELD *fields[17];
    
    const std::vector<std::string> fields_name =
    {
        "HOST",
        "PORT",
        "USERNAME",
        "PASSWORD",
        "       LOGIN",
        "NAME",
        "HOSTS",
        "PORTS",
        "   CREATE TARGET",
        "NAME",
        "SCAN",
        "TARGET",
        "    CREATE TASK",
        "TASK",
        "REFRESH",
        "PROGRESS",
        "       START",
        "       STOP",
        "TASK",
        "FORMAT",
        "    SHOW REPORT",
        "   EXPORT REPORT"
    };
    
    void login(std::vector<std::string> *user_configs);
    void main();
    int menu(std::vector<std::string> *values, std::size_t n);
    int report(std::vector<std::string> *values, std::size_t n);
    void progress(std::string p);
    void status(std::string sts);
    void marker(bool is_menu = true, bool show = true);
    void delete_windows_arr();
    void cleanup();

private:
    std::size_t n_values;
    long menu_data_lines = 36;

    WINDOW **windows_arr = NULL;
    WINDOW *window_menu_data = NULL;

    void menu_data(std::vector<std::string> **values, int c_item, std::size_t n);
    void menu_data_scroll();
    void report_data(std::vector<std::string> **values,
                     unsigned int c_item, std::size_t n);
};

#endif
