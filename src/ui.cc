#include <sys/ioctl.h>
#include <signal.h>
#include <iostream>//

#include "ui.h"

void resize_terminal(int sig)
{
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    std::cout << w.ws_row << "X" << w.ws_col << " ";

endwin();
initscr();
refresh();
clear();
refresh();
/*
int x,y;
getmaxyx(stdscr, y, x);

wmove(upScreen, 0, 0);
wmove(downScreen, y/2, 0);
wresize(upScreen, y/2, x);
wresize(downScreen, y/2, x);
wclear(upScreen);
wclear(downScreen);
waddstr(upScreen, "test1");
waddstr(downScreen, "test2");
wrefresh(upScreen);
wrefresh(downScreen);
refresh();
*/
}

Ui::Ui() :
    n_values(0)
{
    initscr();
    noecho();

    keypad(stdscr, TRUE);
    cbreak();
    
    if (getenv("ESCDELAY") == NULL)
        set_escdelay(25);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(6, COLOR_GREEN, COLOR_BLACK);
    }
    
    signal(SIGWINCH, resize_terminal);
}

Ui::~Ui()
{
    cleanup();
    endwin();
}

void Ui::login(std::vector<std::string> *user_configs)
{
    int rows;
    int cols;
    
    fields[0] = new_field(1, 42, 20, (COLS / 2.63), 0, 0);
    fields[1] = new_field(1, 42, 22, (COLS / 2.63), 0, 0);
    fields[2] = new_field(1, 42, 24, (COLS / 2.63), 0, 0);
    fields[3] = new_field(1, 42, 26, (COLS / 2.63), 0, 0);
    fields[4] = new_field(1, 20, 28, 77, 0, 0);
    fields[5] = NULL;
    
    for (int i = 0; i <= 4; i++) {
        if (i == 4)
            field_opts_off(fields[i], O_EDIT);
        else
            set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
    }
    
    set_field_back(fields[0], COLOR_PAIR(1));
    set_field_back(fields[1], COLOR_PAIR(1));
    set_field_back(fields[2], COLOR_PAIR(2));
    set_field_back(fields[3], COLOR_PAIR(3));
    set_field_back(fields[4], COLOR_PAIR(1));
    
    set_field_buffer(fields[0], 0, (*user_configs)[0].c_str());
    set_field_buffer(fields[1], 0, (*user_configs)[1].c_str());
    set_field_buffer(fields[2], 0, (*user_configs)[2].c_str());
    set_field_buffer(fields[3], 0, (*user_configs)[3].c_str());
    set_field_buffer(fields[4], 0, fields_name[4].c_str());
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    post_form(form);
    
    mvprintw(20, (COLS / 2.85), fields_name[0].c_str());
    mvprintw(22, (COLS / 2.85), fields_name[1].c_str());
    mvprintw(24, (COLS / 3.05), fields_name[2].c_str());
    mvprintw(26, (COLS / 3.05), fields_name[3].c_str());
    
    refresh();

    set_current_field(form, fields[2]);
    form_driver(form, REQ_END_LINE);
}

