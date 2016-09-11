#include "nomp.h"

#define ENTER 10
#define ESCAPE 27
#define QUIT 113

void init()
{
    initscr();

    noecho();
    curs_set(0);
    cbreak();
    //keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE); //TODO: Si acepta colores, modificar el par 0.
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
    }

    login();
}

void login()
{ 
    int rows;
    int cols;
    
    field_login[0] = new_field(1, 30, 0, 1, 0, 0);
    field_login[1] = new_field(1, 30, 3, 1, 0, 0);
    field_login[2] = new_field(1, 30, 6, 1, 0, 0);
    field_login[3] = new_field(1, 30, 9, 1, 0, 0);
    field_login[4] = NULL;
    
    set_field_buffer(field_login[0], 0, "localhost");
    set_field_buffer(field_login[1], 0, "9390");
    
    field_opts_off(field_login[0], O_AUTOSKIP);
    field_opts_off(field_login[1], O_AUTOSKIP);
    field_opts_off(field_login[2], O_AUTOSKIP);
    field_opts_off(field_login[3], O_AUTOSKIP | O_PUBLIC);
	
    form_login = new_form(field_login);
    scale_form(form_login, &rows, &cols);
    
    win_login = create_window(rows + 6, cols + 4, ((LINES - 14) / 2), ((COLS - 36) / 2));
    //box(win_login, 0, 2);
    keypad(win_login, TRUE);
    
    set_form_win(form_login, win_login);
    set_form_sub(form_login, derwin(win_login, rows, cols, 2, 2));

    post_form(form_login);
    
    wattron(win_login, COLOR_PAIR(1));
    mvwprintw(win_login, 1, 3, "HOST");
    mvwprintw(win_login, 4, 3, "PORT");
    mvwprintw(win_login, 7, 3, "USER");
    mvwprintw(win_login, 10, 3, "PASSWORD");
    wattroff(win_login, COLOR_PAIR(1));

    wrefresh(stdscr);
    wrefresh(win_login);

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
   //system("omp -h 127.0.0.1 -p 9390 -u user -w ak474747**OPENVAS -g > temp");
}

void driver()
{ 
    int key;

    do {
        key = wgetch(win_login);
        switch(key)
		{	
            case KEY_DOWN:
				form_driver(form_login, REQ_NEXT_FIELD);
				form_driver(form_login, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(form_login, REQ_PREV_FIELD);
				form_driver(form_login, REQ_END_LINE);
				break;
			default:
				form_driver(form_login, key);
				break;
		}
    } while (key != QUIT);
    
    cleanup();
}

void cleanup()
{
    unpost_form(form_login);
    
    free_form(form_login);
	free_field(field_login[0]);
	free_field(field_login[1]); 
	free_field(field_login[2]); 
	free_field(field_login[3]); 
    
    delwin(win_login);
    endwin();
}

//WINDOW *win_login = create_window(12, 110, 0, ((COLS - 110) / 2));
//WINDOW *win_target = create_window(12, 110, ((LINES / 2) - 11), ((COLS - 110) / 2));
//WINDOW *win_task = create_window(12, 110, ((LINES / 2) + 2), ((COLS - 110) / 2));
//WINDOW *win_launch = create_window(12, 110, (LINES - 10), ((COLS - 110) / 2));
