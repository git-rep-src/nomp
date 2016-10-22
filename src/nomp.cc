#include <thread>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <pwd.h>
//#include <iostream>//

#include "nomp.h"
#include "xml.h"
#include "base64.h"

Nomp::Nomp() :
    c_field(2),
    is_login(true),
    is_task_running(false),
    is_auto_refresh_blocked(false),
    ids(7)
{
    driver();
}

Nomp::~Nomp()
{
}

void Nomp::driver()
{ 
    int key;

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
                        if (is_login)
                            if (c_field == 3)
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                        else
                            if (c_field == 2)
                                ui.marker(true, false);
                            else if (c_field == 3)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(1));
                        break;
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                    case 12:
                    case 13:
                        ui.marker(true, false);
                        break;
                    case 7:
                    case 10:
                    case 11:
                    case 14:
                    case 15:
                        ui.marker(false, false);
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
                        if (is_login)
                            if (c_field == 3)
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false);
                            else
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                        else
                            if (c_field == 2)
                                ui.marker();
                            else if (c_field == 3)
                                ui.marker(false);
                            else
                                set_field_back(ui.p_fields[c_field], COLOR_PAIR(2));
                        break;
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                    case 12:
                    case 13:
                        ui.marker();
                        break;
                    case 7:
                    case 10:
                    case 11:
                    case 14:
                    case 15:
                        ui.marker(false);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_BACKSPACE:
                form_driver(*ui.p_form, REQ_LEFT_CHAR);
                form_driver(*ui.p_form, REQ_DEL_CHAR);
                break;
            case KEY_RETURN:
                if (is_login) {
                    if (c_field == 4) {
                        std::vector<std::string>().swap(user_configs);
                        for (int i = 0; i <= 3; i++)
                            validators.insert(std::make_pair(std::make_pair(i, true), std::make_pair(true, -1)));
                        if (validate(user_configs)) {
                            if (omp("<get_version/>")) {
                                is_login = false;
                                c_field = 0;
                                ui.cleanup();
                                ui.main();
                            } else {
                                ui.status(std::make_pair("LOGIN ERROR", 4), is_login);
                            }
                        }
                    }
                } else {
                    switch(c_field)
		            {
                        case 3:
                            xnodes.push_back("create_target");
                            xnodes.push_back("name");
                            xnodes.push_back("hosts");
                            xnodes.push_back("port_list");
                            validators.insert(std::make_pair(std::make_pair(0, true), std::make_pair(true, -1)));
                            validators.insert(std::make_pair(std::make_pair(1, true), std::make_pair(true, -1)));
                            validators.insert(std::make_pair(std::make_pair(2, true), std::make_pair(false, 0)));
                            create();
                            break;
                        case 7:
                            xnodes.push_back("create_task");
                            xnodes.push_back("name");
                            xnodes.push_back("config");
                            xnodes.push_back("target");
                            validators.insert(std::make_pair(std::make_pair(4, true), std::make_pair(true, -1)));
                            validators.insert(std::make_pair(std::make_pair(5, true), std::make_pair(false, 1)));
                            validators.insert(std::make_pair(std::make_pair(6, true), std::make_pair(false, 2)));
                            create();
                            break;
                        case 10:
                            if (!is_task_running) {
                                xnodes.push_back("start_task");
                                xnodes.push_back("task_id");
                                validators.insert(std::make_pair(std::make_pair(8, true), std::make_pair(false, 3)));
                                validators.insert(std::make_pair(std::make_pair(9, true), std::make_pair(false, 4)));
                                if (create()) {
                                    is_task_running = true;
                                    auto_refresh();
                                }
                            }
                            break;
                        case 11:
                            if (is_task_running) {
                                xnodes.push_back("stop_task");
                                xnodes.push_back("task_id");
                                validators.insert(std::make_pair(std::make_pair(8, true), std::make_pair(false, 3)));
                                if (create()) {
                                    is_task_running = false;
                                    is_auto_refresh_blocked = false;
                                    ui.progress("-2");
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
                        case 15:
                            switch(c_field)
                            {
                                case 2:
                                    xpaths.push_back("/get_port_lists_response/port_list"); 
                                    xpaths.push_back("/get_port_lists_response/port_list/name");
                                    xpaths.push_back("/get_port_lists_response/port_list/name");
                                    xpaths.push_back("/get_port_lists_response/port_list/comment");
                                    xpaths.push_back("/get_port_lists_response/port_list/creation_time");
                                    xpaths.push_back("/get_port_lists_response/port_list/modification_time");
                                    xpaths.push_back("/get_port_lists_response/port_list/port_count/all"); 
                                    xpaths.push_back("/get_port_lists_response/port_list/port_count/tcp"); 
                                    xpaths.push_back("/get_port_lists_response/port_list/port_count/udp"); 
                                    get("<get_port_lists/>");
                                    break;
                                case 5:
                                    xpaths.push_back("/get_configs_response/config"); 
                                    xpaths.push_back("/get_configs_response/config/name");
                                    xpaths.push_back("/get_configs_response/config/name");
                                    xpaths.push_back("/get_configs_response/config/comment");
                                    xpaths.push_back("/get_configs_response/config/creation_time");
                                    xpaths.push_back("/get_configs_response/config/modification_time");
                                    xpaths.push_back("/get_configs_response/config/family_count");
                                    xpaths.push_back("/get_configs_response/config/nvt_count");
                                    get("<get_configs/>");
                                    break;
                                case 6:
                                    xpaths.push_back("/get_targets_response/target"); 
                                    xpaths.push_back("/get_targets_response/target/name");
                                    xpaths.push_back("/get_targets_response/target/name");
                                    xpaths.push_back("/get_targets_response/target/comment");
                                    xpaths.push_back("/get_targets_response/target/creation_time");
                                    xpaths.push_back("/get_targets_response/target/modification_time");
                                    xpaths.push_back("/get_targets_response/target/hosts");
                                    xpaths.push_back("/get_targets_response/target/port_list/name");
                                    get("<get_targets/>");
                                    break;
                                case 8:
                                    if (!is_task_running) {
                                        xpaths.push_back("/get_tasks_response/task"); 
                                        xpaths.push_back("/get_tasks_response/task/name");
                                        xpaths.push_back("/get_tasks_response/task/name");
                                        xpaths.push_back("/get_tasks_response/task/comment");
                                        xpaths.push_back("/get_tasks_response/task/creation_time");
                                        xpaths.push_back("/get_tasks_response/task/modification_time");
                                        xpaths.push_back("/get_tasks_response/task/scanner/name");
                                        xpaths.push_back("/get_tasks_response/task/config/name");
                                        xpaths.push_back("/get_tasks_response/task/target/name");
                                        xpaths.push_back("/get_tasks_response/task/status");
                                        get("<get_tasks/>");
                                    }
                                    break;
                                case 9:
                                    if (!is_task_running) {
                                        xpaths.push_back("");
                                        xpaths.push_back("");
                                        xret = auto_refresh_times;
                                        fill(false);
                                    }
                                    break;
                                case 12:
                                    xpaths.push_back("/get_tasks_response/task/first_report/report"); // TODO: CAMBIAR 
                                    xpaths.push_back("/get_tasks_response/task/name");
                                    get("<get_tasks/>");
                                    break;
                                case 13:
                                    xpaths.push_back("/get_report_formats_response/report_format"); 
                                    xpaths.push_back("/get_report_formats_response/report_format/name");
                                    xpaths.push_back("/get_report_formats_response/report_format/name");
                                    xpaths.push_back("/get_report_formats_response/report_format/comment");
                                    xpaths.push_back("/get_report_formats_response/report_format/creation_time");
                                    xpaths.push_back("/get_report_formats_response/report_format/modification_time");
                                    xpaths.push_back("/get_report_formats_response/report_format/extension");
                                    xpaths.push_back("/get_report_formats_response/report_format/content_type");
                                    xpaths.push_back("/get_report_formats_response/report_format/summary");
                                    xpaths.push_back("/get_report_formats_response/report_format/description");
                                    get("<get_report_formats/>");
                                    break;
                                case 14:
                                    validators.insert(std::make_pair(std::make_pair(12, true), std::make_pair(false, 5)));
                                    xnodes.push_back("get_reports");
                                    xnodes.push_back("filter");
                                    xnodes.push_back("report_id"); 
                                    xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                       notes=0 overrides=0 rows=10000 delta_states=gn \
                                                       result_hosts_only=1 ignore_pagination=1__attr__");
                                    if (create(false)) {
                                        xpaths.push_back("/get_reports_response/report/report/results/result"); 
                                        xpaths.push_back("/get_reports_response/report/report/results/result/name");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/host");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/port");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/severity");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/name");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/family");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/bid");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/cve");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/xref");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/nvt/tags");
                                        xpaths.push_back("/get_reports_response/report/report/results/result/description");
                                        get(xret[0], "id", true, true);
                                    }
                                    break;
                                case 15:
                                    validators.insert(std::make_pair(std::make_pair(12, true), std::make_pair(false, 5)));
                                    validators.insert(std::make_pair(std::make_pair(13, true), std::make_pair(false, 6)));
                                    xnodes.push_back("get_reports");
                                    xnodes.push_back("filter");
                                    xnodes.push_back("report_id"); 
                                    xnodes.push_back("format_id"); 
                                    xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                       notes=0 overrides=0 rows=10000 delta_states=gn \
                                                       result_hosts_only=1 ignore_pagination=1__attr__");
                                    if (create(false)) {
                                        xpaths.push_back("/get_reports_response/report"); 
                                        if (get(xret[0], "extension", false, true))
                                            write();
                                    }
                                    break;
                                default:
                                    break;
                            }
                            std::vector<std::string>().swap(xpaths);
                            break;
                        default:
                            break;
                    }
                    std::vector<std::string>().swap(xnodes);
                    std::vector<std::string>().swap(xvalues);
                }
                std::string().swap(oret);
                std::vector<std::string>().swap(xret);
                std::map<std::pair<int, bool>, std::pair<bool, int>>().swap(validators);
                break;
            default:
                form_driver(*ui.p_form, key);
                break;
        }
    } while (key != KEY_QUIT);
   
    //quit();
}

