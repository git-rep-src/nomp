#include "ui.h"

#include <sstream>
#include <iomanip>

#include <iostream>//

using std::stringstream;

Ui::Ui() :
    has_status(false),
    n_values(0)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    if (getenv("ESCDELAY") == NULL)
        set_escdelay(25);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
    }
    
    window = newpad(49, 175);
    keypad(window, TRUE);

    field_width = (175 / 4.1);
    button_width = (175 / 8.6);

    fill_field_names();
   
    refresh();
}

Ui::~Ui()
{
    clear_form();
    delwin(window);
    endwin();
}

void Ui::authenticate(const vector<string> *user_configs)
{
    int rows;
    int cols;
    
    start_x = (COLS / 2.63);
    start_y = (LINES / 2.45);
    
    fields[0] = new_field(1, field_width,  start_y,       start_x, 0, 0);
    fields[1] = new_field(1, field_width,  (start_y + 2), start_x, 0, 0);
    fields[2] = new_field(1, field_width,  (start_y + 4), start_x, 0, 0);
    fields[3] = new_field(1, field_width,  (start_y + 6), start_x, 0, 0);
    fields[4] = new_field(1, button_width, (start_y + 8), (start_x + 11), 0, 0);
    fields[5] = NULL;
    
    set_field_just(fields[0], JUSTIFY_CENTER);
    set_field_just(fields[1], JUSTIFY_CENTER);
    set_field_just(fields[2], JUSTIFY_CENTER);
    field_opts_off(fields[4], O_EDIT);
    
    for (int i = 0; i <= 4; i++)
        field_opts_off(fields[i], O_AUTOSKIP);
    
    set_field_back(fields[0], COLOR_PAIR(1));
    set_field_back(fields[1], COLOR_PAIR(1));
    set_field_back(fields[2], COLOR_PAIR(2));
    set_field_back(fields[3], COLOR_PAIR(3));
    set_field_back(fields[4], COLOR_PAIR(1));
    
    set_field_buffer(fields[0], 0, (*user_configs)[0].c_str());
    set_field_buffer(fields[1], 0, (*user_configs)[1].c_str());
    set_field_buffer(fields[2], 0, (*user_configs)[2].c_str());
    set_field_buffer(fields[3], 0, (*user_configs)[3].c_str());
    set_field_buffer(fields[4], 0, field_names[4].c_str());
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 0, 0));
    post_form(form);
    
    mvwprintw(window, start_y,       (start_x - 5), field_names[0].c_str());
    mvwprintw(window, (start_y + 2), (start_x - 5), field_names[1].c_str());
    mvwprintw(window, (start_y + 4), (start_x - 9), field_names[2].c_str());
    mvwprintw(window, (start_y + 6), (start_x - 9), field_names[3].c_str());

    prefresh(window, 0, 0, 0, 0, (LINES - 1), (COLS - 1));
    
    set_current_field(form, fields[2]);
    form_driver(form, REQ_END_LINE);
}

