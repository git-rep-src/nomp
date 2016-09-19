#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nomp.h"

#define KEY_RETURN 10
#define KEY_DELCHAR 263
#define KEY_ESCAPE 27
#define KEY_QUIT 113

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

char *config[4];
char **scans;
char **targets;
char **tasks;
char ret[BUFSIZ];
const char *list_ports[] = {
    "All IANA assigned TCP",
    "All IANA assigned TCP/UDP",
    "All privileged TCP",
    "All privileged TCP/UDP",
    "All TCP",
    "All TCP & Nmap top 100 UDP",
    "All TCP & Nmap top 1000 UDP",
    "Nmap top 2000 TCP/100 UDP",
    "OpenVAS Default"
};

void init()
{
    initscr();
    noecho();
    curs_set(0);
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
        init_pair(6, COLOR_WHITE, COLOR_RED);
    }
    
    ui_login();
}

void ui_login()
{
    int rows;
    int cols;
    
    p_fields = &fields_login[0];
    p_form = &form_login;

    n_fields = ((sizeof(fields_login) / sizeof(fields_login[0])) - 2);
   
    window = newwin((LINES - 34), 62, ((LINES - 14) / 2), ((COLS - 63) / 2));
    wbkgd(window, COLOR_PAIR(3));
    keypad(window, TRUE);
    
    fields_login[0] = new_field(1, 30, 1, 14, 0, 0);
    fields_login[1] = new_field(1, 30, 3, 14, 0, 0);
    fields_login[2] = new_field(1, 30, 5, 14, 0, 0);
    fields_login[3] = new_field(1, 30, 7, 14, 0, 0);
    fields_login[4] = new_field(1, 14, 9, 14, 0, 0);
    fields_login[5] = new_field(1, 14, 9, 30, 0, 0);
    fields_login[6] = NULL;
    
    for (int i = 0; i <= n_fields; i++) {
        field_opts_off(fields_login[i], O_AUTOSKIP);
        if (i == 3)
            field_opts_off(fields_login[i], O_PUBLIC);
        else if ((i == 4) || (i == 5))
            field_opts_off(fields_login[i], O_EDIT);
        else
            set_field_just(fields_login[i], JUSTIFY_CENTER);
    }
    
    set_field_buffer(fields_login[0], 0, "localhost");
    set_field_buffer(fields_login[1], 0, "9390");
    set_field_buffer(fields_login[2], 0, "user");
    set_field_buffer(fields_login[3], 0, "ak474747**OPENVAS");
    set_field_buffer(fields_login[4], 0, "     LOGIN ");
    set_field_buffer(fields_login[5], 0, "     EXIT ");
    
    form_login = new_form(fields_login);
    scale_form(form_login, &rows, &cols);
    
    set_form_win(form_login, window);
    set_form_sub(form_login, derwin(window, rows, cols, 2, 2));

    post_form(form_login);
    
    wattron(window, A_BOLD | COLOR_PAIR(3));
    mvwprintw(window, 3, 7, "    HOST");
    mvwprintw(window, 5, 7, "    PORT");
    mvwprintw(window, 7, 7, "    USER");
    mvwprintw(window, 9, 7, "PASSWORD");
    wattroff(window, A_BOLD | COLOR_PAIR(3));
    
    wrefresh(stdscr);
    wrefresh(window);

    set_field_back(fields_login[0], COLOR_PAIR(4));
    form_driver(*p_form, REQ_END_LINE);
    
    driver();
}

void ui()
{
    int rows;
    int cols;
     
    p_fields = &fields[0];
    p_form = &form;
    n_fields = ((sizeof(fields) / sizeof(fields[0])) - 2);
    
    window = newwin((LINES - 24), 62, ((LINES - 25) / 2), ((COLS - 63) / 2));
    wbkgd(window, COLOR_PAIR(3));
    keypad(window, TRUE);

    fields[0] = new_field(1, 30, 1, 14, 0, 0);
    fields[1] = new_field(1, 30, 3, 14, 0, 0);
    fields[2] = new_field(1, 30, 5, 14, 0, 0);
    fields[3] = new_field(1, 30, 10, 14, 0, 0);
    fields[4] = new_field(1, 30, 12, 14, 0, 0);
    fields[5] = new_field(1, 30, 14, 14, 0, 0);
    fields[6] = NULL;
    
    for (int i = 0; i <= n_fields; i++) {
        set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
    }
    field_opts_off(fields[2], O_EDIT);
    field_opts_off(fields[4], O_EDIT);
    field_opts_off(fields[5], O_EDIT);
    
    set_field_buffer(fields[2], 0, "All IANA assigned TCP");
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 2, 2));

    post_form(form);
    
    wattron(window, A_BOLD | COLOR_PAIR(3));
    mvwprintw(window, 3, 9,  "  NAME");
    mvwprintw(window, 5, 9,  " HOSTS");
    mvwprintw(window, 7, 9,  " PORTS");
    mvwprintw(window, 12, 9, "  NAME");
    mvwprintw(window, 14, 9, "TARGET");
    mvwprintw(window, 16, 9, "  SCAN");
    wattroff(window, A_BOLD | COLOR_PAIR(3));
    
    wrefresh(stdscr);
    wrefresh(window);

    set_field_back(fields[0], COLOR_PAIR(4));
    form_driver(*p_form, REQ_END_LINE);
}