bool Nomp::get(const std::string &args, const std::string &attr,
               const bool &get_data, const bool &is_report)
{
    if (omp(args)) {
        Xml xml;
        if (xml.parse(&oret, &xpaths, &xret, attr, get_data, is_report)) {
            if (get_data)
                fill(is_report);
        } else {
            ui.status(std::make_pair("XML INTERNAL ERROR", 4));
            return false;
        }
    } else {
        ui.status(std::make_pair("GET RESOURCE ERROR", 4));
        return false;
    }

    return true;
}

bool Nomp::create(const bool &exec)
{
    if (validate(xvalues)) {
        Xml xml;
        if (xml.create(&xnodes, &xvalues, &xret, !exec)) {
            if (exec) {
                if (omp(xret[0])) {
                    ui.status(std::make_pair("RESOURCE CREATED", 7));
                } else {    
                    ui.status(std::make_pair("CREATE RESOURCE ERROR", 4));
                    return false;
                }
            }
        } else {
            ui.status(std::make_pair("XML INTERNAL ERROR", 4));
            return false;
        }
    } else {
        return false;
    }

    return true;
}

void Nomp::write()
{
    passwd *pw = getpwuid(getuid());
    
    std::ofstream file(std::string(pw->pw_dir) + "/.nomp/" + ids[5] + "." + xret[1]); 
    if (file.is_open()) {
        std::vector<BYTE> data = base64_decode(xret[2]);
        for (std::size_t i = 0; i < data.size(); i++)
            file << data[i];
        file.close();
        ui.status(std::make_pair("EXPORTED " + ids[5] + "." + xret[1], 7));
    } else {
        ui.status(std::make_pair("EXPORT ERROR", 4));
    }
}

