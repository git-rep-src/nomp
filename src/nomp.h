#include <form.h>

void init();
void ui();
void driver();
int run(char *cmd, char *arg);
char *clean_string(char *str);
WINDOW *create_window(int nl, int nc, int par_y, int par_x, int cp);
void quit();
