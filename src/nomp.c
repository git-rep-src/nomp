#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nomp.h"

#define KEY_RETURN 10
#define KEY_ESCAPE 27
#define KEY_QUIT 113

FIELD *fields[9];
FORM *form;
WINDOW *window;

char *host;
char *port;
char *user;
char *password;

int n_fields = ((sizeof(fields) / sizeof(fields[0])) - 2);

void init()
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();

    if (has_colors()) {
        start_color();
        init_color(COLOR_GREEN, 0, 200, 0);
        init_color(COLOR_BLUE, 150, 150, 150);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_GREEN);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
    }

    ui();
}

void ui()
{ 
    int rows;
    int cols;
    
    fields[0] = new_field(1, 30, 1, 13, 0, 0);
    fields[1] = new_field(1, 30, 3, 13, 0, 0);
    fields[2] = new_field(1, 30, 5, 13, 0, 0);
    fields[3] = new_field(1, 30, 7, 13, 0, 0);
    fields[4] = new_field(1, 30, 11, 13, 0, 0);
    fields[5] = new_field(1, 30, 13, 13, 0, 0);
    fields[6] = new_field(1, 30, 17, 13, 0, 0);
    fields[7] = new_field(1, 30, 19, 13, 0, 0);
    fields[8] = NULL;
    
    set_field_back(fields[0], COLOR_PAIR(3));
   
    int i;
    for (i = 0; i <= n_fields; i++) {
        set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
        if (i >= 4) {
            field_opts_off(fields[i], O_ACTIVE);
            set_field_back(fields[i], COLOR_PAIR(1));
        }
    }
    
    set_field_buffer(fields[0], 0, "localhost");
    set_field_buffer(fields[1], 0, "9390");
    set_field_buffer(fields[3], 0, "ak474747**OPENVAS");
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);

    window = create_window((LINES - 24), 120, ((LINES - 25) / 2), ((COLS - 120) / 2), 4);
    keypad(window, TRUE);
    
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 2, 2));

    post_form(form);
    
    wattron(window, A_BOLD | COLOR_PAIR(4));
    mvwprintw(window, 3, 6,   "    HOST");
    mvwprintw(window, 5, 6,   "    PORT");
    mvwprintw(window, 7, 6,   "    USER");
    mvwprintw(window, 9, 6,   "PASSWORD");
    mvwprintw(window, 13, 6,  "  TARGET");
    mvwprintw(window, 15, 6,  "    SCAN");
    mvwprintw(window, 19, 6,  "    TASK");
    mvwprintw(window, 21, 6,  "  UPDATE");
    mvwprintw(window, 3, 90,  "STATUS");
    mvwprintw(window, 13, 90, " SCANS");
    mvwprintw(window, 19, 90, "TARGETS");
    wattroff(window, A_BOLD | COLOR_PAIR(4));
    
    wrefresh(stdscr);
    wrefresh(window);

    driver();
}

void driver()
{ 
    int key;
    int c_field = 0;
    int a_fields = 3;

    do {
        key = wgetch(window);
        switch(key)
		{	
            case KEY_DOWN:
                set_field_back(fields[c_field], COLOR_PAIR(2));
                if (c_field == a_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(fields[c_field], COLOR_PAIR(3));
                break;
            case KEY_UP:
                set_field_back(fields[c_field], COLOR_PAIR(2));
                if (c_field == 0)
                    c_field = a_fields;
                else
                    --c_field;
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(fields[c_field], COLOR_PAIR(3));
                break;
            case KEY_RETURN:
                host = clean_string(field_buffer(fields[0], 0));
                port = clean_string(field_buffer(fields[1], 0));
                user = clean_string(field_buffer(fields[2], 0));
                password = clean_string(field_buffer(fields[3], 0));
                if (run("omp", "-g") == 0) {
                    a_fields = n_fields;
                    int i;
                    for (i = 0; i <= a_fields; i++) {
                        set_field_back(fields[i], COLOR_PAIR(2));
                        field_opts_on(fields[i], O_ACTIVE);
                    }
                }
                //field_opts_off(fields[i], O_AUTOSKIP | O_EDIT);
                break;
            default:
                form_driver(form, key);
                break;
        }
    } while (key != KEY_QUIT);
    
    quit();
}

int run(char *cmd, char *arg)
{
    char cmd_buf[BUFSIZ];
    char path[BUFSIZ];
    FILE *fp;
    
    if (strcmp(cmd, "omp") == 0)
        snprintf(cmd_buf, sizeof(cmd_buf), "%s -h %s -p %s -u %s -w %s %s 2>&1", cmd, host, port, user, password, arg);
    else
        snprintf(cmd_buf, sizeof(cmd_buf), "%s %s 2>&1", cmd, arg);
   
    int i = 4;
    if ((fp = popen(cmd_buf, "r")) != NULL) {
        while (fgets(path, BUFSIZ, fp) != NULL) {
            mvwprintw(window, i, 50, path);
            ++i;
        }
    } else {
        return 1;
    }

    return pclose(fp);    
}

char *clean_string(char *str)
{
    char *end;

    while (isspace(*str))
        str++;

    if (*str == 0) 
        return str;

    end = (str + (strnlen(str, 128) - 1));

    while ((end > str) && isspace(*end))
        end--;

    *(end + 1) = '\0';

    return str;
}

WINDOW *create_window(int nl, int nc, int par_y, int par_x, int cp)
{	
    WINDOW *w;
    w = newwin(nl, nc, par_y, par_x);
    wbkgd(w, COLOR_PAIR(cp));
    
    return w;
}

void quit()
{
    unpost_form(form);
    
    free_form(form);
    
    int i;
    for (i = 0; i <= n_fields; i++)
	    free_field(fields[i]);
    
    delwin(window);
    endwin();
}