WINDOW **create_menu(char ***p_arr, int rows)
{
    if (p_arr == NULL)
        n = 9;

    WINDOW **windows_menu;
    windows_menu = (WINDOW **) malloc ((n + 1) * sizeof(WINDOW *));
    
    windows_menu[0] = newwin(n, 30, rows, 72);

    wbkgd(windows_menu[0], COLOR_PAIR(2)); 
    
    for (int i = 0; i < n; i++) {
        windows_menu[i + 1] = subwin(windows_menu[0], 1, 30, (i + rows), 72);
        if (p_arr == NULL)
            wprintw(windows_menu[i + 1], " %s", list_ports[i]);
        else
            wprintw(windows_menu[i + 1], " %s", (*p_arr)[i] += 37);
    }

    wbkgd(windows_menu[1], COLOR_PAIR(4));
    
    wrefresh(windows_menu[0]);

    return windows_menu;
}

void delete_menu(WINDOW **p_windows_menu)
{
    for (int i = 0; i <= n; i++)
        delwin(p_windows_menu[i]);
    free(p_windows_menu);
}

int scroll_menu(WINDOW **p_windows_menu)
{
    int key;
    int c_item = 0;
    
    do {
        key = getch();
        switch(key)
        {
            case KEY_UP:
            case KEY_DOWN:
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(2));
                wnoutrefresh(p_windows_menu[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n);
                else
                    c_item = (((c_item + n) - 1) % n);
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(4));
                wnoutrefresh(p_windows_menu[c_item + 1]);
                doupdate();
                break;
            case KEY_RETURN:
                return c_item;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    return -1;
}

void driver()
{ 
    int key;
    int c_item;
    int c_field = 0;
    
    WINDOW **p_windows_menu;
    
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
                if (!is_logged) {
                    if (c_field == 4) {
                        if (get_login()) {
                            c_field = 0;
                            is_logged = TRUE;
                            ui();
                        }
                    } else if (c_field == 5) {
                        key = KEY_QUIT;
                    }
                } else {
                    if (c_field == 2) {
                        p_windows_menu = create_menu(NULL, 19);
                    } else if (c_field == 4) {
                        get_targets();
                        p_windows_menu = create_menu(&targets, 26);
                    } else if (c_field == 5) {
                        get_scans();
                        p_windows_menu = create_menu(&scans, 28);
                    }
                    c_item = scroll_menu(p_windows_menu);
                    delete_menu(p_windows_menu);
                    if (c_item >= 0) {
                        if (c_field == 2)
                            set_field_buffer(fields[2], 0, list_ports[c_item]);
                        else if (c_field == 4)
                            set_field_buffer(fields[4], 0, targets[c_item]);
                        else if (c_field == 5)
                            set_field_buffer(fields[5], 0, scans[c_item]);
                    }
                    touchwin(stdscr);
                    touchwin(window);
                    wrefresh(stdscr);
                }
                break;
            default:
                form_driver(*p_form, key);
                break;
        }
    } while (key != KEY_QUIT);
    
    quit();
}

int get_login()
{
    // TODO: Validar.
    int n_config = ((sizeof(config) / sizeof(config[0])));
    for (int i = 0; i < n_config; i++) {
        char *buf = clean_string(field_buffer(fields_login[i], 0));
        int len = (strlen(buf) + 1);
        config[i] = (char*) malloc (len * sizeof(char));
        strcpy(config[i], buf);
    }

    if (run("omp", "--ping") != 0) {
        parse_string(NULL);
        return FALSE;
    }
    
    unpost_form(form_login);
    free_form(form_login);
    for (int i = 0; i <= n_fields; i++)
        free_field(fields_login[i]);
    delwin(window);

    return TRUE;
}

int get_scans()
{
    if (run("omp", "-g") != 0) {
        parse_string(NULL);
        return FALSE;
    }

    parse_string(&scans);

    return TRUE;
}

int get_targets()
{
    if (run("omp", "-T") != 0) {
        parse_string(NULL);
        return FALSE;
    }
    
    parse_string(&targets);
    
    return TRUE;
}

int get_tasks()
{
    if (run("omp", "-G") != 0) {
        parse_string(NULL);
        return FALSE;
    }

    parse_string(&tasks);
    
    return TRUE;
}

int run(char *cmd, char *arg)
{
    char buf[1024];
    char path[BUFSIZ];
    FILE *fp;
    
    memset(ret, 0x00, sizeof(ret));
    
    if (strcmp(cmd, "omp") == 0)
        snprintf(buf, sizeof(buf), "%s -h %s -p %s -u %s -w %s %s 2>&1", cmd, config[0], config[1], config[2], config[3], arg);
    else
        snprintf(buf, sizeof(buf), "%s %s 2>&1", cmd, arg);
    
    if ((fp = popen(buf, "r")) != NULL) {
        while (fgets(path, BUFSIZ, fp) != NULL)
            strcat(ret, path);
    } else {
        strcpy(ret, "ERROR.");
        return ERR;
    }

    return pclose(fp);    
}

void parse_string(char ***p_arr)
{
    int i = 0;
    char *token;

    token = strtok(ret, "\n");                        
    while (token != NULL) {
        if (p_arr == NULL) {
            if (is_logged)
                mvwprintw(window, (i + 20), 7, token);
            else
                mvwprintw(window, (i + 13), 7, token);
        } else {
            *p_arr = realloc(*p_arr, ((i + 1) * sizeof(char*)));
            (*p_arr)[i] = token;
            n = (i + 1);
        }
        token = strtok(NULL, "\n");
        ++i;
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
    int n_config = (sizeof(config) / sizeof(config[0]));
    for (int i = 0; i < n_config; i++)
        free(config[i]);
    free(scans);
    free(targets);
    free(tasks);
    
    if (!is_logged) {
        unpost_form(form_login);
        free_form(form_login);
        for (int i = 0; i <= n_fields; i++)
	        free_field(fields_login[i]);
    } else {
        unpost_form(form);
        free_form(form);
        for (int i = 0; i <= n_fields; i++)
	        free_field(fields[i]);
    }
    
    endwin();
}
