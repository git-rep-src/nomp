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
        key = getch();
        switch(key)
		{
            case KEY_LEFT:
                form_driver(*ui.p_form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(*ui.p_form, REQ_NEXT_CHAR);
                break;
            case KEY_UP:
                if (!is_logged && (c_field == 3))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(6));
                else if (!is_logged && ((c_field == 4) || (c_field == 5)))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                else if (is_logged && ((c_field == 3) || (c_field == 7) || (c_field == 10)))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                else    
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                if (c_field == 0)
                    c_field = ui.n_fields;
                else
                    --c_field;
                form_driver(*ui.p_form, REQ_PREV_FIELD);
                form_driver(*ui.p_form, REQ_END_LINE);
                if (!is_logged) {
                    if (c_field == 3)
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(7));
                    else if ((c_field == 4) || (c_field == 5))
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(4));
                    else    
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                } else {
                    if ((c_field == 3) || (c_field == 7) || (c_field == 10))
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(4));
                    else    
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                }
                break;
            case KEY_DOWN:
                if (!is_logged && (c_field == 3))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(6));
                else if (!is_logged && ((c_field == 4) || (c_field == 5)))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                else if (is_logged && ((c_field == 3) || (c_field == 7) || (c_field == 10)))
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                else    
                    set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                if (c_field == ui.n_fields)
                    c_field = 0;
                else
                    ++c_field;
                form_driver(*ui.p_form, REQ_NEXT_FIELD);
                form_driver(*ui.p_form, REQ_END_LINE);
                if (!is_logged) {
                    if (c_field == 3)
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(7));
                    else if ((c_field == 4) || (c_field == 5))
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(4));
                    else    
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                } else {
                    if ((c_field == 3) || (c_field == 7) || (c_field == 10))
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(4));
                    else    
                        set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                }
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
                            user_configs = ui.get_fields_value(&f, 0);
                            cmd = command.create(&user_configs, "<get_version/>");
                            if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                ui.error(ret_exec);
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
                            f.clear();
                            nodes_xml.clear();
                            nodes_xml.push_back("create_target");
                            nodes_xml.push_back("name");
                            nodes_xml.push_back("hosts");
                            nodes_xml.push_back("port_list");
                            for (int i = 0; i < 2; i++)
                                f.push_back(i);
                            values_xml = ui.get_fields_value(&f, 0);
                            values_xml.push_back(port_list_id + "_attr");
                            ret_xml = xml.create(&nodes_xml, &values_xml);
                            cmd = command.create(&user_configs, ret_xml[0]);
                            if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                ui.error(ret_exec);
                            } else {
                                break;//
                            }
                            break;
                        case 7:
                            f.clear();
                            nodes_xml.clear();
                            nodes_xml.push_back("create_task");
                            nodes_xml.push_back("name");
                            nodes_xml.push_back("config");
                            nodes_xml.push_back("target");
                            f.push_back(4);
                            values_xml = ui.get_fields_value(&f, 4);
                            values_xml.push_back(config_id + "_attr");
                            values_xml.push_back(target_id + "_attr");
                            ret_xml = xml.create(&nodes_xml, &values_xml);
                            cmd = command.create(&user_configs, ret_xml[0]);
                            if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                ui.error(ret_exec);
                            } else {
                                break;//
                            }
                            break;
                        case 10:
                            f.clear();
                            nodes_xml.clear();
                            nodes_xml.push_back("create_task");
                            nodes_xml.push_back("name");
                            nodes_xml.push_back("config");
                            nodes_xml.push_back("target");
                            f.push_back(4);
                            values_xml = ui.get_fields_value(&f, 4);
                            values_xml.push_back(config_id + "_attr");
                            values_xml.push_back(target_id + "_attr");
                            ret_xml = xml.create(&nodes_xml, &values_xml);
                            cmd = command.create(&user_configs, ret_xml[0]);
                            if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                ui.error(ret_exec);
                            } else {
                                break;//
                            }
                            break;
                        case 2:
                        case 5:
                        case 6:
                            paths_xml.clear();
                            if (c_field == 2) {
                                cmd = command.create(&user_configs, "<get_port_lists/>");
                                if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                    ui.error(ret_exec);
                                    break;
                                } else {
                                    paths_xml.push_back(path_port_lists + "/name"); 
                                    paths_xml.push_back(path_port_lists); 
                                    ret_xml = xml.parse(&ret_exec, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 16);
                                }
                            } else if (c_field == 5) {
                                cmd = command.create(&user_configs, "<get_configs/>");
                                if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                    ui.error(ret_exec);
                                    break;
                                } else {
                                    paths_xml.push_back(path_configs + "/name"); 
                                    paths_xml.push_back(path_configs); 
                                    ret_xml = xml.parse(&ret_exec, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 24);
                                }
                            } else if (c_field == 6) {
                                cmd = command.create(&user_configs, "<get_targets/>");
                                if ((ret_exec = command.execute(cmd)).find("ERROR") != string::npos) {
                                    ui.error(ret_exec);
                                    break;
                                } else {
                                    paths_xml.push_back(path_targets + "/name"); 
                                    paths_xml.push_back(path_targets); 
                                    ret_xml = xml.parse(&ret_exec, &paths_xml);
                                    ui.p_windows_menu = ui.create_menu(&ret_xml, 26);
                                }
                            }
                            c_item = ui.scroll_menu(ui.p_windows_menu);
                            ui.delete_menu(ui.p_windows_menu);
                            if (c_item >= 0) {
                                set_field_buffer(ui.p_fields[c_field], 0, ret_xml[c_item].c_str());
                                int n_id = ((ret_xml.size() / 2) + c_item);
                                if (c_field == 2)
                                    port_list_id = ret_xml[n_id];
                                else if (c_field == 5)
                                    config_id = ret_xml[n_id];
                                else if (c_field == 6)
                                    target_id = ret_xml[n_id];
                            }
                            touchwin(stdscr);
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
