#include <string.h>
#include<iostream>
#include "ui.h"

using namespace std;

Ui::Ui() :
    n(0)
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();
    
    if (getenv("ESCDELAY") == NULL)
        set_escdelay(25);
    
    if (has_colors()) {
        start_color();
        
        init_color(COLOR_BLACK, 20, 20, 20);
        init_color(COLOR_BLUE, 70, 70, 70);
        init_color(COLOR_CYAN, 0, 130, 130);
        init_color(COLOR_GREEN, 0, 260, 260);
        init_color(COLOR_MAGENTA, 150, 150, 150);
        
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
        init_pair(4, COLOR_WHITE, COLOR_GREEN);
        init_pair(5, COLOR_BLACK, COLOR_BLUE);
        init_pair(6, COLOR_WHITE, COLOR_RED);
        init_pair(7, COLOR_BLUE, COLOR_BLUE);
        init_pair(8, COLOR_WHITE, COLOR_WHITE);
        init_pair(9, COLOR_WHITE, COLOR_MAGENTA); // FIX: Pasar a 7.
        init_pair(10, COLOR_BLACK, COLOR_MAGENTA);// FIX: Pasar a 6.
    }
    
    login();
}

Ui::~Ui()
{
    cleanup();
    endwin();
}

void Ui::login()
{
    int rows;
    int cols;
   
    p_fields = &fields_login[0];
    p_form = &form_login;

    n_fields = ((sizeof(fields_login) / sizeof(fields_login[0])) - 2);
    
    fields_login[0] = new_field(1, 42, 20, 66, 0, 0);
    fields_login[1] = new_field(1, 42, 22, 66, 0, 0);
    fields_login[2] = new_field(1, 42, 24, 66, 0, 0);
    fields_login[3] = new_field(1, 42, 26, 66, 0, 0);
    fields_login[4] = new_field(1, 20, 28, 66, 0, 0);
    fields_login[5] = new_field(1, 20, 28, 88, 0, 0);
    fields_login[6] = NULL;
    
    for (int i = 0; i <= n_fields; i++) {
        if ((i == 4) || (i == 5))
            field_opts_off(fields_login[i], O_EDIT);
        else
            set_field_just(fields_login[i], JUSTIFY_CENTER);
        field_opts_off(fields_login[i], O_AUTOSKIP);
    }
    
    set_field_back(fields_login[0], COLOR_PAIR(1));
    set_field_back(fields_login[1], COLOR_PAIR(1));
    set_field_back(fields_login[2], COLOR_PAIR(2));
    set_field_back(fields_login[3], COLOR_PAIR(7));
    set_field_back(fields_login[4], COLOR_PAIR(3));
    set_field_back(fields_login[5], COLOR_PAIR(3));
    
    set_field_buffer(fields_login[0], 0, "localhost");
    set_field_buffer(fields_login[1], 0, "9390");
    set_field_buffer(fields_login[2], 0, "user");
    set_field_buffer(fields_login[3], 0, "ak474747**OPENVAS");
    set_field_buffer(fields_login[4], 0, "       LOGIN");
    set_field_buffer(fields_login[5], 0, "        EXIT");
    
    form_login = new_form(fields_login);
    scale_form(form_login, &rows, &cols);
    post_form(form_login);
    
    mvprintw(20, 57, "    HOST");
    mvprintw(22, 57, "    PORT");
    mvprintw(24, 57, "    USER");
    mvprintw(26, 57, "PASSWORD");
    
    refresh();

    set_current_field(form_login, fields_login[2]);
    form_driver(form_login, REQ_END_LINE);
}

