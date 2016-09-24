#include <iostream>

#include "nomp.h"

Nomp::Nomp() :
    is_logged(false)
{
    driver();
}

Nomp::~Nomp()
{
}

void Nomp::driver()
{ 
    int key;
    int c_item;
    int c_field = 0;

    do {
        key = wgetch(ui.window);
        switch(key)
		{
            case KEY_LEFT:
                form_driver(*ui.p_form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(*ui.p_form, REQ_NEXT_CHAR);
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
            case KEY_RETURN:
                if (!is_logged) {
                    switch(c_field)
		            {	
                        case 4:
                            f.clear();
                            for (int i = 0; i < 4; i++)
                                f.push_back(i);
                            user_configs = ui.get_fields_value(&f);
                            //str1.compare(6,5,"apple") == 0)
                            if ((ret_cmd = exec.request(build_command("--ping"))).compare(0, 5, "ERROR") == 0) {
                                ui.error(ret_cmd);
                            } else {
                                is_logged = true;
                                c_field = 0;
                                ui.cleanup();
                                ui.main();
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
                            //TODO: create xml request.
                            break;
                        case 2:
                        case 4:
                        case 5:
                            paths_xml.clear();
                            if (c_field == 2) {
                                if ((ret_cmd = exec.request(build_command("-X '<get_port_lists/>'"))).compare(0, 5, "ERROR") == 0) {
                                    ui.error(ret_cmd);
                                    break;
                                } else {
                                    paths_xml.push_back(path_port_lists); 
                                    ret_xml = xml.parse(&ret_cmd, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 19);
                                }
                            } else if (c_field == 4) {
                                if ((ret_cmd = exec.request(build_command("-X '<get_targets/>'"))).compare(0, 5, "ERROR") == 0) {
                                    ui.error(ret_cmd);
                                    break;
                                } else {
                                    paths_xml.push_back(path_targets); 
                                    ret_xml = xml.parse(&ret_cmd, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 26);
                                }
                            } else if (c_field == 5) {
                                if ((ret_cmd = exec.request(build_command("-X '<get_configs/>'"))).compare(0, 5, "ERROR") == 0) {
                                    ui.error(ret_cmd);
                                    break;
                                } else {
                                    paths_xml.push_back(path_configs); 
                                    ret_xml = xml.parse(&ret_cmd, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 28);
                                }
                            }
                            c_item = ui.scroll_menu(ui.p_windows_menu);
                            ui.delete_menu(ui.p_windows_menu);
                            if (c_item >= 0) {
                                set_field_buffer(ui.p_fields[c_field], 0, ret_xml[c_item].c_str());
                                int n_id = ((ret_xml.size() / 2) + c_item);
                                if (c_field == 2)
                                    port_list_id = ret_xml[n_id];
                                else if (c_field == 4)
                                    target_id = ret_xml[n_id];
                                else
                                    config_id = ret_xml[n_id];
                            }
                            touchwin(stdscr);
                            touchwin(ui.window);
                            wrefresh(stdscr);
                            break;
                        default:
                            break;
                    }
                }
                break;
            default:
                form_driver(*ui.p_form, key);
                break;
        }
    } while (key != KEY_QUIT);
   
    //quit();
}

const string Nomp::build_command(const string arg) 
{
    const string ret = "omp -h " + user_configs[0] + " -p " + user_configs[1] +
                       " -u " + user_configs[2] + " -w " + user_configs[3] + " " + arg;
    
    return ret;
}






