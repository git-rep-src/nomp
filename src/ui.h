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
    
    vector<string> field_names;
    
    FORM *form;
    FIELD *fields[20];
    
    void authenticate(const vector<string> *user_configs);
    void main();
    int menu(const vector<string> *values, size_t n);
    int report(const vector<string> *values, size_t n);
    void progress(const string &p);
    void status(const string &sts);
    void marker(bool is_menu = true, bool show = true);
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
};

#endif
