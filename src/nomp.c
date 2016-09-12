#include "nomp.h"

#define KEY_RETURN 10
#define KEY_ESCAPE 27
#define KEY_QUIT 113

FIELD *field[9];
FORM *form;
WINDOW *window; 

int i;
int n_fields = ((sizeof(field) / sizeof(field[0])) - 2);

void init()
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();

    if (has_colors()) {
        start_color();
        init_color(COLOR_GREEN, 0, 200, 0);
        init_color(COLOR_BLUE, 150, 150, 150);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_GREEN);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
    }

    ui();
}

void ui()
{ 
    int rows;
    int cols;
    
    field[0] = new_field(1, 30, 0, 1, 0, 0);
    field[1] = new_field(1, 30, 3, 1, 0, 0);
    field[2] = new_field(1, 30, 6, 1, 0, 0);
    field[3] = new_field(1, 30, 9, 1, 0, 0);
    field[4] = new_field(1, 30, 14, 1, 0, 0);
    field[5] = new_field(1, 30, 17, 1, 0, 0);
    field[6] = new_field(1, 30, 22, 1, 0, 0);
    field[7] = new_field(1, 30, 25, 1, 0, 0);
    field[8] = NULL;
    
    set_field_back(field[0], COLOR_PAIR(3));
    
    for (i = 0; i <= n_fields; i++) {
        set_field_just(field[i], JUSTIFY_CENTER);
        field_opts_off(field[i], O_AUTOSKIP);
        if (i >= 4) {
            field_opts_off(field[i], O_ACTIVE);
            set_field_back(field[i], COLOR_PAIR(4));
        }
    }
    
    set_field_buffer(field[0], 0, "localhost");
    set_field_buffer(field[1], 0, "9390");
    
    form = new_form(field);
    scale_form(form, &rows, &cols);

    window = create_window(rows + 6, cols + 4, 1, ((COLS - 36) / 2));
    keypad(window, TRUE);
    
    set_form_win(form, window);
    set_form_sub(form, derwin(window, rows, cols, 2, 2));

    post_form(form);
    
    wattron(window,  A_ITALIC | COLOR_PAIR(1));
    mvwprintw(window, 1, 3, "HOST");
    mvwprintw(window, 4, 3, "PORT");
    mvwprintw(window, 7, 3, "USER");
    mvwprintw(window, 10, 3, "PASSWORD");
    mvwprintw(window, 15, 3, "TARGET");
    mvwprintw(window, 18, 3, "SCAN");
    mvwprintw(window, 23, 3, "TASK");
    mvwprintw(window, 26, 3, "UPDATE");
    wattroff(window, A_ITALIC | COLOR_PAIR(1));

    wrefresh(stdscr);
    wrefresh(window);

    driver();
}

WINDOW *create_window(int height, int width, int starty, int startx)
{	
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    wbkgd(win, COLOR_PAIR(1));
    
    return win;
}

void cmd()
{
}

void driver()
{ 
    int key;
    int c_field = 0;
    int a_fields = 3;

    do {
        key = wgetch(window);
        switch(key)
		{	
            case KEY_DOWN:
                set_field_back(field[c_field], COLOR_PAIR(2));
                if (c_field == a_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(field[c_field], COLOR_PAIR(3));
                break;
            case KEY_UP:
                set_field_back(field[c_field], COLOR_PAIR(2));
                if (c_field == 0)
                    c_field = a_fields;
                else
                    --c_field;
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                set_field_back(field[c_field], COLOR_PAIR(3));
                break;
            case KEY_RETURN:
                a_fields = n_fields;
                for (i = 0; i <= a_fields; i++) {
                    set_field_back(field[i], COLOR_PAIR(2));
                    field_opts_on(field[i], O_ACTIVE);
                }
                //field_opts_off(field[i], O_AUTOSKIP | O_EDIT);
                break;
            default:
                form_driver(form, key);
                break;
        }
    } while (key != KEY_QUIT);
    
    quit();
}

void quit()
{
    unpost_form(form);
    
    free_form(form);
    
    for (i = 0; i <= n_fields; i++)
	    free_field(field[i]);
    
    delwin(window);
    endwin();
}
