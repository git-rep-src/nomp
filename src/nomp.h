#include <form.h>

void init();
void ui_login();
void ui();
void driver();
int login();
int get_status();
int get_scans();
int get_targets();
int get_tasks();
int run(char *cmd, char *arg);
void parse_string(char ***p_arr, int y, int x, int err);
char *clean_string(char *str);
void quit();
