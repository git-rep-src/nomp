#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nomp.h"

#define KEY_RETURN 10
#define KEY_DELCHAR 263
#define KEY_ESCAPE 27
#define KEY_QUIT 113

FIELD *fields[7];
FIELD *fields_login[6];
FIELD **p_fields = NULL;

FORM *form;
FORM *form_login;
FORM **p_form = NULL;

WINDOW *window;

char host[BUFSIZ]; // TODO: REVISAR ESTOS CHAR.
char port[BUFSIZ];
char user[BUFSIZ];
char password[BUFSIZ];
char **scans = NULL;
char **targets = NULL;
char **tasks = NULL;
char cmd_ret[BUFSIZ];

int logged = 0;
int n_fields;

void init()
{
    initscr();
    noecho();
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
    
    ui_login();
}

void ui_login()
{ 
    int i;
    int rows;
    int cols;
    
    p_fields = &fields_login[0];
    p_form = &form_login;

    n_fields = ((sizeof(fields_login) / sizeof(fields_login[0])) - 2);
    
    window = newwin((LINES - 24), 80, ((LINES - 25) / 2), ((COLS - 80) / 2));
    wbkgd(window, COLOR_PAIR(3));
    keypad(window, TRUE);
    
    fields_login[0] = new_field(1, 30, 6, 24, 0, 0);
    fields_login[1] = new_field(1, 30, 8, 24, 0, 0);
    fields_login[2] = new_field(1, 30, 10, 24, 0, 0);
    fields_login[3] = new_field(1, 30, 12, 24, 0, 0);
    fields_login[4] = new_field(1, 7, 14, 35, 0, 0);
    fields_login[5] = NULL;
    
    for (i = 0; i <= n_fields; i++) {
        field_opts_off(fields_login[i], O_AUTOSKIP);
        if (i == 3)
            field_opts_off(fields_login[i], O_PUBLIC);
        else if (i == 4)
            field_opts_off(fields_login[i], O_EDIT);
        else
            set_field_just(fields_login[i], JUSTIFY_CENTER);
    }
    
    set_field_back(fields_login[0], COLOR_PAIR(4));
    
    set_field_buffer(fields_login[0], 0, "localhost");
    set_field_buffer(fields_login[1], 0, "9390");
    set_field_buffer(fields_login[2], 0, "user"); // Borrar.
    set_field_buffer(fields_login[3], 0, "ak474747**OPENVAS"); // Borrar.
    set_field_buffer(fields_login[4], 0, " LOGIN ");
    
    form_login = new_form(fields_login);
    scale_form(form_login, &rows, &cols);
    
    set_form_win(form_login, window);
    set_form_sub(form_login, derwin(window, rows, cols, 2, 2));

    post_form(form_login);
    
    wattron(window, A_BOLD | COLOR_PAIR(3));
    mvwprintw(window, 8, 17,  "    HOST");
    mvwprintw(window, 10, 17, "    PORT");
    mvwprintw(window, 12, 17, "    USER");
    mvwprintw(window, 14, 17, "PASSWORD");
    wattroff(window, A_BOLD | COLOR_PAIR(3));
    
    wrefresh(stdscr);
    wrefresh(window);

    form_driver(*p_form, REQ_END_LINE);
    
    driver();
}

void ui()
{
    int i;
    int rows;
    int cols;
     
    p_fields = &fields[0];
    p_form = &form;
    n_fields = ((sizeof(fields) / sizeof(fields[0])) - 2);
    
    window = newwin((LINES - 24), 120, ((LINES - 25) / 2), ((COLS - 120) / 2));
    wbkgd(window, COLOR_PAIR(3));
    keypad(window, TRUE);

    fields[0] = new_field(1, 30, 1, 13, 0, 0);
    fields[1] = new_field(1, 30, 3, 13, 0, 0);
    fields[2] = new_field(1, 30, 5, 13, 0, 0);
    fields[3] = new_field(1, 30, 10, 13, 0, 0);
    fields[4] = new_field(1, 30, 12, 13, 0, 0);
    fields[5] = new_field(1, 30, 14, 13, 0, 0);
    fields[6] = NULL;
    
    for (i = 0; i <= n_fields; i++) {
        set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
    }
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 2, 2));

    post_form(form);
    
    wattron(window, A_BOLD | COLOR_PAIR(3));
    mvwprintw(window, 3, 8,  "  NAME");
    mvwprintw(window, 5, 8,  " HOSTS");
    mvwprintw(window, 7, 8,  " PORTS");
    mvwprintw(window, 12, 8, "  NAME");
    mvwprintw(window, 14, 8, "TARGET");
    mvwprintw(window, 16, 8, "  SCAN");
    //mvwprintw(window, 3, 90,  "STATUS");
    //mvwprintw(window, 13, 90, " SCANS");
    //mvwprintw(window, 19, 90, "TARGETS");
    wattroff(window, A_BOLD | COLOR_PAIR(3));
    
    wrefresh(stdscr);
    wrefresh(window);

    form_driver(*p_form, REQ_END_LINE);

    get_scans();
    get_targets();
    get_tasks();
}

