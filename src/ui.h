#include <form.h>

class Ui
{
public:
    Ui();
    ~Ui();

    FIELD *fields_login[7];
    FIELD *fields[7];
    FIELD **p_fields = NULL;

    FORM *form_login;
    FORM *form;
    FORM **p_form = NULL;

    WINDOW *window;

    int n = 0;
    int n_fields;
    int is_logged = FALSE;

private:
    void login();
    void main();
//    WINDOW **create_menu(char ***p_arr, int rows);
//    void delete_menu(WINDOW **p_windows_menu);
//    int scroll_menu(WINDOW **p_windows_menu);
/*
    int create_xml(int n);
    void xml_parse_node();
    void xml_parse_string(xmlDocPtr doc, xmlNodePtr cur, char ***p_arr);
    int get_login();
    int get_scans();
    int get_targets();
    int run(char *cmd, char *arg);
    void parse_string(char ***p_arr);
    char *clean_string(char *str);
    void quit();
*/
};