bool Nomp::validate(std::vector<std::string> &v)
{
    for (std::map<std::pair<int, bool>, std::pair<bool, int>>::iterator it = validators.begin();
         it != validators.end(); ++it) {
        if (it->first.second) {
            if (!isblank(field_buffer(ui.p_fields[it->first.first], 0)[0])) {
                if (it->second.first)
                    v.push_back(trim(field_buffer(ui.p_fields[it->first.first], 0)));
                else if (it->second.second != -1)
                    v.push_back(ids[it->second.second] + "__attr__");
            } else {
                int n = it->first.first;
                if (!is_login) {
                   if (n < 11)
                       n += 5;
                   else   
                       n += 6;
                }
                ui.status(std::make_pair("THE " + ui.fields_name[n] + " FIELD CANNOT BE BLANK", 5), is_login);
                return false;
            }
        } else if (it->second.first) {
            v.push_back(trim(field_buffer(ui.p_fields[it->first.first], 0)));
        }
    }
    
    return true;
}

inline std::string Nomp::trim(const char *c)
{
    std::string str(c);
    
    str.erase(unique(str.begin(), str.end(), [] (char a, char b) {
        return isspace(a) && isspace(b);}), str.end());
    str.replace((str.size() - 1), 1, "");

    return str;
}

void Nomp::fill(const bool &is_report)
{
    int c_item;
   
    is_auto_refresh_blocked = true;

    if (is_report)
        c_item = ui.report(&xret, xpaths.size());
    else
        c_item = ui.menu(&xret, xpaths.size());
                            
    if (c_item >= 0) {
        switch(c_field)
        {
            case 2:
                ids[0] = xret[c_item];
                break;
            case 5:
                ids[1] = xret[c_item];
                break;
            case 6:
                ids[2] = xret[c_item];
                break;
            case 8:
                ids[3] = xret[c_item];
                break;
            case 9:
                ids[4] = xret[c_item];
                break;
            case 12:
                ids[5] = xret[c_item];
                break;
            case 13:
                ids[6] = xret[c_item];
                break;
            default:
                break;
        }
        set_field_buffer(ui.p_fields[c_field], 0, xret[((xret.size() / xpaths.size()) + c_item)].c_str());
    }

    is_auto_refresh_blocked = false;

    ui.delete_windows_arr();
    touchwin(stdscr);
}

