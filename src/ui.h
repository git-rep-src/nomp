#include<vector>
#include<string>
#include<iostream>//

#include <form.h>

using namespace std;

const int KEY_ESCAPE = 27;
const int KEY_RETURN = 10;
const int KEY_TAB = 9;
const int KEY_DELCHAR = 263; 
const int KEY_QUIT = 113;

class Ui
{
public:
    Ui();
    ~Ui();

    int n_fields;

    FIELD **p_fields = NULL;
    FORM **p_form = NULL;
    
    void main();
    int menu(vector<string> *values, uint n);
    int report(vector<string> *values, uint n);
    void delete_windows_arr();
    void indicator(bool is_menu = true, bool show = true);
    void progress(string p);
    void error(const string err);
    void cleanup();
    vector<string> get_fields_value(vector<int> *i_fields, int i);

private:
    int n_values;
    int menu_data_lines = 36;

    FIELD *fields_login[6];
    FIELD *fields_main[17];

    FORM *form_login;
    FORM *form_main;

    WINDOW **windows_arr = NULL;
    WINDOW *window_menu_data = NULL;
    
    void login();
    void menu_data(vector<string> **values, int c_item, uint n);
    void menu_data_scroll();
    void report_data(vector<string> **values, int c_item, uint n);
    char *trim_whitespaces(char *str);
};
