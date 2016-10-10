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
    void menu(vector<string> *values, int rows);
    int menu_scroll(vector<string> *values);
    void report(vector<string> *values);
    int report_scroll(vector<string> *values);
    void delete_windows_arr(int windows_extra = 0);
    void delete_arr_report();
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
    void menu_data(vector<string> **values, int c_item);
    void menu_data_scroll();
    void report_data(vector<string> **values, int c_item);
    char *trim_whitespaces(char *str);
};