void Nomp::auto_refresh()
{
    if (is_task_running && !is_auto_refresh_blocked) {
        std::map<std::pair<int, bool>, std::pair<bool, int>>().swap(validators);
        std::vector<std::string>().swap(xnodes);
        std::vector<std::string>().swap(xvalues);
        std::vector<std::string>().swap(xpaths);
        std::vector<std::string>().swap(xret);
        std::string().swap(oret);
    
        validators.insert(std::make_pair(std::make_pair(8, true), std::make_pair(false, 3)));
        xnodes.push_back("get_tasks");
        xnodes.push_back("id");
    
        if (create(false)) {
            xpaths.push_back("/get_tasks_response/task/progress");
            if (get(xret[0], "", false, true))
                ui.progress(xret.back());
        } 
    }
    
    if (is_task_running) {
        if (xret.back() == "-1") {
            is_task_running = false;
            is_auto_refresh_blocked = false;
        } else {
            std::thread t(&Nomp::auto_refresh_sleep, this);
            t.detach();
        }
    }
}

void Nomp::auto_refresh_sleep()
{
    std::this_thread::sleep_for(std::chrono::seconds(stoi(ids[4])));
    auto_refresh();
}

bool Nomp::omp(const std::string &args)
{
    char buf[BUFSIZ];
    const std::string cmd = "omp -h " + user_configs[0] +
                            " -p "    + user_configs[1] +
                            " -u "    + user_configs[2] + 
                            " -w "    + user_configs[3] + 
                            " -X '"   + args + "'"      +
                            " 2>/dev/null";

    FILE *fp = popen(cmd.c_str(), "r");
    if (!fp)
        return false;
    
    while (!feof(fp))
        if (fgets(buf, BUFSIZ, fp) != NULL)
            oret += buf;

    if (pclose(fp) != 0)
        return false;

    return true;
}
