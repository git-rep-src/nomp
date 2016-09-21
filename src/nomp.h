#include <form.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void init();
void ui_login();
void ui();
WINDOW **create_menu(char ***p_arr, int rows);
void delete_menu(WINDOW **p_windows_menu);
int scroll_menu(WINDOW **p_windows_menu);
void driver();
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
