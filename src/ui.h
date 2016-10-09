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
    void menu_create(vector<string> *values, int rows, bool is_report = false);
    int menu_scroll(vector<string> *values, bool is_report = false);
    void menu_delete(int windows_extra = 0);
    void progress(string p);
    void error(const string err);
    void cleanup();
    vector<string> get_fields_value(vector<int> *i_fields, int i);

private:
    int n_values;
    int lines_pad_data;

    FIELD *fields_login[7];
    FIELD *fields_main[17];

    FORM *form_login;
    FORM *form_main;

    WINDOW **windows_menu = NULL;
    WINDOW *pad_data = NULL;
    
    void login();
    void data(vector<string> **values, int c_item);
    void data_scroll();
    void data_report(vector<string> **values, int c_item);
    char *trim_whitespaces(char *str);
};
