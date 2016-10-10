#include <iostream>
#include <fstream>//
#include <chrono>//
#include <thread>//

#include "nomp.h"

Nomp::Nomp() :
    is_logged(false),
    is_task_running(false)
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
    int c_field = 2;

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
            case KEY_DOWN:
                switch(c_field)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        if (!is_logged) {
                            if (c_field == 3) {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                            } else if (c_field == 4) {
                                attron(COLOR_PAIR(1));
                                mvprintw(28, 77, " ");
                                mvprintw(28, 96, " ");
                                attroff(COLOR_PAIR(1));
                                curs_set(1);
                            } else {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                            }
                        } else {
                            if (c_field == 2) {
                                mvdelch(11, 66);
                                curs_set(1);
                            } else if (c_field == 3) {
                                attron(COLOR_PAIR(1));
                                mvprintw(13, 34, " ");
                                mvprintw(13, 53, " ");
                                attroff(COLOR_PAIR(1));
                                curs_set(1);
                            } else {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                            }
                        }
                        break;
                    case 5:
                        mvdelch(19, 66);
                        curs_set(1);
                        break;
                    case 6:
                        mvdelch(21, 66);
                        curs_set(1);
                        break;
                    case 7:
                        attron(COLOR_PAIR(1));
                        mvprintw(23, 34, " ");
                        mvprintw(23, 53, " ");
                        attroff(COLOR_PAIR(1));
                        curs_set(1);
                        break;
                    case 8:
                        mvdelch(27, 66);
                        curs_set(1);
                        break;
                    case 9:
                        mvdelch(29, 66);
                        curs_set(1);
                        break;
                    case 10:
                        if (is_task_running) {
                            attron(COLOR_PAIR(1));
                            mvprintw(33, 23, " ");
                            mvprintw(33, 42, " ");
                            attroff(COLOR_PAIR(1));
                        } else {
                            attron(COLOR_PAIR(1));
                            mvprintw(33, 23, " ");
                            mvprintw(33, 42, " ");
                            attroff(COLOR_PAIR(1));
                        }
                        curs_set(1);
                        break;
                    case 11:
                        if (is_task_running) {
                            attron(COLOR_PAIR(1));
                            mvprintw(33, 45, " ");
                            mvprintw(33, 64, " ");
                            attroff(COLOR_PAIR(1));
                        } else {
                            attron(COLOR_PAIR(1));
                            mvprintw(33, 45, " ");
                            mvprintw(33, 64, " ");
                            attroff(COLOR_PAIR(1));
                        }
                        curs_set(1);
                        break;
                    case 12:
                        mvdelch(37, 66);
                        curs_set(1);
                        break;
                    case 13:
                        mvdelch(39, 66);
                        curs_set(1);
                        break;
                    case 14:
                        if (is_task_running) {
                            attron(COLOR_PAIR(1));
                            mvprintw(41, 23, " ");
                            mvprintw(41, 42, " ");
                            attroff(COLOR_PAIR(1));
                        } else {
                            attron(COLOR_PAIR(1));
                            mvprintw(41, 23, " ");
                            mvprintw(41, 42, " ");
                            attroff(COLOR_PAIR(1));
                        }
                        curs_set(1);
                        break;
                    case 15:
                        if (is_task_running) {
                            attron(COLOR_PAIR(1));
                            mvprintw(41, 45, " ");
                            mvprintw(41, 64, " ");
                            attroff(COLOR_PAIR(1));
                        } else {
                            attron(COLOR_PAIR(1));
                            mvprintw(41, 45, " ");
                            mvprintw(41, 64, " ");
                            attroff(COLOR_PAIR(1));
                        }
                        curs_set(1);
                        break;
                    default:
                        break;
                }
                
                if (key == KEY_UP) {
                    if (c_field == 0)
                        c_field = ui.n_fields;
                    else
                        --c_field;
                    form_driver(*ui.p_form, REQ_PREV_FIELD);
                    form_driver(*ui.p_form, REQ_END_LINE);
                } else {
                    if (c_field == ui.n_fields)
                        c_field = 0;
                    else
                        ++c_field;
                    form_driver(*ui.p_form, REQ_NEXT_FIELD);
                    form_driver(*ui.p_form, REQ_END_LINE);
                }
                
                switch(c_field)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        if (!is_logged) {
                            if (c_field == 3) {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                            } else if (c_field == 4) {
                                mvaddch(28, 77, ACS_CKBOARD);
                                mvaddch(28, 96, ACS_CKBOARD);
                                curs_set(0);
                            } else {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                            }
                        } else {
                            if (c_field == 2) {
                                mvaddch(11, 66, ACS_DIAMOND);
                                curs_set(0);
                            } else if (c_field == 3) {
                                mvaddch(13, 34, ACS_CKBOARD);
                                mvaddch(13, 53, ACS_CKBOARD);
                                curs_set(0);
                            } else {
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                            }
                        }
                        break;
                    case 5:
                        mvaddch(19, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 6:
                        mvaddch(21, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 7:
                        mvaddch(23, 34, ACS_CKBOARD);
                        mvaddch(23, 53, ACS_CKBOARD);
                        curs_set(0);
                        break;
                    case 8:
                        mvaddch(27, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 9:
                        mvaddch(29, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 10:
                        if (is_task_running) {
                            mvaddch(33, 23, ACS_CKBOARD);
                            mvaddch(33, 42, ACS_CKBOARD);
                        } else {
                            mvaddch(33, 23, ACS_CKBOARD);
                            mvaddch(33, 42, ACS_CKBOARD);
                        }
                        curs_set(0);
                        break;
                    case 11:
                        if (is_task_running) {
                            mvaddch(33, 45, ACS_CKBOARD);
                            mvaddch(33, 64, ACS_CKBOARD);
                        } else {
                            mvaddch(33, 45, ACS_CKBOARD);
                            mvaddch(33, 64, ACS_CKBOARD);
                        }
                        curs_set(0);
                        break;
                    case 12:
                        mvaddch(37, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 13:
                        mvaddch(39, 66, ACS_DIAMOND);
                        curs_set(0);
                        break;
                    case 14:
                        if (is_task_running) {
                            mvaddch(41, 23, ACS_CKBOARD);
                            mvaddch(41, 42, ACS_CKBOARD);
                        } else {
                            mvaddch(41, 23, ACS_CKBOARD);
                            mvaddch(41, 42, ACS_CKBOARD);
                        }
                        curs_set(0);
                        break;
                    case 15:
                        if (is_task_running) {
                            mvaddch(41, 45, ACS_CKBOARD);
                            mvaddch(41, 64, ACS_CKBOARD);
                        } else {
                            mvaddch(41, 45, ACS_CKBOARD);
                            mvaddch(41, 64, ACS_CKBOARD);
                        }
                        curs_set(0);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_DELCHAR:
                // TODO: Que no salte al field anterior al borrar todo el texto.
                form_driver(*ui.p_form, REQ_DEL_PREV);
                break;
            case KEY_RETURN:
                if (!is_logged) {
                    if (c_field == 4) {
                        i_fields.clear();
                        for (int i = 0; i < 4; i++)
                            i_fields.push_back(i);
                        user_configs = ui.get_fields_value(&i_fields, 0);
                        cmd = command.create(&user_configs, "<get_version/>");
                        if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                            ui.error(cret);
                        } else {
                            is_logged = true;
                            c_field = 0;
                            ui.cleanup();
                            ui.main();
                        }
                    }
                    break;
                } else {
                    i_fields.clear();
                    xnodes.clear();
                    xvalues.clear();
                    switch(c_field)
		            {
                        case 3:
                            xnodes.push_back("create_target");
                            xnodes.push_back("name");
                            xnodes.push_back("hosts");
                            xnodes.push_back("port_list");
                            for (int i = 0; i < 2; i++)
                                i_fields.push_back(i);
                            xvalues = ui.get_fields_value(&i_fields, 0);
                            xvalues.push_back(port_list_id + "__attr__");
                            xret = xml.create(&xnodes, &xvalues);
                            cmd = command.create(&user_configs, xret[0]);
                            if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                                ui.error(cret);
                            } else {
                                break;//
                            }
                            break;
                        case 7:
                            xnodes.push_back("create_task");
                            xnodes.push_back("name");
                            xnodes.push_back("config");
                            xnodes.push_back("target");
                            i_fields.push_back(4);
                            xvalues = ui.get_fields_value(&i_fields, 4);
                            xvalues.push_back(config_id + "__attr__");
                            xvalues.push_back(target_id + "__attr__");
                            xret = xml.create(&xnodes, &xvalues);
                            cmd = command.create(&user_configs, xret[0]);
                            if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                                ui.error(cret);
                            } else {
                                break;//
                            }
                            break;
                        case 10:
                            xnodes.push_back("start_task");
                            xnodes.push_back("task_id");
                            xvalues.push_back(task_id + "__attr__");
                            xret = xml.create(&xnodes, &xvalues);
                            cmd = command.create(&user_configs, xret[0]);
                            if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                                ui.error(cret);
                            } else {
                                is_task_running = true;
                                refresh();
                                //set_field_buffer(ui.p_fields[11], 0, "       PAUSE");
                            }
                            break;
                        case 11:
                            if (is_task_running) {
                                xnodes.push_back("stop_task");
                                xnodes.push_back("task_id");
                                xvalues.push_back(task_id + "__attr__");
                                xret = xml.create(&xnodes, &xvalues);
                                cmd = command.create(&user_configs, xret[0]);
                                if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                                    ui.error(cret);
                                } else {
                                    is_task_running = false;
                                }
                            }
                            break;
                            if (is_task_running) {
                                xnodes.push_back("stop_task");
                                xnodes.push_back("task_id");
                                xvalues.push_back(task_id + "__attr__");
                                xret = xml.create(&xnodes, &xvalues);
                                cmd = command.create(&user_configs, xret[0]);
                                if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
                                    ui.error(cret);
                                } else {
                                    is_task_running = false;
                                }
                            }
                            break;
                        case 2:
                        case 5:
                        case 6:
                        case 8:
                        case 9:
                        case 12:
                        case 13:
                        case 14:
                            xpaths.clear();
                            switch(c_field)
                            {
                                case 2:
                                    xpaths.push_back("/get_port_lists_response/port_list"); 
                                    xpaths.push_back("/get_port_lists_response/port_list/name");
                                    get("<get_port_lists/>", 10);
                                    break;
                                case 5:
                                    xpaths.push_back("/get_configs_response/config"); 
                                    xpaths.push_back("/get_configs_response/config/name");
                                    get("<get_configs/>", 18);
                                    break;
                                case 6:
                                    xpaths.push_back("/get_targets_response/target"); 
                                    xpaths.push_back("/get_targets_response/target/name");
                                    get("<get_targets/>", 20);
                                    break;
                                case 8:
                                    xpaths.push_back("/get_tasks_response/task"); 
                                    xpaths.push_back("/get_tasks_response/task/name");
                                    get("<get_tasks/>", 26);
                                    break;
                                case 9:
                                    xret = times;
                                    ui.menu(&xret, 28);
                                    break;
                                case 12:
                                    xpaths.push_back("/get_tasks_response/task/second_last_report/report"); // TODO: CAMBIAR 
                                    xpaths.push_back("/get_tasks_response/task/name");
                                    get("<get_tasks/>", 36);
                                    break;
                                case 13:
                                    xpaths.push_back("/get_report_formats_response/report_format"); 
                                    xpaths.push_back("/get_report_formats_response/report_format/name");
                                    get("<get_report_formats/>", 32);
                                    cmd = command.create(&user_configs, "<get_report_formats/>");
                                    break;
                                case 14:
                                    xnodes.push_back("get_reports");
                                    xnodes.push_back("report_id"); 
                                    xnodes.push_back("filter");
                                    xvalues.push_back(report_id + "__attr__");
                                    xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                       notes=0 overrides=0 rows=10000 delta_states=gn \
                                                       result_hosts_only=1 ignore_pagination=1__attr__");
                                    xret = xml.create(&xnodes, &xvalues);//

                                    xpaths.push_back("/get_reports_response/report/report/results/result"); 
                                    xpaths.push_back("/get_reports_response/report/report/results/result/name");
                                    xpaths.push_back("/get_reports_response/report/report/results/result/port");
                                    xpaths.push_back("/get_reports_response/report/report/results/result/severity");
                                    get(xret[0], 0, true, "id", true);
                                    break;
                                default:
                                    break;
                            }
                           
                            if (c_field == 14) {
                                c_item = ui.report_scroll(&xret);
                                ui.delete_arr_report();
                            } else {
                                c_item = ui.menu_scroll(&xret);
                                ui.delete_windows_arr(2);
                            }
                            
                            if (c_item >= 0) {
                                if (c_field == 2)
                                    port_list_id = xret[c_item];
                                else if (c_field == 5)
                                    config_id = xret[c_item];
                                else if (c_field == 6)
                                    target_id = xret[c_item];
                                else if (c_field == 8)
                                    task_id = xret[c_item];
                                else if (c_field == 9)
                                    refresh_id = xret[c_item];
                                else if (c_field == 12)
                                    report_id = xret[c_item];
                                else if (c_field == 13)
                                    report_format_id = xret[c_item];

                                set_field_buffer(ui.p_fields[c_field], 0, xret[(xret.size() / 3) + c_item].c_str());
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

void Nomp::get(string cmd_opt, int rows, bool get_data, string attr_name, bool is_report)
{
    cmd = command.create(&user_configs, cmd_opt);
    if ((cret = command.execute(cmd)).find("__ERROR__") != string::npos) {
        ui.error(cret);
    } else {
        xret = xml.parse(&cret, &xpaths, attr_name, get_data, is_report);
        if (rows != -1) {
            if (is_report)
                ui.report(&xret);
            else
                ui.menu(&xret, rows);
        }
    }
}

void Nomp::refresh()
{
    xnodes.clear();
    xvalues.clear();
    xpaths.clear();
    
    xnodes.push_back("get_tasks");
    xnodes.push_back("task_id");
    xvalues.push_back(task_id + "__attr__");
    
    xret = xml.create(&xnodes, &xvalues);

    xpaths.push_back("/get_tasks_response/task/progress"); 
    get(xret[0], -1, false);
    
    ui.progress(xret[0].erase(xret[0].size() - 1));
    
    if (is_task_running) {
        std::thread t(&Nomp::refresh_sleep, this);
        t.detach();
    }
}

void Nomp::refresh_sleep()
{
    std::this_thread::sleep_for(std::chrono::seconds(stoi(refresh_id)));
    refresh();
}