void driver()
{ 
    int key;
    int c_field = 0;

    do {
        key = wgetch(window);
        switch(key)
		{	
            case KEY_LEFT:
                form_driver(*p_form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(*p_form, REQ_NEXT_CHAR);
                break;
            case KEY_UP:
                set_field_back(p_fields[c_field], COLOR_PAIR(2));
                if (c_field == 0)
                    c_field = n_fields;
                else
                    --c_field;
                form_driver(*p_form, REQ_PREV_FIELD);
                form_driver(*p_form, REQ_END_LINE);
                set_field_back(p_fields[c_field], COLOR_PAIR(4));
                break;
            case KEY_DOWN:
                set_field_back(p_fields[c_field], COLOR_PAIR(2));
                if (c_field == n_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(*p_form, REQ_NEXT_FIELD);
                form_driver(*p_form, REQ_END_LINE);
                set_field_back(p_fields[c_field], COLOR_PAIR(4));
                break;
            case KEY_DELCHAR:
                // TODO: Que no salte al field anterior al borrar todo el texto.
                form_driver(*p_form, REQ_DEL_PREV);
                break;
            case KEY_RETURN:
                // FIX: si p_form == form_login
                if (c_field == 4) {
                    if (login() == 1) {
                        logged = 1;
                        c_field = 0;
                        ui();
                    }
                } 
                break;
            default:
                form_driver(*p_form, key);
                break;
        }
    } while (key != KEY_QUIT);
    
    quit();
}

int login()
{
    // TODO: Validar.
    strcpy(&host[0], clean_string(field_buffer(fields_login[0], 0)));
    strcpy(&port[0], clean_string(field_buffer(fields_login[1], 0)));
    strcpy(&user[0], clean_string(field_buffer(fields_login[2], 0)));
    strcpy(&password[0], clean_string(field_buffer(fields_login[3], 0)));

    if (get_status() == 0) {    
        int i;
        unpost_form(form_login);
        free_form(form_login);
        for (i = 0; i <= n_fields; i++)
	        free_field(fields_login[i]);

        delwin(window);
    
        return 1;
    }

    return 0;
}

int get_status()
{
    if (run("omp", "--ping") != 0) {
        parse_string(NULL, 3, 80, 1);
        return 1;
    }

    return 0;
}

int get_scans()
{
    if (run("omp", "-g") == 0) {
        parse_string(&scans, 3, 70, 0);
    } else {
        parse_string(NULL, 3, 80, 1);
        return 1;
    }

    return 0;
}

int get_targets()
{
    if (run("omp", "-T") == 0) {
        parse_string(&targets, 13, 70, 0);
    } else {
        parse_string(NULL, 3, 80, 1);
        return 1;
    }

    return 0;
}

int get_tasks()
{
    if (run("omp", "-G") == 0) {
        parse_string(&tasks, 19, 70, 0);
    } else {
        parse_string(NULL, 3, 80, 1);
        return 1;
    }

    return 0;
}

int run(char *cmd, char *arg)
{
    char buf[BUFSIZ];
    char path[BUFSIZ];
    FILE *fp;

    memset(cmd_ret, 0x00, sizeof(cmd_ret));
    
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

void parse_string(char ***p_arr, int y, int x, int err)
{
    int i = 0;
    char buf[BUFSIZ];
    char buf2[BUFSIZ];
    char *token;

    strcpy(buf, cmd_ret);

    token = strtok(buf, "\n");                        
    while (token != NULL) {
        if (err == 0) {
            *p_arr = realloc(*p_arr, (i + 1) * sizeof(**p_arr));
            // TODO: Dejar solo el hash.
            (*p_arr)[i] = token;
            snprintf(buf2, sizeof(buf2), "%i%s", i, token += 36);
        } else {
            snprintf(buf2, sizeof(buf2), "%s", token);
        }
        mvwprintw(window, y, x, buf2);
        token = strtok(NULL, "\n");
        ++i;
        ++y;
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
    int i;
    
    free(scans);
    free(targets);
    free(tasks);
    
    unpost_form(*p_form);
    free_form(*p_form);
    for (i = 0; i <= n_fields; i++)
	    free_field(p_fields[i]);
    
    delwin(window);
    endwin();
}
