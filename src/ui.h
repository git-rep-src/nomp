#ifndef UI_H
#define UI_H

#include <form.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

const int KEY_RETURN = 10;
const int KEY_TAB = 9;
const int KEY_Y = 121;
const int KEY_N = 110;
const int KEY_ESCAPE = 27;
const int KEY_QUIT = 17;

class Ui
{
public:
    Ui();
    ~Ui();
    
    const vector<string> fields_name =
    {
        "HOST", "PORT", "USERNAME", "PASSWORD",
        "       LOGIN", "NAME", "HOSTS", "PORTS",
        "   CREATE TARGET", "NAME", "SCAN", "TARGET",
        "    CREATE TASK", "TASK", "REFRESH", "PROGRESS",
        "       START", "      RESUME", "       STOP",
        "TASK", "FORMAT", "    SHOW REPORT",
        "   EXPORT REPORT", "                   SAVE",
        "                   EXIT"
    };
    
    FORM *form;
    FIELD *fields[17];
    
    void login(vector<string> *user_configs);
    void main();
    int menu(vector<string> *values, size_t n);
    int report(vector<string> *values, size_t n);
    void progress(string p);
    void status(string sts);
    int save_config();
    void marker(bool is_menu = true, bool show = true);
    void clear_windows_arr();
    void cleanup();

private:
    size_t n_values;
    long menu_data_lines = 36;

    WINDOW **windows_arr = NULL;
    WINDOW *window_menu_data = NULL;

    void menu_data(vector<string> **values, int c_item, size_t n);
    void menu_data_scroll();
    void report_data(vector<string> **values, unsigned int c_item, size_t n);
};

#endif
