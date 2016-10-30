#include "nomp.h"
#include "xml.h"
#include "base64.h"

#include <thread>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <iostream>//

using std::ifstream;
using std::ofstream;
using std::make_pair;

Nomp::Nomp() :
    c_field(2),
    is_login(true),
    is_task_running(false),
    is_task_resumed(false),
    is_auto_refresh_blocked(false),
    ids(7)
{
    disk(true, true, false);
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
                form_driver(ui.form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(ui.form, REQ_NEXT_CHAR);
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
                                set_field_back(ui.fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(1));
                        else
                            if (c_field == 2)
                                ui.marker(true, false);
                            else if (c_field == 3)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(1));
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
                        c_field = (field_count(ui.form) - 1);
                    else
                        --c_field;
                    form_driver(ui.form, REQ_PREV_FIELD);
                    form_driver(ui.form, REQ_END_LINE);
                } else {
                    if (c_field == (field_count(ui.form) - 1))
                        c_field = 0;
                    else
                        ++c_field;
                    form_driver(ui.form, REQ_NEXT_FIELD);
                    form_driver(ui.form, REQ_END_LINE);
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
                                set_field_back(ui.fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(2));
                        else
                            if (c_field == 2)
                                ui.marker();
                            else if (c_field == 3)
                                ui.marker(false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(2));
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
                form_driver(ui.form, REQ_LEFT_CHAR);
                form_driver(ui.form, REQ_DEL_CHAR);
                break;
            case KEY_RETURN:
                if (is_login) {
                    if (c_field == 4) {
                        vector<string>().swap(user_configs);
                        for (int i = 0; i <= 3; i++)
                            validators.insert(make_pair(make_pair(i, true), make_pair(true, -1)));
                        if (validate(user_configs)) {
                            if (exec("omp", "<get_version/>")) {
                                is_login = false;
                                c_field = 0;
                                ui.cleanup();
                                ui.main();
                            } else {
                                ui.status("LOGIN ERROR");
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
                            validators.insert(make_pair(make_pair(0, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(1, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(2, true), make_pair(false, 0)));
                            create();
                            break;
                        case 7:
                            xnodes.push_back("create_task");
                            xnodes.push_back("name");
                            xnodes.push_back("config");
                            xnodes.push_back("target");
                            validators.insert(make_pair(make_pair(4, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(5, true), make_pair(false, 1)));
                            validators.insert(make_pair(make_pair(6, true), make_pair(false, 2)));
                            create();
                            break;
                        case 10:
                            if (!is_task_running) {
                                if (string(field_buffer(ui.fields[10], 0)).find("START") != string::npos) 
                                    xnodes.push_back("start_task");
                                else
                                    xnodes.push_back("resume_task");
                                xnodes.push_back("task_id");
                                validators.insert(make_pair(make_pair(8, true), make_pair(false, 3)));
                                validators.insert(make_pair(make_pair(9, true), make_pair(false, 4)));
                                if (create()) {
                                    is_task_running = true;
                                    auto_refresh();
                                    if (string(field_buffer(ui.fields[10], 0)).find("START") != string::npos) 
                                        ui.status("TASK STARTED");
                                    else    
                                        ui.status("TASK RESUMED");
                                }
                            }
                            break;
                        case 11:
                            is_auto_refresh_blocked = true;
                            xnodes.push_back("get_tasks");
                            xnodes.push_back("task_id");
                            validators.insert(make_pair(make_pair(8, true), make_pair(false, 3)));
                            if (create(false)) {
                                xpaths.push_back("/get_tasks_response/task/status");
                                if (get(xret[0], "", false, true)) {
                                    if ((xret.back() == "Requested") || (xret.back() == "Running") || is_task_running) {
                                        clear_vectors();
                                        xnodes.push_back("stop_task");
                                        xnodes.push_back("task_id");
                                        validators.insert(make_pair(make_pair(8, true), make_pair(false, 3)));
                                        if (create()) {
                                            is_task_running = false;
                                            if (is_task_resumed)
                                                disk(false, false, false);
                                            ui.progress("-2");
                                        }
                                    }
                                }
                            }
                            is_auto_refresh_blocked = false;
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
                                    xnodes.push_back("get_tasks");
                                    xnodes.push_back("filter");
                                    xvalues.push_back("status=Done__attr__");
                                    if (create(false)) {
                                        xpaths.push_back("/get_tasks_response/task/first_report/report"); 
                                        xpaths.push_back("/get_tasks_response/task/name");
                                        xpaths.push_back("/get_tasks_response/task/name");
                                        xpaths.push_back("/get_tasks_response/task/comment");
                                        xpaths.push_back("/get_tasks_response/task/creation_time");
                                        xpaths.push_back("/get_tasks_response/task/modification_time");
                                        xpaths.push_back("/get_tasks_response/task/scanner/name");
                                        xpaths.push_back("/get_tasks_response/task/config/name");
                                        xpaths.push_back("/get_tasks_response/task/target/name");
                                        xpaths.push_back("/get_tasks_response/task/status");
                                        get(xret[0]);
                                    }
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
                                    xnodes.push_back("get_reports");
                                    xnodes.push_back("filter");
                                    xnodes.push_back("report_id"); 
                                    xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                       notes=0 overrides=0 rows=10000 delta_states=gn \
                                                       result_hosts_only=1 ignore_pagination=1__attr__");
                                    validators.insert(make_pair(make_pair(12, true), make_pair(false, 5)));
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
                                    xnodes.push_back("get_reports");
                                    xnodes.push_back("filter");
                                    xnodes.push_back("report_id"); 
                                    xnodes.push_back("format_id"); 
                                    xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                       notes=0 overrides=0 rows=10000 delta_states=gn \
                                                       result_hosts_only=1 ignore_pagination=1__attr__");
                                    validators.insert(make_pair(make_pair(12, true), make_pair(false, 5)));
                                    validators.insert(make_pair(make_pair(13, true), make_pair(false, 6)));
                                    if (create(false)) {
                                        xpaths.push_back("/get_reports_response/report"); 
                                        if (get(xret[0], "extension", false, true))
                                            disk(false, false, true);
                                    }
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                }
                clear_vectors();
                break;
            default:
                form_driver(ui.form, key);
                break;
        }
    } while (key != KEY_QUIT);
  
    if (is_task_running && !is_task_resumed) {
        int scret = ui.save_config();
        if (scret == 1)
            disk(true, false, true);
        else if (scret == -1)
            driver();
    }
}

bool Nomp::get(const string &args, const string &attr,
               const bool &get_data, const bool &is_report)
{
    if (exec("omp", args)) {
        if (!is_report)
            vector<string>().swap(xret);
        Xml xml;
        if (xml.parse(&eret, &xpaths, &xret, attr, get_data, is_report)) {
            if (get_data)
                fill(is_report);
        } else {
            ui.status("XML INTERNAL ERROR");
            return false;
        }
    } else {
        ui.status("GET RESOURCE ERROR");
        return false;
    }

    return true;
}

bool Nomp::create(const bool &is_exec)
{
    if (validate(xvalues)) {
        Xml xml;
        if (xml.create(&xnodes, &xvalues, &xret, !is_exec)) {
            if (is_exec) {
                if (exec("omp", xret[0])) {
                    if ((xnodes[0] != "start_task") && (xnodes[0] != "stop_task"))
                        ui.status("RESOURCE CREATED");
                } else {    
                    ui.status("CREATE RESOURCE ERROR");
                    return false;
                }
            }
        } else {
            ui.status("XML INTERNAL ERROR");
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool Nomp::validate(vector<string> &v)
{
    for (map<pair<int, bool>, pair<bool, int>>::iterator it = validators.begin();
         it != validators.end(); ++it) {
        if (it->first.second) {
            if (!isblank(field_buffer(ui.fields[it->first.first], 0)[0])) {
                if (it->second.first)
                    v.push_back(clear_whitespace(field_buffer(ui.fields[it->first.first], 0)));
                else if (it->second.second != -1)
                    v.push_back(ids[it->second.second] + "__attr__");
            } else {
                int n = it->first.first;
                if (!is_login) {
                   if (n < 11)
                       n += 5;
                   else   
                       n += 7;
                }
                ui.status("THE " + ui.fields_name[n] + " FIELD CANNOT BE BLANK");
                return false;
            }
        } else if (it->second.first) {
            v.push_back(clear_whitespace(field_buffer(ui.fields[it->first.first], 0)));
        }
    }
    
    return true;
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
        set_field_buffer(ui.fields[c_field], 0,
                         xret[((xret.size() / xpaths.size()) + c_item)].c_str());
    }

    is_auto_refresh_blocked = false;

    ui.clear_windows_arr();
    touchwin(stdscr);
}

void Nomp::disk(const bool &is_config, const bool &is_read, const bool &is_write)
{
    vector<string> cfs = {"host=", "port=", "username=", "password="};
    
    passwd *pw = getpwuid(getuid());
    string home_path = pw->pw_dir;
    
    struct stat st;
    if (!((stat((home_path + "/.nomp/reports").c_str(), &st) == 0) && S_ISDIR(st.st_mode)))
        exec("mkdir -p", (home_path + "/.nomp/reports"));
    
    if (is_read) {
        bool is_user_config = false;
        bool is_config_ok = false;
        string line;
        string task_name;
        string refresh_name;
        ifstream file;
        file.open(home_path + "/.nomp/.task.tmp");
        if (file.is_open()) {
            cfs.push_back("task_name="); 
            cfs.push_back("task_id="); 
            cfs.push_back("refresh_name="); 
            cfs.push_back("refresh_id="); 
        } else {
            file.open(home_path + "/.nomp/config");
            if (file.is_open())
                is_user_config = true;
        }
        if (file.is_open()) {
            for (size_t i = 0; i < cfs.size(); i++) {
                while (getline(file, line)) {
                    if (line.find(cfs[i]) != string::npos) {
                        if ((line.erase(0, cfs[i].size())).size() != 0) {
                            if (i == 4)
                                task_name = line;
                            else if (i == 5)
                                ids[3] = line;
                            else if (i == 6)
                                refresh_name = line;
                            else if (i == 7)
                                ids[4] = line;
                            else
                                user_configs.push_back(line);
                            is_config_ok = true;
                        }
                            break;
                    }
                }
                if ((i < (cfs.size() - 1)) && is_config_ok)
                    is_config_ok = false;
                else
                    break;
            }
            file.close();
        }
        if (is_config_ok) {
            if (exec("omp", "<get_version/>")) {
                if (is_user_config) {
                    c_field = 0;
                    is_login = false;
                    ui.main();
                } else {
                    c_field = 0;
                    is_login = false;
                    is_task_running = true;
                    is_task_resumed = true;
                    ui.main();
                    set_field_buffer(ui.fields[8], 0, task_name.c_str());
                    set_field_buffer(ui.fields[9], 0, refresh_name.c_str());
                    curs_set(0);
                    auto_refresh();
                }
            } else {
                is_config_ok = false;
            }
        }
        if (!is_config_ok) {
            vector<string>().swap(user_configs);
            user_configs.push_back("localhost");
            user_configs.push_back("9390");
            user_configs.push_back("");
            user_configs.push_back("");
            ui.login(&user_configs);
            vector<string>().swap(user_configs);
        }
        clear_vectors();
    } else if (is_write){
        ofstream file;
        if (is_config) {
            cfs.push_back("task_name="); 
            cfs.push_back("task_id="); 
            cfs.push_back("refresh_name="); 
            cfs.push_back("refresh_id="); 
            file.open(home_path + "/.nomp/.task.tmp");
            if (file.is_open()) {
                for (size_t i = 0; i < cfs.size(); i++) {
                    if (i <= 3)
                        file << cfs[i] << user_configs[i] << std::endl;
                    else if (i == 4)
                        file << cfs[i] << clear_whitespace(field_buffer(ui.fields[8], 0)) << std::endl;
                    else if (i == 5)
                        file << cfs[i] << ids[3] << std::endl;
                    else if (i == 6)
                        file << cfs[i] << clear_whitespace(field_buffer(ui.fields[9], 0)) << std::endl;
                    else
                        file << cfs[i] << ids[4] << std::endl;
                }
                file.close();
            }
        } else {
            file.open(home_path + "/.nomp/reports/" + ids[5] + "." + xret[1]);
            if (file.is_open()) {
                vector<BYTE> data = base64_decode(xret[2]);
                for (size_t i = 0; i < data.size(); i++)
                    file << data[i];
                file.close();
                ui.status("EXPORTED " + home_path + "/.nomp/reports/" + ids[5] + "." + xret[1]);
            } else {
                ui.status("EXPORT ERROR");
            }
        }
    } else {
        ifstream file(home_path + "/.nomp/.task.tmp");
        if (file.is_open()) {
            file.close();
            std::remove((home_path + "/.nomp/.task.tmp").c_str());
            is_task_resumed = false;
        }
    }
}

bool Nomp::exec(const string &cmd, const string &args)
{
    char buf[BUFSIZ];
    string command;

    if (cmd == "omp") {
        command = cmd + " -h "  + user_configs[0] +
                        " -p "  + user_configs[1] +
                        " -u "  + user_configs[2] + 
                        " -w "  + user_configs[3] + 
                        " -X '" + args + "'"      +
                        " 2>/dev/null";
    } else {
        command = cmd + " " + args + " 2>/dev/null";
    }

    FILE *fp = popen(command.c_str(), "r");
    if (!fp)
        return false;
    
    while (!feof(fp))
        if (fgets(buf, BUFSIZ, fp) != NULL)
            eret += buf;

    if (pclose(fp) != 0)
        return false;

    return true;
}

void Nomp::auto_refresh()
{
    if (is_task_running && !is_auto_refresh_blocked) {
        clear_vectors();
        xnodes.push_back("get_tasks");
        xnodes.push_back("task_id");
        validators.insert(make_pair(make_pair(8, true), make_pair(false, 3)));
        if (create(false)) {
            xpaths.push_back("/get_tasks_response/task/progress");
            xpaths.push_back("/get_tasks_response/task/status");
            if (get(xret[0], "", false, true)) {
                if ((xret.back() == "Requested") || (xret.back() == "Running"))
                    ui.progress(xret[1]);
                else if (xret.back() == "Stopped")
                    ui.progress("-2");
                else if (xret.back() == "Done")
                    ui.progress("-1");
            }
        } 
    }
    
    if (is_task_running) {
        if ((xret.back() == "Stopped") || (xret.back() == "Done")) {
            is_task_running = false;
            is_auto_refresh_blocked = false;
            if (is_task_resumed)
                disk(false, false, false);
        } else {
            std::thread t(&Nomp::auto_refresh_sleep, this);
            t.detach();
        }
        if (!is_auto_refresh_blocked)
            clear_vectors();
    }
}

void Nomp::auto_refresh_sleep()
{
    std::this_thread::sleep_for(std::chrono::seconds(stoi(ids[4])));
    auto_refresh();
}

inline string Nomp::clear_whitespace(const char *c)
{
    string str(c);
    
    str.erase(unique(str.begin(), str.end(), [] (char a, char b) {
        return isspace(a) && isspace(b);}), str.end());
    str.replace((str.size() - 1), 1, "");

    return str;
}

void Nomp::clear_vectors()
{
    map<pair<int, bool>, pair<bool, int>>().swap(validators);
    vector<string>().swap(xnodes);
    vector<string>().swap(xvalues);
    vector<string>().swap(xpaths);
    vector<string>().swap(xret);
    string().swap(eret);
}
