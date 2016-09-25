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
    
    FIELD *fields_login[7];
    FIELD *fields_main[7];

    FORM *form_login;
    FORM *form_main;

    WINDOW *window;
    
    FIELD **p_fields = NULL;
    FORM **p_form = NULL;
    WINDOW **p_windows_menu = NULL;
    
    void main();
    WINDOW **create_menu(vector<string> *values, int rows);
    void delete_menu(WINDOW **p_windows_menu);
    int scroll_menu(WINDOW **p_windows_menu);
    void error(const string err);
    void cleanup();

    vector<string> get_fields_value(vector<int> *f, int i);

private:
    int n;

    void login();
    char *trim_whitespaces(char *str);
};
