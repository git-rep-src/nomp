#include <string.h>
#include<iostream>
#include "ui.h"

using namespace std;

Ui::Ui() :
    n_values(0)
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
        init_color(COLOR_CYAN, 150, 150, 150);
        init_color(COLOR_MAGENTA, 110, 110, 110);
        init_color(COLOR_GREEN, 0, 127, 0);
        
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
        init_pair(4, COLOR_BLACK, COLOR_BLUE);
        init_pair(5, COLOR_WHITE, COLOR_CYAN);
        init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
        init_pair(7, COLOR_BLUE, COLOR_BLUE);
        init_pair(8, COLOR_WHITE, COLOR_WHITE);
        init_pair(9, COLOR_GREEN, COLOR_GREEN);
        init_pair(10, COLOR_WHITE, COLOR_GREEN);
        init_pair(11, COLOR_BLUE, COLOR_BLUE);
        init_pair(12, COLOR_WHITE, COLOR_RED);
        init_pair(13, COLOR_RED, COLOR_MAGENTA);
        init_pair(14, COLOR_YELLOW, COLOR_MAGENTA);
        init_pair(15, COLOR_WHITE, COLOR_MAGENTA);
        init_pair(16, COLOR_WHITE, COLOR_BLACK); // menu_data
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
    
    fields_main[0] = new_field(1, 42, 8, 23, 0, 0);
    fields_main[1] = new_field(1, 42, 10, 23, 0, 0);
    fields_main[2] = new_field(1, 42, 12, 23, 0, 0);
    fields_main[3] = new_field(1, 20, 14, 34, 0, 0);
    fields_main[4] = new_field(1, 42, 18, 23, 0, 0);
    fields_main[5] = new_field(1, 42, 20, 23, 0, 0);
    fields_main[6] = new_field(1, 42, 22, 23, 0, 0);
    fields_main[7] = new_field(1, 20, 24, 34, 0, 0);
    fields_main[8] = new_field(1, 42, 28, 23, 0, 0);
    fields_main[9] = new_field(1, 42, 30, 23, 0, 0);
    fields_main[10] = new_field(1, 20, 34, 23, 0, 0);
    fields_main[11] = new_field(1, 20, 34, 45, 0, 0);
    fields_main[12] = new_field(1, 42, 38, 23, 0, 0);
    fields_main[13] = new_field(1, 42, 40, 23, 0, 0);
    fields_main[14] = new_field(1, 20, 42, 23, 0, 0);
    fields_main[15] = new_field(1, 20, 42, 45, 0, 0);
    fields_main[16] = NULL;

    for (int i =  0; i <= n_fields; i++) {
        if ((i != 0) && (i != 1) && (i != 4))
            field_opts_off(fields_main[i], O_EDIT);
        if ((i != 3) && (i != 7) &&
            (i != 10) && (i != 11) && (i != 14) && (i != 15))
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
    set_field_back(fields_main[11], COLOR_PAIR(4));
    set_field_back(fields_main[12], COLOR_PAIR(1));
    set_field_back(fields_main[13], COLOR_PAIR(1));
    set_field_back(fields_main[14], COLOR_PAIR(4));
    set_field_back(fields_main[15], COLOR_PAIR(4));
    
    set_field_buffer(fields_main[3], 0,  "   CREATE TARGET");
    set_field_buffer(fields_main[7], 0,  "    CREATE TASK");
    set_field_buffer(fields_main[10], 0, "       START");
    set_field_buffer(fields_main[11], 0, "       STOP");
    set_field_buffer(fields_main[14], 0, "    SHOW REPORT");
    set_field_buffer(fields_main[15], 0, "   EXPORT REPORT");
    
    form_main = new_form(fields_main);
    scale_form(form_main, &rows, &cols);
    post_form(form_main);

    mvprintw(8, 14,  "    NAME");
    mvprintw(10, 14, "   HOSTS");
    mvprintw(12, 14, "   PORTS");
    mvprintw(18, 14, "    NAME");
    mvprintw(20, 14, "    SCAN");
    mvprintw(22, 14, "  TARGET");
    mvprintw(28, 14, "    TASK");
    mvprintw(30, 14, " REFRESH");
    mvprintw(32, 14, "PROGRESS");
    mvprintw(38, 14, "    TASK");
    mvprintw(40, 14, "  FORMAT");
    
    attron(COLOR_PAIR(4));
    mvprintw(32, 23, "                                          ");
    attroff(COLOR_PAIR(4));

    window_menu_data = newwin(35, 43, 8, 112);
    wbkgd(window_menu_data, COLOR_PAIR(16));
    
    refresh();

    form_driver(form_main, REQ_END_LINE);
}

