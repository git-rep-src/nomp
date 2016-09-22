#include "ui.h"

using namespace std;

Ui::Ui()
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
    
    //driver();
}

void Ui::main()
{
    int rows;
    int cols;
     
    p_fields = &fields[0];
    p_form = &form;
    n_fields = ((sizeof(fields) / sizeof(fields[0])) - 2);
    
    window = newwin((LINES - 24), 62, ((LINES - 25) / 2), ((COLS - 63) / 2));
    wbkgd(window, COLOR_PAIR(2));
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
    
    wattron(window, A_BOLD | COLOR_PAIR(2));
    mvwprintw(window, 3, 9,  "  NAME");
    mvwprintw(window, 5, 9,  " HOSTS");
    mvwprintw(window, 7, 9,  " PORTS");
    mvwprintw(window, 12, 9, "  NAME");
    mvwprintw(window, 14, 9, "TARGET");
    mvwprintw(window, 16, 9, "  SCAN");
    wattroff(window, A_BOLD | COLOR_PAIR(2));
    
    wrefresh(stdscr);
    wrefresh(window);

    set_field_back(fields[0], COLOR_PAIR(3));
    form_driver(*p_form, REQ_END_LINE);
}

/*
WINDOW **create_menu(char ***p_arr, int rows)
{
    WINDOW **windows_menu;
    windows_menu = (WINDOW **) malloc ((n + 1) * sizeof(WINDOW *));
    
    windows_menu[0] = newwin(n, 30, rows, 72);

    wbkgd(windows_menu[0], COLOR_PAIR(1)); 
    
    for (int i = 0; i < n; i++) {
        windows_menu[i + 1] = subwin(windows_menu[0], 1, 30, (i + rows), 72);
        //wprintw(windows_menu[i + 1], " %s", (*p_arr)[i] += 37);
        wprintw(windows_menu[i + 1], " %s", (*p_arr)[i]);
    }

    wbkgd(windows_menu[1], COLOR_PAIR(3));
    
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
*/
