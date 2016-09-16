#include <form.h>

void init();
void ui();
void driver();
void authentication(int *a_fields);
int run(char *cmd, char *arg);
void parse_string(char ***arr, int x, int y, int err);
char *clean_string(char *str);
void quit();