void Ui::main()
{
    int rows;
    int cols;
    
    fields[0] = new_field(1, 42, 7, 23, 0, 0);
    fields[1] = new_field(1, 42, 9, 23, 0, 0);
    fields[2] = new_field(1, 42, 11, 23, 0, 0);
    fields[3] = new_field(1, 20, 13, 34, 0, 0);
    fields[4] = new_field(1, 42, 17, 23, 0, 0);
    fields[5] = new_field(1, 42, 19, 23, 0, 0);
    fields[6] = new_field(1, 42, 21, 23, 0, 0);
    fields[7] = new_field(1, 20, 23, 34, 0, 0);
    fields[8] = new_field(1, 42, 27, 23, 0, 0);
    fields[9] = new_field(1, 42, 29, 23, 0, 0);
    fields[10] = new_field(1, 20, 33, 23, 0, 0);
    fields[11] = new_field(1, 20, 33, 45, 0, 0);
    fields[12] = new_field(1, 42, 37, 23, 0, 0);
    fields[13] = new_field(1, 42, 39, 23, 0, 0);
    fields[14] = new_field(1, 20, 41, 23, 0, 0);
    fields[15] = new_field(1, 20, 41, 45, 0, 0);
    fields[16] = NULL;

    for (int i = 0; i <= 15; i++) {
        if ((i != 0) && (i != 1) && (i != 4))
            field_opts_off(fields[i], O_EDIT);
        if ((i != 3) && (i != 7) &&
            (i != 10) && (i != 11) && (i != 14) && (i != 15))
            set_field_just(fields[i], JUSTIFY_CENTER);
        field_opts_off(fields[i], O_AUTOSKIP);
    }
    
    set_field_back(fields[0], COLOR_PAIR(2));
    set_field_back(fields[1], COLOR_PAIR(1));
    set_field_back(fields[2], COLOR_PAIR(1));
    set_field_back(fields[3], COLOR_PAIR(1));
    set_field_back(fields[4], COLOR_PAIR(1));
    set_field_back(fields[5], COLOR_PAIR(1));
    set_field_back(fields[6], COLOR_PAIR(1));
    set_field_back(fields[7], COLOR_PAIR(1));
    set_field_back(fields[8], COLOR_PAIR(1));
    set_field_back(fields[9], COLOR_PAIR(1));
    set_field_back(fields[10], COLOR_PAIR(1));
    set_field_back(fields[11], COLOR_PAIR(1));
    set_field_back(fields[12], COLOR_PAIR(1));
    set_field_back(fields[13], COLOR_PAIR(1));
    set_field_back(fields[14], COLOR_PAIR(1));
    set_field_back(fields[15], COLOR_PAIR(1));
    
    set_field_buffer(fields[3], 0, fields_name[8].c_str());
    set_field_buffer(fields[7], 0, fields_name[12].c_str());
    set_field_buffer(fields[10], 0, fields_name[16].c_str());
    set_field_buffer(fields[11], 0, fields_name[17].c_str());
    set_field_buffer(fields[14], 0, fields_name[20].c_str());
    set_field_buffer(fields[15], 0, fields_name[21].c_str());
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    post_form(form);

    mvprintw(7, 18, fields_name[5].c_str());
    mvprintw(9, 17, fields_name[6].c_str());
    mvprintw(11, 17, fields_name[7].c_str());
    mvprintw(17, 18, fields_name[9].c_str());
    mvprintw(19, 18, fields_name[10].c_str());
    mvprintw(21, 16, fields_name[11].c_str());
    mvprintw(27, 18, fields_name[13].c_str());
    mvprintw(29, 15, fields_name[14].c_str());
    mvprintw(31, 14, fields_name[15].c_str());
    mvprintw(37, 18, fields_name[18].c_str());
    mvprintw(39, 16, fields_name[19].c_str());
    
    curs_set(1);
    
    refresh();

    form_driver(form, REQ_END_LINE);
}

