#ifndef UI_H
#define UI_H

#include <vector>
#include <string>

#include <form.h>

const int KEY_RETURN = 10;
const int KEY_TAB = 9;
const int KEY_ESCAPE = 27;
const int KEY_QUIT = 17;

class Ui
{
public:
    Ui();
    ~Ui();

    std::size_t n_fields;
    
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

    FIELD **p_fields = NULL;
    FORM **p_form = NULL;
    
    void login();
    void main();
    int menu(std::vector<std::string> *values, std::size_t n);
    int report(std::vector<std::string> *values, std::size_t n);
    void progress(std::string p);
    void status(std::pair<std::string, int> sts, bool is_login = false);
    void marker(bool is_menu = true, bool show = true);
    void delete_windows_arr();
    void cleanup();

private:
    std::size_t n_values;
    long menu_data_lines = 36;

    FIELD *fields_login[6];
    FIELD *fields_main[17];

    FORM *form_login;
    FORM *form_main;

    WINDOW **windows_arr = NULL;
    WINDOW *window_menu_data = NULL;

    void menu_data(std::vector<std::string> **values,
                   unsigned int c_item, std::size_t n);
    void menu_data_scroll();
    void report_data(std::vector<std::string> **values,
                     unsigned int c_item, std::size_t n);
};

#endif
