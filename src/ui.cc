//#include <iostream>//

#include "ui.h"

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
        init_pair(6, COLOR_BLUE, COLOR_BLACK);
        init_pair(7, COLOR_GREEN, COLOR_BLACK);
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
    fields_login[4] = new_field(1, 20, 28, 77, 0, 0);
    fields_login[5] = NULL;
    
    for (std::size_t i = 0; i <= n_fields; i++) {
        if (i == 4)
            field_opts_off(fields_login[i], O_EDIT);
        else
            set_field_just(fields_login[i], JUSTIFY_CENTER);
        field_opts_off(fields_login[i], O_AUTOSKIP);
    }
    
    set_field_back(fields_login[0], COLOR_PAIR(1));
    set_field_back(fields_login[1], COLOR_PAIR(1));
    set_field_back(fields_login[2], COLOR_PAIR(2));
    set_field_back(fields_login[3], COLOR_PAIR(3));
    set_field_back(fields_login[4], COLOR_PAIR(1));
    
    set_field_buffer(fields_login[0], 0, "localhost");
    set_field_buffer(fields_login[1], 0, "9390");
    set_field_buffer(fields_login[2], 0, "user");              // TODO: BORRAR
    set_field_buffer(fields_login[3], 0, "ak474747**OPENVAS"); // TODO: BORAR
    set_field_buffer(fields_login[4], 0, fields_name[4].c_str());
    
    form_login = new_form(fields_login);
    scale_form(form_login, &rows, &cols);
    post_form(form_login);
    
    mvprintw(20, 61, fields_name[0].c_str());
    mvprintw(22, 61, fields_name[1].c_str());
    mvprintw(24, 57, fields_name[2].c_str());
    mvprintw(26, 57, fields_name[3].c_str());
    
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
    
    fields_main[0] = new_field(1, 42, 7, 23, 0, 0);
    fields_main[1] = new_field(1, 42, 9, 23, 0, 0);
    fields_main[2] = new_field(1, 42, 11, 23, 0, 0);
    fields_main[3] = new_field(1, 20, 13, 34, 0, 0);
    fields_main[4] = new_field(1, 42, 17, 23, 0, 0);
    fields_main[5] = new_field(1, 42, 19, 23, 0, 0);
    fields_main[6] = new_field(1, 42, 21, 23, 0, 0);
    fields_main[7] = new_field(1, 20, 23, 34, 0, 0);
    fields_main[8] = new_field(1, 42, 27, 23, 0, 0);
    fields_main[9] = new_field(1, 42, 29, 23, 0, 0);
    fields_main[10] = new_field(1, 20, 33, 23, 0, 0);
    fields_main[11] = new_field(1, 20, 33, 45, 0, 0);
    fields_main[12] = new_field(1, 42, 37, 23, 0, 0);
    fields_main[13] = new_field(1, 42, 39, 23, 0, 0);
    fields_main[14] = new_field(1, 20, 41, 23, 0, 0);
    fields_main[15] = new_field(1, 20, 41, 45, 0, 0);
    fields_main[16] = NULL;

    for (std::size_t i = 0; i <= n_fields; i++) {
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
    set_field_back(fields_main[3], COLOR_PAIR(1));
    set_field_back(fields_main[4], COLOR_PAIR(1));
    set_field_back(fields_main[5], COLOR_PAIR(1));
    set_field_back(fields_main[6], COLOR_PAIR(1));
    set_field_back(fields_main[7], COLOR_PAIR(1));
    set_field_back(fields_main[8], COLOR_PAIR(1));
    set_field_back(fields_main[9], COLOR_PAIR(1));
    set_field_back(fields_main[10], COLOR_PAIR(1));
    set_field_back(fields_main[11], COLOR_PAIR(1));
    set_field_back(fields_main[12], COLOR_PAIR(1));
    set_field_back(fields_main[13], COLOR_PAIR(1));
    set_field_back(fields_main[14], COLOR_PAIR(1));
    set_field_back(fields_main[15], COLOR_PAIR(1));
    
    set_field_buffer(fields_main[3], 0, fields_name[8].c_str());
    set_field_buffer(fields_main[7], 0, fields_name[12].c_str());
    set_field_buffer(fields_main[10], 0, fields_name[16].c_str());
    set_field_buffer(fields_main[11], 0, fields_name[17].c_str());
    set_field_buffer(fields_main[14], 0, fields_name[20].c_str());
    set_field_buffer(fields_main[15], 0, fields_name[21].c_str());
    
    form_main = new_form(fields_main);
    scale_form(form_main, &rows, &cols);
    post_form(form_main);

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

    form_driver(form_main, REQ_END_LINE);
}

int Ui::menu(std::vector<std::string> *values, std::size_t n)
{
    int key;
    int row;
    unsigned int c_item = 0;
    
    n_values = ((values->size()) / n);
    
    field_info(current_field(*p_form), NULL, NULL, &row, NULL, NULL, NULL);
    if (field_index(current_field(form_main)) == 13)
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

void Ui::menu_data(std::vector<std::string> **values, unsigned int c_item,
                               std::size_t n)
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
            wattron(windows_arr[i], COLOR_PAIR(6));
            mvwprintw(windows_arr[i], 0, 165, "%s", (*values)[(n_values * 4) + i].c_str());
            wattroff(windows_arr[i], COLOR_PAIR(6));
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
            status(std::make_pair("TASK FINISHED", 7));
        else if (p == "-2")
            status(std::make_pair("TASK STOPED", 5));
        else
            status(std::make_pair("TASK PAUSED", 5));
    } else {
        mvprintw(31, 23, " %s/100 ", p.c_str());
        mvhline(31, 33, ACS_VLINE, (stoi(p) / 3));
    }
    
    refresh();
}

void Ui::status(std::pair<std::string, int> sts, bool is_login)
{
    int start_x = 0;
    int max_y;
    int max_x;
    WINDOW *window_status;

    if (is_login) {
        window_status = newwin(1, 63, 18, 55);
        getmaxyx(window_status, max_y, max_x);
        start_x = ((max_x - (sts.first.size() + 8)) / 2); 
    } else {
        window_status = newwin(35, 63, 7, 109);
    }
    max_y = 0;

    wattron(window_status, COLOR_PAIR(sts.second));
    mvwprintw(window_status, max_y, start_x, "[STATUS]");
    wattroff(window_status, COLOR_PAIR(sts.second));
    mvwprintw(window_status, max_y, (start_x + 9), sts.first.c_str());
    
    wrefresh(window_status);
    delwin(window_status);
}

void Ui::marker(bool is_menu, bool show)
{
    int row;
    int col;
    int cols;
    
    field_info(current_field(*p_form), NULL, &cols, &row, &col, NULL, NULL);

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
    for (std::size_t i = 0; i <= n_fields; i++)
        free_field(p_fields[i]);
    unpost_form(*p_form);
    free_form(*p_form);
}
