#include <form.h>

FIELD *field_login[5];
FORM *form_login;
WINDOW *win_login; 

void init();
void login();
WINDOW *create_window(int height, int width, int starty, int startx);
void cmd();
void driver();
void cleanup();