void Ui::main()
{
    int rows;
    int cols;
     
    p_fields = &fields_main[0];
    p_form = &form_main;
    n_fields = ((sizeof(fields_main) / sizeof(fields_main[0])) - 2);
    
    fields_main[0] = new_field(1, 42, 12, 23, 0, 0);
    fields_main[1] = new_field(1, 42, 14, 23, 0, 0);
    fields_main[2] = new_field(1, 42, 16, 23, 0, 0);
    fields_main[3] = new_field(1, 20, 18, 34, 0, 0);
    fields_main[4] = new_field(1, 42, 22, 23, 0, 0);
    fields_main[5] = new_field(1, 42, 24, 23, 0, 0);
    fields_main[6] = new_field(1, 42, 26, 23, 0, 0);
    fields_main[7] = new_field(1, 20, 28, 34, 0, 0);
    fields_main[8] = new_field(1, 42, 32, 23, 0, 0);
    fields_main[9] = new_field(1, 42, 34, 23, 0, 0);
    fields_main[10] = new_field(1, 20, 36, 23, 0, 0);
    fields_main[11] = new_field(1, 20, 36, 45, 0, 0);
    fields_main[12] = new_field(1, 42, 44, 23, 0, 0);
    fields_main[13] = new_field(1, 20, 46, 34, 0, 0);
    fields_main[14] = NULL;

    for (int i = 0; i <= n_fields; i++) {
        if ((i != 0) && (i != 1) && (i != 4))
            field_opts_off(fields_main[i], O_EDIT);
        if ((i != 3) && (i != 7) &&
            (i != 10) && (i != 11) && (i != 13))
            set_field_just(fields_main[i], JUSTIFY_CENTER);
        field_opts_off(fields_main[i], O_AUTOSKIP);
    }
    
    set_field_back(fields_main[0], COLOR_PAIR(2));
    set_field_back(fields_main[1], COLOR_PAIR(1));
    set_field_back(fields_main[2], COLOR_PAIR(1));
    set_field_back(fields_main[3], COLOR_PAIR(3));
    set_field_back(fields_main[4], COLOR_PAIR(1));
    set_field_back(fields_main[5], COLOR_PAIR(1));
    set_field_back(fields_main[6], COLOR_PAIR(1));
    set_field_back(fields_main[7], COLOR_PAIR(3));
    set_field_back(fields_main[8], COLOR_PAIR(1));
    set_field_back(fields_main[9], COLOR_PAIR(1));
    set_field_back(fields_main[10], COLOR_PAIR(3));
    set_field_back(fields_main[11], COLOR_PAIR(5));
    set_field_back(fields_main[12], COLOR_PAIR(1));
    set_field_back(fields_main[13], COLOR_PAIR(5));
    
    set_field_buffer(fields_main[3], 0,  "   CREATE TARGET");
    set_field_buffer(fields_main[7], 0,  "    CREATE TASK");
    set_field_buffer(fields_main[10], 0, "       START");
    set_field_buffer(fields_main[11], 0, "       STOP");
    set_field_buffer(fields_main[13], 0, "     GET REPORT");
    
    form_main = new_form(fields_main);
    scale_form(form_main, &rows, &cols);
    post_form(form_main);

    mvprintw(12, 14, "    NAME");
    mvprintw(14, 14, "   HOSTS");
    mvprintw(16, 14, "   PORTS");
    mvprintw(22, 14, "    NAME");
    mvprintw(24, 14, "    SCAN");
    mvprintw(26, 14, "  TARGET");
    mvprintw(32, 14, "    TASK");
    mvprintw(34, 14, " REFRESH");
    mvprintw(40, 14, "PROGRESS");
    mvprintw(44, 14, "  FORMAT");
    
    refresh();

    form_driver(form_main, REQ_END_LINE);
}

WINDOW **Ui::create_menu(vector<string> *values, int rows)
{
    n = ((values->size()) / 2);
    
    WINDOW **windows_menu;
    windows_menu = (WINDOW **) malloc ((n + 1) * sizeof(WINDOW *));
    
    windows_menu[0] = newwin(n, 42, rows, 23);

    wbkgd(windows_menu[0], COLOR_PAIR(1)); 
    
    for (int i = 0; i < n; i++) {
        windows_menu[i + 1] = subwin(windows_menu[0], 1, 42, (i + rows), 23);
        wprintw(windows_menu[i + 1], " %s", (*values)[i].c_str());
    }

    wbkgd(windows_menu[1], COLOR_PAIR(9));
    
    wrefresh(windows_menu[0]);

    return windows_menu;
}

void Ui::delete_menu(WINDOW **p_windows_menu)
{
    for (int i = 0; i <= n; i++)
        delwin(p_windows_menu[i]);
    free(p_windows_menu);
}

int Ui::scroll_menu(WINDOW **p_windows_menu)
{
    int key;
    int c_item = 0;
    
    do {
        key = getch();
        switch(key)
        {
            case KEY_UP:
            case KEY_DOWN:
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(1));
                wrefresh(p_windows_menu[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n);
                else
                    c_item = (((c_item + n) - 1) % n);
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(9));
                wrefresh(p_windows_menu[c_item + 1]);
                break;
            case KEY_RETURN:
                return c_item;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    return -1;
}

void Ui::progress(string p)
{
    mvhline(38, 23, ACS_VLINE, (stoi(p) / 3));
    refresh();
}

void Ui::error(const string err)
{
    touchwin(stdscr);
    refresh();
    
    attron(A_BOLD | COLOR_PAIR(6));
    mvprintw(13, 20, err.c_str());
    attroff(A_BOLD | COLOR_PAIR(6));
}

void Ui::cleanup()
{
    unpost_form(*p_form);
    free_form(*p_form);
    for (int i = 0; i <= n_fields; i++)
        free_field(p_fields[i]);
}

vector<string> Ui::get_fields_value(vector<int> *f, int i)
{
    int n = f->back();
    vector<string> ret;
    
    for (; i <= n; i++)
        ret.push_back(string(trim_whitespaces(field_buffer(p_fields[i], 0))));

    return ret;
}

char *Ui::trim_whitespaces(char *str)
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