int Ui::menu(std::vector<std::string> *values, std::size_t n)
{
    int key;
    int row;
    int c_item = 0;
    
    n_values = ((values->size()) / n);
    
    field_info(current_field(form), NULL, NULL, &row, NULL, NULL, NULL);
    if (field_index(current_field(form)) == 13)
        row -= 7;
    else
        --row;
    
    windows_arr = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    
    windows_arr[0] = newwin((n_values + 2), 42, row, 23);
    box(windows_arr[0], 0, 0);
    
    for (std::size_t i = 1; i <= n_values; i++) {
        windows_arr[i] = subwin(windows_arr[0], 1, 40, (i + row), 24);
        mvwprintw(windows_arr[i], 0, 1, "%s", (*values)[n_values + (i - 1)].c_str()); 
    }
    
    wbkgd(windows_arr[1], A_REVERSE);
    wrefresh(windows_arr[0]);
    
    menu_data(&values, c_item, n);
    
    do {
        key = getch();
        switch(key)
        {
            case KEY_RIGHT:
            case KEY_TAB:
                if (menu_data_lines > 36)
                    menu_data_scroll();
                break;
            case KEY_UP:
            case KEY_DOWN:
                wbkgd(windows_arr[c_item + 1], COLOR_PAIR(2));
                wrefresh(windows_arr[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n_values);
                else
                    c_item = (((c_item + n_values) - 1) % n_values);
                wbkgd(windows_arr[c_item + 1], A_REVERSE);
                wrefresh(windows_arr[c_item + 1]);
                menu_data(&values, c_item, n);
                break;
            case KEY_RETURN:
                return c_item;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);

    return -1;
}

void Ui::menu_data(std::vector<std::string> **values, int c_item, std::size_t n)
{
    menu_data_lines = 36;
    long data_lines = 0;

    for (std::size_t i = 2; i < n; i++)
        for (std::size_t ii = 0; ii < (**values)[(n_values * i) + c_item].size(); ii++)
            if ((**values)[(n_values * i) + c_item][ii] == '\n')
                ++data_lines;
    
    if (data_lines > menu_data_lines)
        menu_data_lines = data_lines;

    if (window_menu_data != NULL) 
        delwin(window_menu_data);
    
    window_menu_data = newpad(menu_data_lines, 63);
    
    for (std::size_t i = 2; i < n; i++)
        wprintw(window_menu_data, "%s", ((**values)[(n_values * i) + c_item]).c_str());
    
    prefresh(window_menu_data, 0, 0, 7, 109, 41, 171);
}

void Ui::menu_data_scroll()
{
    int key;
    int c_line = 0;
    
    mvaddch(23, 106, ACS_UARROW);
    mvaddch(24, 106, ACS_DARROW);
    refresh();
    
    keypad(window_menu_data, true);
    
    do {
        prefresh(window_menu_data, c_line, 0, 7, 109, 41, 171);
        key = wgetch(window_menu_data);
        switch(key)
        {
            case KEY_LEFT:
            case KEY_TAB:
                key = KEY_ESCAPE;
                break;
            case KEY_UP:
                if (c_line <= 0)
                    continue;
                c_line--;
                break;
            case KEY_DOWN:
                if ((35 + c_line) >= menu_data_lines)
                    continue;
                c_line++;
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    mvprintw(23, 106, " ");
    mvprintw(24, 106, " ");
}

int Ui::report(std::vector<std::string> *values, std::size_t n)
{
    int key;
    unsigned int c_item = 0;

    n_values = ((values->size()) / n);
    
    windows_arr = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    
    windows_arr[0] = newpad((n_values + 35), COLS);
    keypad(windows_arr[0], true);
    
    for (std::size_t i = 1; i <= n_values; i++) {
        windows_arr[i] = subpad(windows_arr[0], 1, (COLS - 4), (i - 1), 0);
        
        mvwprintw(windows_arr[i], 0, 0, "%s", (*values)[n_values + i].c_str());
        mvwprintw(windows_arr[i], 0, 129, "%s", (*values)[(n_values * 2) + i].c_str());
        mvwprintw(windows_arr[i], 0, 146, "%s", (*values)[(n_values * 3) + i].c_str());
        
        if ((stoi((*values)[(n_values * 4) + i])) >= 7) { 
            wattron(windows_arr[i], COLOR_PAIR(4));
            mvwprintw(windows_arr[i], 0, 165, "%s", (*values)[(n_values * 4) + i].c_str());
            wattroff(windows_arr[i], COLOR_PAIR(4));
        } else if (((stoi((*values)[(n_values * 4) + i])) >= 4) &&
                   ((stoi((*values)[(n_values * 4) + i])) < 7)) { 
            wattron(windows_arr[i], COLOR_PAIR(5));
            mvwprintw(windows_arr[i], 0, 165, "%s", (*values)[(n_values * 4) + i].c_str());
            wattroff(windows_arr[i], COLOR_PAIR(5));
        } else {
            wattron(windows_arr[i], COLOR_PAIR(2));
            mvwprintw(windows_arr[i], 0, 165, "%s", (*values)[(n_values * 4) + i].c_str());
            wattroff(windows_arr[i], COLOR_PAIR(2));
        }
    }
    
    wbkgd(windows_arr[1], COLOR_PAIR(2));
    wbkgd(windows_arr[1], A_REVERSE);
    
    prefresh(windows_arr[0], 0, 0, 7, 3, 41, (COLS - 4));

    do {
        prefresh(windows_arr[0], c_item, 0, 7, 3, 41, (COLS - 4));
        key = wgetch(windows_arr[0]);
        switch(key)
        {
            case KEY_UP:
                if (c_item > 0) {
                    wbkgd(windows_arr[c_item + 1], COLOR_PAIR(2));
                    c_item = (((c_item + n_values) - 1) % n_values);
                }
                break;
            case KEY_DOWN:
                if (c_item < (n_values - 1)) {
                    c_item = ((c_item + 1) % n_values);
                    wbkgd(windows_arr[c_item + 1], A_REVERSE);
                }
                break;
            case KEY_RIGHT:
            case KEY_RETURN:
                report_data(&values, (c_item + 1), n);
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    return -1;
}

void Ui::report_data(std::vector<std::string> **values, unsigned int c_item,
                     std::size_t n)
{
    int key;
    int c_line = 0;
    long report_data_lines = 36;
    long data_lines = 2;

    for (std::size_t i = 5; i < n; i++)
        for (std::size_t ii = 0; ii < (**values)[(n_values * i) + c_item].size(); ii++)
            if ((**values)[(n_values * i) + c_item][ii] == '\n')
                ++data_lines;

    if (data_lines > report_data_lines)
        report_data_lines = data_lines;
    
    WINDOW *window_report_data = newpad(report_data_lines, (COLS - 4));
    keypad(window_report_data, true);
    
    for (std::size_t i = 5; i < n; i++)
        wprintw(window_report_data, "%s", ((**values)[(n_values * i) + c_item]).c_str());
    
    do {
        prefresh(window_report_data, c_line, 0, 7, 3, 41, (COLS - 4));
        key = wgetch(window_report_data);
        switch(key)
        {
            case KEY_LEFT:
                key = KEY_ESCAPE;
                break;
            case KEY_UP:
                if (c_line <= 0)
                    continue;
                c_line--;
                break;
            case KEY_DOWN:
                if ((37 + c_line) >= report_data_lines)
                    continue;
                c_line++;
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    delwin(window_report_data);
}

void Ui::progress(std::string p)
{  
    if ((p == "-1") || (p == "-2") || (p == "-3")) {
        for (int i = 0; i <= 45; i++)
            mvdelch(31, 23);
        if (p == "-1")
            status("TASK FINISHED");
        else if (p == "-2")
            status("TASK STOPED");
        else
            status("TASK PAUSED");
    } else {
        mvprintw(31, 23, " %s/100 ", p.c_str());
        mvhline(31, 33, ACS_VLINE, (stoi(p) / 3));
    }
    
    refresh();
}

void Ui::status(std::string sts)
{
    int start_x = 0;
    int max_y;
    int max_x;
    WINDOW *window_status;

    window_status = newwin(1, COLS, (LINES - 1), 0);
    wbkgd(window_status, COLOR_PAIR(1));
    
    getmaxyx(window_status, max_y, max_x);
    start_x = ((max_x - sts.size()) / 2); 
    max_y = 0;
    mvwprintw(window_status, max_y, start_x, sts.c_str());
    
    wrefresh(window_status);
    delwin(window_status);
}

void Ui::marker(bool is_menu, bool show)
{
    int row;
    int col;
    int cols;
    
    field_info(current_field(form), NULL, &cols, &row, &col, NULL, NULL);

    if (show) {
        if (is_menu) {
            mvprintw(row, 66, "+");
        } else {
            mvaddch(row, col, ACS_CKBOARD);
            mvaddch(row, (col + (cols - 1)), ACS_CKBOARD);
        }
        curs_set(0);
    } else {
        if (is_menu) {
            mvprintw(row, 66, " ");
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(row, col, " ");
            mvprintw(row, (col + (cols - 1)), " ");
            attroff(COLOR_PAIR(1));
        }
        curs_set(1);
    }
}

void Ui::delete_windows_arr()
{
    for (std::size_t i = 1; i < (n_values + 1); i++)
        delwin(windows_arr[i]);
    delwin(windows_arr[0]);
    free(windows_arr);
    
    if (window_menu_data != NULL) {
        delwin(window_menu_data);
        window_menu_data = NULL;
    }
}

void Ui::cleanup()
{
    for (int i = 0; i <= (field_count(form) - 1); i++)
        free_field(fields[i]);
    unpost_form(form);
    free_form(form);
}
