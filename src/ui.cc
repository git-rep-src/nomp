#include <string.h>

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

    if (has_colors()) {
        start_color();
        init_color(COLOR_CYAN, 150, 150, 150);
        init_color(COLOR_BLUE, 80, 80, 80);
        init_color(COLOR_GREEN, 0, 200, 0);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_CYAN);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_WHITE, COLOR_GREEN);
        init_pair(5, COLOR_WHITE, COLOR_RED);
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
   
    window = newwin((LINES - 34), 62, ((LINES - 14) / 2), ((COLS - 63) / 2));
    wbkgd(window, COLOR_PAIR(2));
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
    
    wattron(window, A_BOLD | COLOR_PAIR(2));
    mvwprintw(window, 3, 7, "    HOST");
    mvwprintw(window, 5, 7, "    PORT");
    mvwprintw(window, 7, 7, "    USER");
    mvwprintw(window, 9, 7, "PASSWORD");
    wattroff(window, A_BOLD | COLOR_PAIR(2));
    
    wrefresh(stdscr);
    wrefresh(window);

    set_field_back(fields_login[0], COLOR_PAIR(3));
    form_driver(*p_form, REQ_END_LINE);
}

void Ui::main()
{
    int rows;
    int cols;
     
    p_fields = &fields_main[0];
    p_form = &form_main;
    n_fields = ((sizeof(fields_main) / sizeof(fields_main[0])) - 2);
    
    window = newwin((LINES - 24), 62, ((LINES - 25) / 2), ((COLS - 63) / 2));
    wbkgd(window, COLOR_PAIR(2));
    keypad(window, TRUE);

    fields_main[0] = new_field(1, 30, 1, 14, 0, 0);
    fields_main[1] = new_field(1, 30, 3, 14, 0, 0);
    fields_main[2] = new_field(1, 30, 5, 14, 0, 0);
    fields_main[3] = new_field(1, 30, 10, 14, 0, 0);
    fields_main[4] = new_field(1, 30, 12, 14, 0, 0);
    fields_main[5] = new_field(1, 30, 14, 14, 0, 0);
    fields_main[6] = NULL;
    
    for (int i = 0; i <= n_fields; i++) {
        set_field_just(fields_main[i], JUSTIFY_CENTER);
        field_opts_off(fields_main[i], O_AUTOSKIP);
    }
    field_opts_off(fields_main[2], O_EDIT);
    field_opts_off(fields_main[4], O_EDIT);
    field_opts_off(fields_main[5], O_EDIT);
    
    form_main = new_form(fields_main);
    scale_form(form_main, &rows, &cols);
    
    set_form_win(form_main, window);
    set_form_sub(form_main, derwin(window, rows, cols, 2, 2));

    post_form(form_main);
    
    wattron(window, A_BOLD | COLOR_PAIR(2));
    mvwprintw(window, 3, 9,  "  NAME");
    mvwprintw(window, 5, 9,  " HOSTS");
    mvwprintw(window, 7, 9,  " PORTS");
    mvwprintw(window, 12, 9, "  NAME");
    mvwprintw(window, 14, 9, "  SCAN");
    mvwprintw(window, 16, 9, "TARGET");
    wattroff(window, A_BOLD | COLOR_PAIR(2));
    
    wrefresh(stdscr);
    wrefresh(window);

    set_field_back(fields_main[0], COLOR_PAIR(3));
    form_driver(*p_form, REQ_END_LINE);
}

WINDOW **Ui::create_menu(vector<string> *values, int rows)
{
    n = ((values->size()) / 2);
    
    WINDOW **windows_menu;
    windows_menu = (WINDOW **) malloc ((n + 1) * sizeof(WINDOW *));
    
    windows_menu[0] = newwin(n, 30, rows, 72);

    wbkgd(windows_menu[0], COLOR_PAIR(1)); 
    
    for (int i = 0; i < n; i++) {
        windows_menu[i + 1] = subwin(windows_menu[0], 1, 30, (i + rows), 72);
        wprintw(windows_menu[i + 1], " %s", (*values)[i].c_str());
    }

    wbkgd(windows_menu[1], COLOR_PAIR(3));
    
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
                wnoutrefresh(p_windows_menu[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n);
                else
                    c_item = (((c_item + n) - 1) % n);
                wbkgd(p_windows_menu[c_item + 1], COLOR_PAIR(3));
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

void Ui::error(const string err)
{
    touchwin(stdscr);
    touchwin(window);
    wrefresh(stdscr);
    
    wattron(window, A_BOLD | COLOR_PAIR(5));
    mvwprintw(window, 13, 20, err.c_str());
    wattroff(window, A_BOLD | COLOR_PAIR(5));
}

void Ui::cleanup()
{
    unpost_form(*p_form);
    free_form(*p_form);
    for (int i = 0; i <= n_fields; i++)
        free_field(p_fields[i]);
    delwin(window);
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
