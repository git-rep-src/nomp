#include "ui.h"

#include <sstream>
#include <iomanip>

using std::stringstream;

Ui::Ui() :
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
    
    field_width = (COLS / 4.1);
    button_width = (COLS / 8.6);

    // TODO: REVISAR LINES/COLS EN XML.
    // CALCULAR MINIMO PARA LINES Y COLS.
    
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

Ui::~Ui()
{
    clear_form();
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
    post_form(form);
    
    mvprintw(start_y,       (start_x - 5), field_names[0].c_str());
    mvprintw((start_y + 2), (start_x - 5), field_names[1].c_str());
    mvprintw((start_y + 4), (start_x - 9), field_names[2].c_str());
    mvprintw((start_y + 6), (start_x - 9), field_names[3].c_str());
    
    refresh();

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
    post_form(form);

    mvprintw(start_y,        (start_x - 5), field_names[5].c_str());
    mvprintw((start_y + 2),  (start_x - 8), field_names[6].c_str());
    mvprintw((start_y + 4),  (start_x - 6), field_names[7].c_str());
    mvprintw((start_y + 6),  (start_x - 6), field_names[8].c_str());
    mvprintw((start_y + 12), (start_x - 5), field_names[10].c_str());
    mvprintw((start_y + 14), (start_x - 8), field_names[11].c_str());
    mvprintw((start_y + 16), (start_x - 5), field_names[12].c_str());
    mvprintw((start_y + 18), (start_x - 7), field_names[13].c_str());
    mvprintw((start_y + 24), (start_x - 5), field_names[15].c_str());
    mvprintw((start_y + 26), (start_x - 8), field_names[16].c_str());
    mvprintw((start_y + 28), (start_x - 9), field_names[17].c_str());
    mvprintw((start_y + 34), (start_x - 5), field_names[21].c_str());
    mvprintw((start_y + 36), (start_x - 7), field_names[22].c_str());
    mvprintw((start_y + 38), (start_x - 7), field_names[23].c_str());
    
    curs_set(1);
    
    refresh();

    form_driver(form, REQ_END_LINE);
}

int Ui::menu(const vector<string> *values, size_t n)
{
    int key;
    int row;
    int c_item = 0;

    n_values = ((values->size()) / n);
    
    field_info(current_field(form), NULL, NULL, &row, NULL, NULL, NULL);
    if (field_index(current_field(form)) == 16)
        row -= 11;
    else
        --row;
    
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
    WINDOW *w = newwin((start_y + 37), (COLS / 2.40), start_y, (COLS / 1.77));

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
    
    items = (WINDOW **) malloc ((n_values + 1) * sizeof(WINDOW *));
    items[0] = newpad((n_values + (start_y + 40)), COLS);
    keypad(items[0], true);
    
    for (size_t i = 1; i <= n_values; i++) {
        items[i] = subpad(items[0], 1, (COLS - 5), (i - 1), 0);
        mvwprintw(items[i], 0, 0, "%s", (*values)[n_values + i].c_str());
        mvwprintw(items[i], 0, (COLS / 1.39), "%s", (*values)[(n_values * 2) + i].c_str());
        mvwprintw(items[i], 0, (COLS / 1.23), "%s", (*values)[(n_values * 3) + i].c_str());
        mvwprintw(items[i], 0, (COLS / 1.09), "%s", (*values)[(n_values * 4) + i].c_str());
    }
    
    wbkgd(items[1], COLOR_PAIR(2));
    wbkgd(items[1], A_REVERSE);
    
    prefresh(items[0], 0, 0, start_y, (COLS / 34.8), (start_y + 40), (COLS - 5));

    do {
        prefresh(items[0], c_item, 0, start_y, (COLS / 34.8), (start_y + 40), (COLS - 5));
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
    
    return -1;
}

void Ui::report_details(const vector<string> **values, unsigned int c_item, size_t n)
{
    int key;
    int c_line = 0;
    int details_lines = 2;
    int height = (start_y + 40);

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
        prefresh(w, c_line, 0, start_y, (COLS / 34.8), (start_y + 40), (COLS - 5));
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
                if (((start_y + 40) + c_line) >= height)
                    continue;
                c_line++;
                break;
            default:
                break;
        }
    } while (key != KEY_ESCAPE);
    
    delwin(w);
}

void Ui::progress(const string &p)
{  
    for (int i = 0; i <= field_width; i++)
        mvdelch((start_y + 28), start_x);
    
    if (p == "-1") {
        status("TASK FINISHED");
    } else if (p == "-2") {    
        status("TASK STOPPED");
    } else {
        if ((stoi(p) > 0) && (stoi(p) < 3))
            mvhline((start_y + 28), start_x, ACS_VLINE, 1);
        else
            mvhline((start_y + 28), start_x, ACS_VLINE, (stoi(p) / 2.38));
    }
    
    refresh();
}

void Ui::status(const string &sts)
{
    int sts_x = 0;
    int max_y;
    int max_x;
    
    WINDOW *w = newwin(1, COLS, (LINES - 1), 0);
    
    getmaxyx(w, max_y, max_x);
    sts_x = ((max_x - sts.size()) / 2) + 1; 
    max_y = 0;
    mvwprintw(w, max_y, sts_x, sts.c_str());
    
    wrefresh(w);
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
            mvprintw(row, ((start_x + field_width) + 1), "+");
        } else {
            mvaddch(row, col, ACS_CKBOARD);
            mvaddch(row, (col + (cols - 1)), ACS_CKBOARD);
        }
        curs_set(0);
    } else {
        if (is_menu) {
            mvprintw(row, ((start_x + field_width) + 1), " ");
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(row, col, " ");
            mvprintw(row, (col + (cols - 1)), " ");
            attroff(COLOR_PAIR(1));
        }
        curs_set(1);
    }
}

int Ui::save()
{
    int key;
    int curs_state = curs_set(0);
    
    WINDOW *w = newwin((LINES / 4.45), (COLS / 4.26), (LINES / 2.88), (COLS / 2.65));
    wbkgd(w, COLOR_PAIR(1));
    box(w, 0, 0);
    keypad(w, TRUE);

    mvwprintw(w, (LINES / 9.8), (COLS / 58.33), "SAVE THE RUNNING TASK CONFIG? (Y/N)");
    
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

    touchwin(stdscr);

    return -1;
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