void Ui::main()
{
    int rows;
    int cols;

    start_x = (COLS / 7.6);
    start_y = (LINES / 10);

    fields[0]  = new_field(1, field_width,  start_y,        start_x, 0, 0);
    fields[1]  = new_field(1, field_width,  (start_y + 2),  start_x, 0, 0);
    fields[2]  = new_field(1, field_width,  (start_y + 4),  start_x, 0, 0);
    fields[3]  = new_field(1, field_width,  (start_y + 6),  start_x, 0, 0);
    fields[4]  = new_field(1, button_width, (start_y + 8),  (start_x + 11), 0, 0);
    fields[5]  = new_field(1, field_width,  (start_y + 12), start_x, 0, 0);
    fields[6]  = new_field(1, field_width,  (start_y + 14), start_x, 0, 0);
    fields[7]  = new_field(1, field_width,  (start_y + 16), start_x, 0, 0);
    fields[8]  = new_field(1, field_width,  (start_y + 18), start_x, 0, 0);
    fields[9]  = new_field(1, button_width, (start_y + 20), (start_x + 11), 0, 0);
    fields[10] = new_field(1, field_width,  (start_y + 24), start_x, 0, 0);
    fields[11] = new_field(1, field_width,  (start_y + 26), start_x, 0, 0);
    fields[12] = new_field(1, button_width, (start_y + 30), start_x, 0, 0);
    fields[13] = new_field(1, button_width, (start_y + 30), (start_x + 22), 0, 0);
    fields[14] = new_field(1, field_width,  (start_y + 34), start_x, 0, 0);
    fields[15] = new_field(1, field_width,  (start_y + 36), start_x, 0, 0);
    fields[16] = new_field(1, field_width,  (start_y + 38), start_x, 0, 0);
    fields[17] = new_field(1, button_width, (start_y + 40), start_x, 0, 0);
    fields[18] = new_field(1, button_width, (start_y + 40), (start_x + 22), 0, 0);
    fields[19] = NULL;

    for (int i = 0; i <= 18; i++) {
        if ((i != 4) && (i != 9) &&
            (i != 12) && (i != 13) && (i != 17) && (i != 18))
            set_field_just(fields[i], JUSTIFY_CENTER);
        if ((i != 0) && (i != 1) && (i != 2) && (i != 5) && (i != 6))
            field_opts_off(fields[i], O_EDIT);
        field_opts_off(fields[i], O_AUTOSKIP);
        if (i == 0)
            set_field_back(fields[i], COLOR_PAIR(2));
        else
            set_field_back(fields[i], COLOR_PAIR(1));
    }
    
    set_field_buffer(fields[4],  0, field_names[9].c_str());
    set_field_buffer(fields[9],  0, field_names[14].c_str());
    set_field_buffer(fields[12], 0, field_names[18].c_str());
    set_field_buffer(fields[13], 0, field_names[20].c_str());
    set_field_buffer(fields[17], 0, field_names[24].c_str());
    set_field_buffer(fields[18], 0, field_names[25].c_str());
    
    form = new_form(fields);
    scale_form(form, &rows, &cols);
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 0, 0));
    post_form(form);

    mvwprintw(window, start_y,        (start_x - 5), field_names[5].c_str());
    mvwprintw(window, (start_y + 2),  (start_x - 8), field_names[6].c_str());
    mvwprintw(window, (start_y + 4),  (start_x - 6), field_names[7].c_str());
    mvwprintw(window, (start_y + 6),  (start_x - 6), field_names[8].c_str());
    mvwprintw(window, (start_y + 12), (start_x - 5), field_names[10].c_str());
    mvwprintw(window, (start_y + 14), (start_x - 8), field_names[11].c_str());
    mvwprintw(window, (start_y + 16), (start_x - 5), field_names[12].c_str());
    mvwprintw(window, (start_y + 18), (start_x - 7), field_names[13].c_str());
    mvwprintw(window, (start_y + 24), (start_x - 5), field_names[15].c_str());
    mvwprintw(window, (start_y + 26), (start_x - 8), field_names[16].c_str());
    mvwprintw(window, (start_y + 28), (start_x - 9), field_names[17].c_str());
    mvwprintw(window, (start_y + 34), (start_x - 5), field_names[21].c_str());
    mvwprintw(window, (start_y + 36), (start_x - 7), field_names[22].c_str());
    mvwprintw(window, (start_y + 38), (start_x - 7), field_names[23].c_str());
    
    curs_set(1);

    prefresh(window, 0, 0, 0, 0, (LINES - 1), (COLS - 1));

    if (LINES < 49)
        status("USE (CTRL + U) AND (CTRL + D) TO UP AND DOWN SCREEN", 0);

    form_driver(form, REQ_END_LINE);
}

