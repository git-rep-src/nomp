#ifndef UI_H
#define UI_H

#include <form.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

const int KEY_Y = 121;
const int KEY_N = 110;
const int KEY_DELCHAR = 127;
const int KEY_RETURN = 10;
const int KEY_UP_SCREEN = 21;
const int KEY_DOWN_SCREEN = 4;
const int KEY_ESCAPE = 27;
const int KEY_QUIT = 5;

class Ui
{
public:
    Ui();
    ~Ui();
   
    bool has_status;
    bool is_empty_report;
    
    vector<string> field_names;
    
    WINDOW *window;
    FORM *form;
    FIELD *fields[20];
    
    void authenticate(const vector<string> *user_configs);
    void main();
    int menu(const vector<string> *values, size_t n, int n_tabs);
    int report(const vector<string> *values, size_t n);
    void marker(bool is_menu = true, bool show = true);
    void status(const string &s, int n_tabs);
    void progress(const string &p, int n_tabs);
    int save();
    void clear_items();
    void clear_form();

private:
    int start_x;
    int start_y;
    int field_width;
    int button_width;

    size_t n_values;

    WINDOW **items = NULL;

    void menu_details(const vector<string> **values, int c_item, size_t n);
    void report_details(const vector<string> **values, unsigned int c_item, size_t n);
    void fill_field_names();
};

#endif
