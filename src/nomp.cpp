#include <iostream>

#include "nomp.h"

Nomp::Nomp()
{
    driver();
}

Nomp::~Nomp()
{
}

void Nomp::driver()
{ 
    //const int KEY_RETURN = 10;
    const int KEY_DELCHAR = 263;
    //const int KEY_ESCAPE = 27;
    const int KEY_QUIT = 113;

    int key;
    //int c_item;
    int c_field = 0;
    
    //WINDOW **p_windows_menu;

    do {
        key = wgetch(ui.window);
        switch(key)
		{
            case KEY_LEFT:
                form_driver(*ui.p_form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                ret_cmd = exec.request("omp -h 127.0.0.1 -u user -w ak474747**OPENVAS -X '<get_targets/>'");
                paths.push_back(path_targets); 
                ret_xml = xml.parse(&ret_cmd, &paths);
                cout << ret_xml[0] << endl;
                
                /* LOGIN
                ret = exec.request("omp -h 127.0.0.1 -u user -w ak474747**OPENVAS -X '<get_version/>'");
                if (ret == "OMP ping was successful.\n")
                     LOGGED
                */

                //form_driver(*ui.p_form, REQ_NEXT_CHAR);
                break;
            case KEY_UP:
                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                if (c_field == 0)
                    c_field = ui.n_fields;
                else
                    --c_field;
                form_driver(*ui.p_form, REQ_PREV_FIELD);
                form_driver(*ui.p_form, REQ_END_LINE);
                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                break;
            case KEY_DOWN:
                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                if (c_field == ui.n_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(*ui.p_form, REQ_NEXT_FIELD);
                form_driver(*ui.p_form, REQ_END_LINE);
                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                break;
            case KEY_DELCHAR:
                // TODO: Que no salte al field anterior al borrar todo el texto.
                form_driver(*ui.p_form, REQ_DEL_PREV);
                break;
            /*
            case KEY_RETURN:
                if (!is_logged) {
                    switch(c_field)
		            {	
                        case 4:
                            if (get_login()) {
                                c_field = 0;
                                is_logged = TRUE;
                                ui();
                            }
                            break;
                        case 5:
                            key = KEY_QUIT;
                            break;
                        default:
                            break;
                    }
                } else {
                    switch(c_field)
		            {
                        case 3:
                            //if(!create_xml(0))
                                //printf("%s", "ERROR");
                            break;
                        case 2:
                        case 4:
                        case 5:
                            if (c_field == 2) {
                                p_windows_menu = create_menu(NULL, 19);
                            } else if (c_field == 4) {
                                run("omp", "--xml='<get_targets/>'"); // FIX: AL ABRIR DOS VECES SE ROMPE.
                                xml_parse_node();
                                p_windows_menu = create_menu(&targets, 26);
                            } else if (c_field == 5) {
                                get_scans();
                                p_windows_menu = create_menu(&scans, 28);
                            }
                            c_item = scroll_menu(p_windows_menu);
                            delete_menu(p_windows_menu);
                            if (c_item >= 0) {
                                if (c_field == 2)
                                    set_field_buffer(fields[2], 0, ports[c_item]);
                                else if (c_field == 4)
                                    set_field_buffer(fields[4], 0, targets[c_item]);
                                else if (c_field == 5)
                                    set_field_buffer(fields[5], 0, scans[c_item]);
                            }
                            touchwin(stdscr);
                            touchwin(window);
                            wrefresh(stdscr);
                            break;
                        default:
                            break;
                    }
                }
                break;
                */
            default:
                form_driver(*ui.p_form, key);
                break;
        }
    } while (key != KEY_QUIT);
   
    //quit();
}