int Ui::menu(const vector<string> *values, size_t n, int n_tabs)
{
    int key;
    int row;
    int c_item = 0;

    n_values = ((values->size()) / n);
    
    field_info(current_field(form), NULL, NULL, &row, NULL, NULL, NULL);
    if (field_index(current_field(form)) == 16)
        row = row - (n_tabs + 11);
    else
        row = row - (n_tabs + 1);
    
    items = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    items[0] = newwin((n_values + 2), field_width, row, start_x);
    box(items[0], 0, 0);
    
    for (size_t i = 1; i <= n_values; i++) {
        items[i] = subwin(items[0], 1, (field_width - 2), (i + row), (start_x + 1));
        mvwprintw(items[i], 0, 1, "%s", (*values)[n_values + (i - 1)].c_str()); 
    }
    
    wbkgd(items[1], A_REVERSE);
    wrefresh(items[0]);
    
    menu_details(&values, c_item, n);
    
    do {
        key = getch();
        switch(key)
        {
            case KEY_UP:
            case KEY_DOWN:
                wbkgd(items[c_item + 1], COLOR_PAIR(2));
                wrefresh(items[c_item + 1]);
                if (key == KEY_DOWN)
                    c_item = ((c_item + 1) % n_values);
                else
                    c_item = (((c_item + n_values) - 1) % n_values);
                wbkgd(items[c_item + 1], A_REVERSE);
                wrefresh(items[c_item + 1]);
                menu_details(&values, c_item, n);
                break;
            case KEY_RETURN:
                if (field_index(current_field(form)) == 10) {
                    if ((((*values)[(n_values * 9) + c_item]).find("New") != string::npos) ||
                        (((*values)[(n_values * 9) + c_item]).find("Done") != string::npos))
                        set_field_buffer(fields[12], 0, field_names[18].c_str());
                    else
                        set_field_buffer(fields[12], 0, field_names[19].c_str());
                }
                return c_item;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);

    return -1;
}

void Ui::menu_details(const vector<string> **values, int c_item, size_t n)
{
    int md_start_x = (COLS / 1.76);
    
    if (md_start_x < (start_x + field_width + 8))
        md_start_x = (start_x + field_width + 8);

    WINDOW *w = newwin((start_y + 37), (175 / 2.40), start_y, md_start_x);

    for (size_t i = 2; i < n; i++)
        wprintw(w, "%s", ((**values)[(n_values * i) + c_item]).c_str());
    
    wrefresh(w);
    delwin(w);
}

int Ui::report(const vector<string> *values, size_t n)
{
    int key;
    unsigned int c_item = 0;

    n_values = ((values->size()) / n);
    
    WINDOW *w = newwin(LINES, COLS, 0, 0); 
    wrefresh(w);

    items = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    items[0] = newpad((n_values + (start_y + 40)), COLS);
    keypad(items[0], true);
    
    for (size_t i = 1; i <= n_values; i++) {
        items[i] = subpad(items[0], 1, (COLS - 5), (i - 1), 0);
        mvwprintw(items[i], 0, 0, "%s", (*values)[n_values + i].c_str());
        mvwprintw(items[i], 0, (COLS - 50), "%s", (*values)[(n_values * 2) + i].c_str());
        mvwprintw(items[i], 0, (COLS - 33), "%s", (*values)[(n_values * 3) + i].c_str());
        mvwprintw(items[i], 0, (COLS - 15), "%s", (*values)[(n_values * 4) + i].c_str());
    }
    
    wbkgd(items[1], COLOR_PAIR(2));
    wbkgd(items[1], A_REVERSE);
    
    prefresh(items[0], 0, 0, start_y, 5, (LINES - start_y), (COLS - 5));

    do {
        prefresh(items[0], c_item, 0, start_y, 5, (LINES - start_y), (COLS - 5));
        key = wgetch(items[0]);
        switch(key)
        {
            case KEY_LEFT:
                key = KEY_ESCAPE;
                break;
            case KEY_UP:
                if (c_item > 0) {
                    wbkgd(items[c_item + 1], COLOR_PAIR(2));
                    c_item = (((c_item + n_values) - 1) % n_values);
                }
                break;
            case KEY_DOWN:
                if (c_item < (n_values - 1)) {
                    c_item = ((c_item + 1) % n_values);
                    wbkgd(items[c_item + 1], A_REVERSE);
                }
                break;
            case KEY_RIGHT:
            case KEY_RETURN:
                report_details(&values, (c_item + 1), n);
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    delwin(w);

    return -1;
}

void Ui::report_details(const vector<string> **values, unsigned int c_item, size_t n)
{
    int key;
    int c_line = 0;
    int details_lines = 2;
    int height = (LINES - (start_y * 2));

    for (size_t i = 5; i < n; i++)
        for (size_t ii = 0; ii < (**values)[(n_values * i) + c_item].size(); ii++)
            if ((**values)[(n_values * i) + c_item][ii] == '\n')
                ++details_lines;

    if (details_lines > height)
        height = details_lines;
    
    WINDOW *w = newpad(height, (COLS - 5));
    keypad(w, true);
    
    for (size_t i = 5; i < n; i++)
        wprintw(w, "%s", ((**values)[(n_values * i) + c_item]).c_str());
    
    do {
        prefresh(w, c_line, 0, start_y, 5, (LINES - start_y), (COLS - 5));
        key = wgetch(w);
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
                if (((LINES - ((start_y * 2) - 4)) + c_line) >= height)
                    continue;
                c_line++;
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    delwin(w);
}

void Ui::marker(bool is_menu, bool show)
{
    int row;
    int col;
    int cols;
    
    field_info(current_field(form), NULL, &cols, &row, &col, NULL, NULL);

    if (show) {
        if (is_menu) {
            mvwprintw(window, row, ((start_x + field_width) + 1), "+");
        } else {
            mvwaddch(window, row, col, ACS_CKBOARD);
            mvwaddch(window, row, (col + (cols - 1)), ACS_CKBOARD);
        }
        curs_set(0);
    } else {
        if (is_menu) {
            mvwprintw(window, row, ((start_x + field_width) + 1), " ");
        } else {
            wattron(window, COLOR_PAIR(1));
            mvwprintw(window ,row, col, " ");
            mvwprintw(window ,row, (col + (cols - 1)), " ");
            wattroff(window, COLOR_PAIR(1));
        }
        curs_set(1);
    }
}

void Ui::status(const string &sts, int n_tabs)
{
    int sts_x = (COLS - sts.size()) / 2; 
    
    mvwprintw(window, (0 + n_tabs), sts_x, sts.c_str());
    
    prefresh(window, 0, 0, 0, 0, (LINES - 1), (COLS - 1));
    
    has_status = true;
}

void Ui::progress(const string &p, int n_tabs)
{  
    for (int i = 0; i <= field_width; i++)
        mvwdelch(window, (start_y + 28), start_x);
    
    if (p == "-1") {
        status("TASK FINISHED", n_tabs);
    } else if (p == "-2") {    
        status("TASK STOPPED", n_tabs);
    } else {
        if ((stoi(p) > 0) && (stoi(p) < 3))
            mvwhline(window, (start_y + 28), start_x, ACS_VLINE, 1);
        else
            mvwhline(window, (start_y + 28), start_x, ACS_VLINE, (stoi(p) / 2.38));
    }
    
    prefresh(window, 0, 0, 0, 0, (LINES - 1), (COLS - 1));
}

int Ui::save()
{
    int key;
    int curs_state = curs_set(0);
    
    WINDOW *w = newwin(11, 41, ((LINES / 2) - 6), ((COLS / 2) - 21));
    wbkgd(w, COLOR_PAIR(1));
    box(w, 0, 0);
    keypad(w, TRUE);

    mvwprintw(w, 5, 3, "SAVE THE RUNNING TASK CONFIG? (Y/N)");
    
    curs_set(0);
    
    wrefresh(w);
    
    do {
        key = wgetch(w);
        switch(key)
        {
            case KEY_Y:
                return 1;
                break;
            case KEY_N:
                return 0;
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);

    delwin(w);

    curs_set(curs_state);

    return -1;
}

void Ui::fill_field_names()
{
    stringstream ss;
    
    field_names.push_back("HOST");
    field_names.push_back("PORT");
    field_names.push_back("USERNAME");
    field_names.push_back("PASSWORD");
    
    ss << std::setw(button_width / 2.8) << std::setfill(' ') << "" << "LOGIN";
    field_names.push_back(ss.str());

    field_names.push_back("NAME");
    field_names.push_back("COMMENT");
    field_names.push_back("HOSTS");
    field_names.push_back("PORTS");
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 4) << std::setfill(' ') << "" << "NEW TARGET";
    field_names.push_back(ss.str());
    
    field_names.push_back("NAME");
    field_names.push_back("COMMENT");
    field_names.push_back("SCAN");
    field_names.push_back("TARGET");
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 3.3) << std::setfill(' ') << "" << "NEW TASK";
    field_names.push_back(ss.str());

    field_names.push_back("TASK");
    field_names.push_back("REFRESH");
    field_names.push_back("PROGRESS");
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 2.8) << std::setfill(' ') << "" << "START";
    field_names.push_back(ss.str());
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 2.8) << std::setfill(' ') << "" << "RESUME";
    field_names.push_back(ss.str());
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 2.8) << std::setfill(' ') << "" << "STOP";
    field_names.push_back(ss.str());

    field_names.push_back("TASK");
    field_names.push_back("REPORT");
    field_names.push_back("FORMAT");
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 2.5) << std::setfill(' ') << "" << "SHOW";
    field_names.push_back(ss.str());
    
    stringstream().swap(ss);
    ss << std::setw(button_width / 2.8) << std::setfill(' ') << "" << "EXPORT";
    field_names.push_back(ss.str());
}

void Ui::clear_items()
{
    for (size_t i = 1; i < (n_values + 1); i++)
        delwin(items[i]);
    delwin(items[0]);
    free(items);
}

void Ui::clear_form()
{
    for (int i = 0; i <= (field_count(form) - 1); i++)
        free_field(fields[i]);
    unpost_form(form);
    free_form(form);
}
