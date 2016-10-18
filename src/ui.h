#include<string>
#include<vector>
#include <utility> 
#include<iostream>//

#include <form.h>

using namespace std;

const int KEY_RETURN = 10;
const int KEY_TAB = 9;
const int KEY_ESCAPE = 27;
const int KEY_QUIT = 17;

class Ui
{
public:
    Ui();
    ~Ui();

    int n_fields;

    FIELD **p_fields = NULL;
    FORM **p_form = NULL;
    
    void login();
    void main();
    int menu(vector<string> *values, uint n);
    int report(vector<string> *values, uint n);
    void progress(string p);
    void status(pair<string, int> sts);
    void marker(bool is_menu = true, bool show = true);
    void delete_windows_arr();
    void cleanup();

private:
    int n_values;
    int menu_data_lines = 36;

    FIELD *fields_login[6];
    FIELD *fields_main[17];

    FORM *form_login;
    FORM *form_main;

    WINDOW **windows_arr = NULL;
    WINDOW *window_menu_data = NULL;

    void menu_data(vector<string> **values, int c_item, uint n);
    void menu_data_scroll();
    void report_data(vector<string> **values, int c_item, uint n);
};
