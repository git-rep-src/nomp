#include <form.h>

void init();
void ui_login();
void ui();
WINDOW **create_menu(char ***p_arr, int rows);
void delete_menu(WINDOW **p_windows_menu);
int scroll_menu(WINDOW **p_windows_menu);
void driver();
int get_login();
int get_scans();
int get_targets();
int get_tasks();
int run(char *cmd, char *arg);
void parse_string(char ***p_arr);
char *clean_string(char *str);
void quit();
