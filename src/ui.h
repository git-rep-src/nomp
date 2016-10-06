#include<vector>
#include<string>
#include<iostream>//

#include <form.h>

using namespace std;

const int KEY_ESCAPE = 27;
const int KEY_RETURN = 10;
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
    WINDOW **p_windows_menu = NULL;
    
    void main();
    WINDOW **create_menu(vector<string> *values, int rows, bool is_report = false);
    int scroll_menu(WINDOW **p_windows_menu, vector<string> *values, bool is_report = false);
    void delete_menu(WINDOW **p_windows_menu);
    void progress(string p);
    void error(const string err);
    void cleanup();
    vector<string> get_fields_value(vector<int> *i_fields, int i);

private:
    int n_values;

    FIELD *fields_login[7];
    FIELD *fields_main[17];

    FORM *form_login;
    FORM *form_main;

    WINDOW *window_menu_data_bar = NULL;
    WINDOW *window_menu_data = NULL;
    
    void login();
    void menu_data(vector<string> **values, int c_item);
    void report_data(vector<string> **values, int c_item);
    char *trim_whitespaces(char *str);
};
