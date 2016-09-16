#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nomp.h"

#define KEY_RETURN 10
#define KEY_DELCHAR 263
#define KEY_ESCAPE 27
#define KEY_QUIT 113

FIELD *fields[10];
FORM *form;
WINDOW *window;

char *host;
char *port;
char *user;
char *password;
char **scans = NULL;
char cmd_ret[BUFSIZ];

int login = 0;
int n_fields = ((sizeof(fields) / sizeof(fields[0])) - 2);

void init()
{
    initscr();
    noecho();
    //curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();

    if (has_colors()) {
        start_color();
        init_color(COLOR_CYAN, 150, 150, 150);
        init_color(COLOR_BLUE, 70, 70, 70);
        init_color(COLOR_GREEN, 0, 200, 0);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_CYAN);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
        init_pair(5, COLOR_WHITE, COLOR_GREEN);
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
    fields[4] = new_field(1, 7, 9, 23, 0, 0);
    fields[5] = new_field(1, 30, 11, 13, 0, 0);
    fields[6] = new_field(1, 30, 13, 13, 0, 0);
    fields[7] = new_field(1, 30, 17, 13, 0, 0);
    fields[8] = new_field(1, 30, 19, 13, 0, 0);
    fields[9] = NULL;
    
    set_field_back(fields[0], COLOR_PAIR(4));
    
    int i;
    for (i = 0; i <= n_fields; i++) {
        if (i != 4)
            set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
        if (i >= 5) {
            field_opts_off(fields[i], O_ACTIVE);
            set_field_back(fields[i], COLOR_PAIR(1));
        }
    }
    
    set_field_buffer(fields[0], 0, "localhost");
    set_field_buffer(fields[1], 0, "9390");
    set_field_buffer(fields[3], 0, "ak474747**OPENVAS");
    set_field_buffer(fields[4], 0, " LOGIN ");
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);

    window = newwin((LINES - 24), 120, ((LINES - 25) / 2), ((COLS - 120) / 2));
    wbkgd(window, COLOR_PAIR(3));
    keypad(window, TRUE);
    
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 2, 2));

    post_form(form);
    
    wattron(window, A_BOLD | COLOR_PAIR(3));
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
    wattroff(window, A_BOLD | COLOR_PAIR(3));
    
    wrefresh(stdscr);
    wrefresh(window);

    driver();
}

void driver()
{ 
    int key;
    int c_field = 0;
    int a_fields = 4;

    form_driver(form, REQ_END_LINE);
    
    do {
        key = wgetch(window);
        switch(key)
		{	
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR);
                break;
            case KEY_UP:
                set_field_back(fields[c_field], COLOR_PAIR(2));
                if (c_field == 0)
                    c_field = a_fields;
                else
                    --c_field;
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(fields[c_field], COLOR_PAIR(4));
                break;
            case KEY_DOWN:
                set_field_back(fields[c_field], COLOR_PAIR(2));
                if (c_field == a_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(fields[c_field], COLOR_PAIR(4));
                break;
            case KEY_DELCHAR:
                // TODO: Que no salte al field anterior al borrar todo el texto.
                form_driver(form, REQ_DEL_PREV);
                break;
            case KEY_RETURN:
                if (c_field == 4)
                    authentication(&a_fields);
                break;
            default:
                form_driver(form, key);
                break;
        }
    } while (key != KEY_QUIT);
    
    quit();
}

void authentication(int *a_fields)
{
    if (login == 0) {
        host = clean_string(field_buffer(fields[0], 0));
        port = clean_string(field_buffer(fields[1], 0));
        user = clean_string(field_buffer(fields[2], 0));
        password = clean_string(field_buffer(fields[3], 0));

        if (run("omp", "-g") == 0) {
            *a_fields = n_fields;
            int i;
            for (i = 0; i <= *a_fields; i++) {
                set_field_back(fields[i], COLOR_PAIR(2));
                field_opts_on(fields[i], O_ACTIVE);
            }
            set_field_buffer(fields[4], 0, "LOGOUT");
            parse_string(&scans, 3, 80, 0);
            login = 1;
        } else {
            parse_string(&scans, 3, 80, 1);
        }
    } else {
        set_field_buffer(fields[4], 0, " LOGIN ");
        login = 0;
    }

    //printf("%.36s", scans[0]);
}

int run(char *cmd, char *arg)
{
    char buf[BUFSIZ];
    char path[BUFSIZ];
    FILE *fp;

    if (strcmp(cmd, "omp") == 0)
        snprintf(buf, sizeof(buf), "%s -h %s -p %s -u %s -w %s %s 2>&1", cmd, host, port, user, password, arg);
    else
        snprintf(buf, sizeof(buf), "%s %s 2>&1", cmd, arg);
   
    if ((fp = popen(buf, "r")) != NULL) {
        while (fgets(path, BUFSIZ, fp) != NULL)
            strcat(cmd_ret, path);
    } else {
        strcpy(cmd_ret, "ERROR.");
        return 1;
    }

    return pclose(fp);    
}

void parse_string(char ***arr, int x, int y, int err)
{
    int i = 0;
    char buf[BUFSIZ];
    char buf2[BUFSIZ];
    char *token;

    strcpy(buf, cmd_ret);

    token = strtok(buf, "\n");                        
    while (token != NULL) {
        if (err == 0) {
            *arr = realloc(*arr, (i + 1) * sizeof(**arr));
            // TODO: Dejar solo el hash.
            (*arr)[i] = token;
            snprintf(buf2, sizeof(buf2), "%i%s", i, token += 36);
        } else {
            snprintf(buf2, sizeof(buf2), "%s", token);
        }
        mvwprintw(window, x + 1, y, buf2);
        token = strtok(NULL, "\n");
        ++i;
        ++x;
    }
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

void quit()
{
    free(scans);
    
    unpost_form(form);
    
    free_form(form);
    
    int i;
    for (i = 0; i <= n_fields; i++)
	    free_field(fields[i]);
    
    delwin(window);
    endwin();
}