WINDOW **Ui::create_menu(vector<string> *values, int rows, bool is_report)
{
    if (is_report)
        n_values = ((values->size()) / 5);
    else
        n_values = ((values->size()) / 3);

    WINDOW **windows_menu;
    windows_menu = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    
    if (is_report)
        windows_menu[0] = newwin(n_values, COLS, 0, 0);
    else
        windows_menu[0] = newwin(n_values, 42, rows, 23);
    
    wbkgd(windows_menu[0], COLOR_PAIR(1)); 
    
    for (int i = 0; i < n_values; i++) {
        if (is_report) {
            windows_menu[i + 1] = subwin(windows_menu[0], 1, COLS, i, 0);
            mvwprintw(windows_menu[i + 1], 0, 1, " %s ", (*values)[n_values + i].c_str());
            wattron(windows_menu[i + 1], A_BOLD | COLOR_PAIR(15));
            mvwprintw(windows_menu[i + 1], 0, 148, " %s ", (*values)[(n_values * 2) + i].c_str());
            wattroff(windows_menu[i + 1], A_BOLD | COLOR_PAIR(15));
            if (i < 18) { 
                wattron(windows_menu[i + 1], A_BOLD | COLOR_PAIR(13));
                mvwprintw(windows_menu[i + 1], 0, 168, " %s ", (*values)[(n_values * 3) + i].c_str());
                wattroff(windows_menu[i + 1], A_BOLD | COLOR_PAIR(13));
            } else {
                wattron(windows_menu[i + 1], A_BOLD | COLOR_PAIR(14));
                mvwprintw(windows_menu[i + 1], 0, 168, " %s ", (*values)[(n_values * 3) + i].c_str());
                wattroff(windows_menu[i + 1], A_BOLD | COLOR_PAIR(14));
            }
        } else {
            windows_menu[i + 1] = subwin(windows_menu[0], 1, 42, (i + rows), 23);
            wprintw(windows_menu[i + 1], " %s", (*values)[n_values + i].c_str()); 
        }
    }
    
    wbkgd(windows_menu[1], COLOR_PAIR(6));
   
    wrefresh(windows_menu[0]);
    
    return windows_menu;
}

int Ui::scroll_menu(WINDOW **p_windows_menu, vector<string> *values, bool is_report)
{
    int key;
    int c_item = 0;
    
    if (!is_report)
        menu_data(&values, c_item);

    do {
        key = getch();
        switch(key)
        {
            case KEY_UP:
            case KEY_DOWN:
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(1));
                wrefresh(p_windows_menu[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n_values);
                else
                    c_item = (((c_item + n_values) - 1) % n_values);
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(6));
                wrefresh(p_windows_menu[c_item + 1]);
                if (!is_report)
                    menu_data(&values, c_item);
                break;
            case KEY_RETURN:
                if (is_report) {
                    report_data(&values, c_item);
                    redrawwin(p_windows_menu[0]);
                    wrefresh(p_windows_menu[0]);
                    break;
                } else {
                    return c_item;
                }
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    return -1;
}

void Ui::delete_menu(WINDOW **p_windows_menu)
{
    for (int i = 0; i <= n_values; i++)
        delwin(p_windows_menu[i]);
    free(p_windows_menu);
}

void Ui::menu_data(vector<string> **values, int c_item)
{
    werase(window_menu_data);
    
    mvwprintw(window_menu_data, 0, 0, "%s", (**values)[(n_values * 2) + c_item].c_str());

    wrefresh(window_menu_data);
}

void Ui::report_data(vector<string> **values, int c_item)
{
    int key;

    WINDOW *windows_report_details = newwin(LINES, COLS, 0, 0);
    wbkgd(windows_report_details, COLOR_PAIR(1));

    mvwprintw(windows_report_details, 1, 0, "%s", (**values)[(n_values * 4) + c_item].c_str());

    wrefresh(windows_report_details);
    
    do {
        key = wgetch(windows_report_details);
        switch(key)
        {
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    delwin(windows_report_details);
}


void Ui::progress(string p)
{  
    if ((p == "1") || (p == "-1")) {
        for (int i = 0; i < 45; i++)
            mvdelch(32, 23);
    }

    if (p == "-1") {
        attron(COLOR_PAIR(10));
        mvprintw(32, 23, "  DONE  ");
        attroff(COLOR_PAIR(10));
        attron(COLOR_PAIR(11));
        mvhline(32, 33, ACS_VLINE, 32);
        attroff(COLOR_PAIR(11));
    } else {
        attron(COLOR_PAIR(1));
        mvprintw(32, 23, " %s/100 ", p.c_str());
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(9));
        mvhline(32, 33, ACS_VLINE, (stoi(p) / 3));
        attroff(COLOR_PAIR(9));
    }
    
    refresh();
}

void Ui::error(const string err)
{
    touchwin(stdscr);
    refresh();
    
    attron(A_BOLD | COLOR_PAIR(12));
    mvprintw(13, 20, err.c_str());
    attroff(A_BOLD | COLOR_PAIR(12));
}

void Ui::cleanup()
{
    for (int i = 0; i <= n_fields; i++)
        free_field(p_fields[i]);
    unpost_form(*p_form);
    free_form(*p_form);
    if (window_menu_data != NULL)
        delwin(window_menu_data);
}

vector<string> Ui::get_fields_value(vector<int> *i_fields, int i)
{
    int n = i_fields->back();
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
