#include "nomp.h"
#include "xml.h"
#include "base64.h"

#include <thread>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

using std::ifstream;
using std::ofstream;
using std::make_pair;

Nomp::Nomp() :
    c_field(2),
    n_tabs(0),
    is_authentication(true),
    is_task_running(false),
    is_task_resumed(false),
    is_auto_refresh_blocked(false),
    ids(8)
{
    disk(true, false, false);
    driver();
}

Nomp::~Nomp()
{
}

void Nomp::driver()
{ 
    int key;

    do {
        prefresh(ui.window, n_tabs, 0, 0, 0, (LINES - 1), (COLS - 1));
        key = wgetch(ui.window);
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
                if (ui.has_status) {
                    for (int i = 0; i < COLS; i++)
                        mvwdelch(ui.window, (0 + n_tabs), 0);
                    ui.has_status = false;
                }
                switch(c_field)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        if (is_authentication)
                            if (c_field == 3)
                                set_field_back(ui.fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(1));
                        else
                            if (c_field == 3)
                                ui.marker(true, false);
                            else if (c_field == 4)
                                ui.marker(false, false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(1));
                        break;
                    case 7:
                    case 8:
                    case 10:
                    case 11:
                    case 14:
                    case 15:
                    case 16:
                        ui.marker(true, false);
                        break;
                    case 9:
                    case 12:
                    case 13:
                    case 17:
                    case 18:
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
                    case 5:
                    case 6:
                        if (is_authentication)
                            if (c_field == 3)
                                set_field_back(ui.fields[c_field], COLOR_PAIR(3));
                            else if (c_field == 4)
                                ui.marker(false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(2));
                        else
                            if (c_field == 3)
                                ui.marker();
                            else if (c_field == 4)
                                ui.marker(false);
                            else
                                set_field_back(ui.fields[c_field], COLOR_PAIR(2));
                        break;
                    case 7:
                    case 8:
                    case 10:
                    case 11:
                    case 14:
                    case 15:
                    case 16:
                        ui.marker();
                        break;
                    case 9:
                    case 12:
                    case 13:
                    case 17:
                    case 18:
                        ui.marker(false);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_UP_SCREEN:
                if (ui.has_status) {
                    for (int i = 0; i < COLS; i++)
                        mvwdelch(ui.window, (0 + n_tabs), 0);
                    ui.has_status = false;
                }
                if (n_tabs > 0)
                    --n_tabs;
                break;
            case KEY_DOWN_SCREEN:
                if (ui.has_status) {
                    for (int i = 0; i < COLS; i++)
                        mvwdelch(ui.window, (0 + n_tabs), 0);
                    ui.has_status = false;
                }
                if ((is_authentication && ((LINES + n_tabs) < 15)) ||
                    (!is_authentication && ((LINES + n_tabs) < 45)))
                    ++n_tabs;
                prefresh(ui.window, 0, 0, 0, 0, (LINES - 1), (COLS - 1));
                break;
            case KEY_BACKSPACE:
            case KEY_DELCHAR:
            case KEY_DC:
                form_driver(ui.form, REQ_LEFT_CHAR);
                form_driver(ui.form, REQ_DEL_CHAR);
                break;
            case KEY_RETURN:
                if (is_authentication) {
                    if (c_field == 4) {
                        vector<string>().swap(user_configs);
                        for (int i = 0; i <= 3; i++)
                            validators.insert(make_pair(make_pair(i, true), make_pair(true, -1)));
                        if (authenticate()) {
                            is_authentication = false;
                            c_field = 0;
                            ui.clear_form();
                            ui.main();
                        }
                    }
                } else {
                    switch(c_field)
                    {
                        case 3:
                            xpaths.push_back("/get_port_lists_response");
                            xpaths.push_back("/get_port_lists_response/port_list"); 
                            xpaths.push_back("/get_port_lists_response/port_list/name");
                            xpaths.push_back("/get_port_lists_response/port_list/name");
                            xpaths.push_back("/get_port_lists_response/port_list/comment");
                            xpaths.push_back("/get_port_lists_response/port_list/creation_time");
                            xpaths.push_back("/get_port_lists_response/port_list/modification_time");
                            xpaths.push_back("/get_port_lists_response/port_list/port_count/all"); 
                            xpaths.push_back("/get_port_lists_response/port_list/port_count/tcp"); 
                            xpaths.push_back("/get_port_lists_response/port_list/port_count/udp"); 
                            get_resource("<get_port_lists/>");
                            break;
                        case 4:
                            xnodes.push_back("create_target");
                            xnodes.push_back("name");
                            xnodes.push_back("comment");
                            xnodes.push_back("hosts");
                            xnodes.push_back("port_list");
                            validators.insert(make_pair(make_pair(0, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(1, false), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(2, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(3, true), make_pair(false, 0)));
                            create_resource();
                            break;
                        case 7:
                            xpaths.push_back("/get_configs_response"); 
                            xpaths.push_back("/get_configs_response/config"); 
                            xpaths.push_back("/get_configs_response/config/name");
                            xpaths.push_back("/get_configs_response/config/name");
                            xpaths.push_back("/get_configs_response/config/comment");
                            xpaths.push_back("/get_configs_response/config/creation_time");
                            xpaths.push_back("/get_configs_response/config/modification_time");
                            xpaths.push_back("/get_configs_response/config/family_count");
                            xpaths.push_back("/get_configs_response/config/nvt_count");
                            get_resource("<get_configs/>");
                            break;
                        case 8:
                            xpaths.push_back("/get_targets_response"); 
                            xpaths.push_back("/get_targets_response/target"); 
                            xpaths.push_back("/get_targets_response/target/name");
                            xpaths.push_back("/get_targets_response/target/name");
                            xpaths.push_back("/get_targets_response/target/comment");
                            xpaths.push_back("/get_targets_response/target/creation_time");
                            xpaths.push_back("/get_targets_response/target/modification_time");
                            xpaths.push_back("/get_targets_response/target/hosts");
                            xpaths.push_back("/get_targets_response/target/port_list/name");
                            get_resource("<get_targets/>");
                            break;
                        case 9:
                            xnodes.push_back("create_task");
                            xnodes.push_back("name");
                            xnodes.push_back("comment");
                            xnodes.push_back("config");
                            xnodes.push_back("target");
                            validators.insert(make_pair(make_pair(5, true), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(6, false), make_pair(true, -1)));
                            validators.insert(make_pair(make_pair(7, true), make_pair(false, 1)));
                            validators.insert(make_pair(make_pair(8, true), make_pair(false, 2)));
                            create_resource();
                            break;
                        case 10:
                            if (!is_task_running) {
                                xpaths.push_back("/get_tasks_response"); 
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
                                get_resource("<get_tasks/>");
                            }
                            break;
                        case 11:
                            if (!is_task_running) {
                                for (int i = 0; i < 3; i++)
                                    xpaths.push_back("");
                                xret = auto_refresh_times;
                                fill_items(false);
                            }
                            break;
                        case 12:
                            if (!is_task_running) {
                                if (string(field_buffer(ui.fields[12], 0)).find("START") != string::npos) 
                                    xnodes.push_back("start_task");
                                else
                                    xnodes.push_back("resume_task");
                                xnodes.push_back("task_id");
                                validators.insert(make_pair(make_pair(10, true), make_pair(false, 3)));
                                validators.insert(make_pair(make_pair(11, true), make_pair(false, 4)));
                                if (create_resource()) {
                                    is_task_running = true;
                                    auto_refresh();
                                    if (string(field_buffer(ui.fields[12], 0)).find("START") != string::npos) 
                                        ui.status("TASK STARTED - CTRL+E EXIT (TASK RUNNING ON BACKGROUND)", n_tabs);
                                    else    
                                        ui.status("TASK RESUMED - CTRL+E EXIT (TASK RUNNING ON BACKGROUND)", n_tabs);
                                }
                            }
                            break;
                        case 13:
                            is_auto_refresh_blocked = true;
                            xnodes.push_back("get_tasks");
                            xnodes.push_back("task_id");
                            validators.insert(make_pair(make_pair(10, true), make_pair(false, 3)));
                            if (create_resource(true)) {
                                xpaths.push_back("/get_tasks_response");
                                xpaths.push_back("/get_tasks_response/task/status");
                                if (get_resource(xret[0], "", false, true)) {
                                    if ((xret.back() == "Requested") || (xret.back() == "Running") || is_task_running) {
                                        clear_vectors();
                                        xnodes.push_back("stop_task");
                                        xnodes.push_back("task_id");
                                        validators.insert(make_pair(make_pair(10, true), make_pair(false, 3)));
                                        if (create_resource()) {
                                            is_task_running = false;
                                            if (is_task_resumed)
                                                disk(false, false, false);
                                            ui.progress("-2", n_tabs);
                                        }
                                    }
                                }
                            }
                            is_auto_refresh_blocked = false;
                            break;
                        case 14:
                            xnodes.push_back("get_tasks");
                            xnodes.push_back("filter");
                            xvalues.push_back("status=Done not last=__attr__");
                            if (create_resource(true)) {
                                xpaths.push_back("/get_tasks_response"); 
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
                                xpaths.push_back("/get_tasks_response/task/report_count");
                                get_resource(xret[0]);
                                set_field_buffer(ui.fields[15], 0, "");
                            }
                            break;
                        case 15:
                            xnodes.push_back("get_tasks");
                            xnodes.push_back("task_id");
                            validators.insert(make_pair(make_pair(14, true), make_pair(false, 5)));
                            if (create_resource(true)) {
                                xpaths.push_back("/get_tasks_response"); 
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report"); 
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/timestamp");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/timestamp");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/scan_end");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/debug");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/hole");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/info");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/log");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/warning");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/result_count/false_positive");
                                xpaths.push_back("/get_tasks_response/task/*[(name() = 'first_report') or \
                                                 (name() = 'last_report')]/report/severity");
                                get_resource(xret[0]);
                            }
                            break;
                        case 16:
                            xpaths.push_back("/get_report_formats_response"); 
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
                            get_resource("<get_report_formats/>");
                            break;
                        case 17:
                            if (!ui.is_empty_report) {
                                xnodes.push_back("get_reports");
                                xnodes.push_back("filter");
                                xnodes.push_back("report_id"); 
                                xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                                   notes=0 overrides=0 rows=10000 delta_states=gn \
                                                   result_hosts_only=1 ignore_pagination=1__attr__");
                                validators.insert(make_pair(make_pair(14, true), make_pair(false, -1)));
                                validators.insert(make_pair(make_pair(15, true), make_pair(false, 6)));
                                if (create_resource(true)) {
                                    xpaths.push_back("/get_reports_response"); 
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
                                    get_resource(xret[0], "id", true, true);
                                }
                            } else {
                                ui.status("REPORT IS EMPTY", n_tabs);
                            }
                            break;
                        case 18:
                            xnodes.push_back("get_reports");
                            xnodes.push_back("filter");
                            xnodes.push_back("report_id"); 
                            xnodes.push_back("format_id"); 
                            xvalues.push_back("sort-reverse=severity first=1 levels=hmlg autofp=0 \
                                               notes=0 overrides=0 rows=10000 delta_states=gn \
                                               result_hosts_only=1 ignore_pagination=1__attr__");
                            validators.insert(make_pair(make_pair(14, true), make_pair(false, -1)));
                            validators.insert(make_pair(make_pair(15, true), make_pair(false, 6)));
                            validators.insert(make_pair(make_pair(16, true), make_pair(false, 7)));
                            if (create_resource(true)) {
                                xpaths.push_back("/get_reports_response"); 
                                xpaths.push_back("/get_reports_response/report"); 
                                if (get_resource(xret[0], "extension", false, true))
                                    disk(false, true, true);
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
        int sret = ui.save();
        if (sret == 1)
            disk(false, true, false);
        else if (sret == -1)
            driver();
    }
}

bool Nomp::authenticate(bool has_user_configs)
{
    if (!has_user_configs)
        if (!validate_fields(user_configs))
            return false;

    xnodes.push_back("authenticate");
    xnodes.push_back("credentials");
    xnodes.push_back("username");
    xnodes.push_back("password");
    xvalues.push_back("");
    xvalues.push_back(user_configs[2]);
    xvalues.push_back(user_configs[3]);
    
    Xml xml;
    if (xml.create(&xnodes, &xvalues, &xret, false)) {
        if (!socket.is_started) {
            if (!socket.start(user_configs[0], user_configs[1])) {
                ui.status("SOCKET ERROR", n_tabs);
                return false;
            }
        }
        xpaths.push_back("/authenticate_response"); 
        if (!omp(xret[0]) || !(xml.parse(&ret, &xpaths, &xret, "id", false, false))) {
            ui.status("AUTHENTICATION ERROR", n_tabs);
            return false;
        }
    } else {
        ui.status("XML INTERNAL ERROR", n_tabs);
        return false;
    }

    return true;
}

bool Nomp::get_resource(const string &cmd, const string &attr, bool get_details, bool is_report)
{
    if (omp(cmd)) {
        if (!is_report)
            vector<string>().swap(xret);
        Xml xml;
        if (xml.parse(&ret, &xpaths, &xret, attr, get_details, is_report)) {
            if (get_details && (xret.size() > 0))
                fill_items(is_report);
        } else {
            ui.status("XML INTERNAL ERROR", n_tabs);
            return false;
        }
    } else {
        ui.status("GET RESOURCE ERROR", n_tabs);
        return false;
    }

    return true;
}

bool Nomp::create_resource(bool only_cmd)
{
    if (validate_fields(xvalues)) {
        Xml xml;
        if (xml.create(&xnodes, &xvalues, &xret, only_cmd)) {
            if (!only_cmd) {
                if (omp(xret[0])) {
                    if ((xnodes[0] != "start_task") && (xnodes[0] != "stop_task"))
                        ui.status("RESOURCE CREATED", n_tabs);
                } else {    
                    ui.status("CREATE RESOURCE ERROR", n_tabs);
                    return false;
                }
            }
        } else {
            ui.status("XML INTERNAL ERROR", n_tabs);
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool Nomp::validate_fields(vector<string> &v)
{
    for (map<pair<int, bool>, pair<bool, int>>::iterator it = validators.begin();
         it != validators.end(); ++it) {
        if (it->first.second) {
            if (!isblank(field_buffer(ui.fields[it->first.first], 0)[0])) {
                if (it->second.first)
                    v.push_back(clear_whitespaces(field_buffer(ui.fields[it->first.first], 0)));
                else if (it->second.second != -1)
                    v.push_back(ids[it->second.second] + "__attr__");
            } else {
                int n = it->first.first;
                if (!is_authentication) {
                   if (n < 13)
                       n += 5;
                   else   
                       n += 7;
                }
                ui.status("THE " + ui.field_names[n] + " FIELD CANNOT BE BLANK", n_tabs);
                return false;
            }
        } else if (it->second.first) {
            v.push_back(clear_whitespaces(field_buffer(ui.fields[it->first.first], 0)));
        }
    }
    
    return true;
}

void Nomp::fill_items(bool is_report)
{
    int c_item;
   
    is_auto_refresh_blocked = true;

    if (is_report)
        c_item = ui.report(&xret, (xpaths.size() - 1));
    else
        c_item = ui.menu(&xret, (xpaths.size() - 1), n_tabs);
                            
    if (c_item >= 0) {
        switch(c_field)
        {
            case 3:
                ids[0] = xret[c_item];
                break;
            case 7:
                ids[1] = xret[c_item];
                break;
            case 8:
                ids[2] = xret[c_item];
                break;
            case 10:
                ids[3] = xret[c_item];
                break;
            case 11:
                ids[4] = xret[c_item];
                break;
            case 14:
                ids[5] = xret[c_item];
                break;
            case 15:
                ids[6] = xret[c_item];
                break;
            case 16:
                ids[7] = xret[c_item];
                break;
            default:
                break;
        }
        set_field_buffer(ui.fields[c_field], 0, xret[((xret.size() / (xpaths.size() - 1)) + c_item)].c_str());
    }

    is_auto_refresh_blocked = false;
    ui.clear_items();
}

void Nomp::disk(bool read, bool write, bool is_report)
{
    passwd *pw = getpwuid(getuid());
    string home_path = pw->pw_dir;
    vector<string> configs = {"host=", "port=", "username=", "password="};
    
    struct stat st;
    if (!((stat((home_path + "/.nomp/reports").c_str(), &st) == 0) && S_ISDIR(st.st_mode)))
        cli("mkdir -p", (home_path + "/.nomp/reports"));
    
    if (read) {
        bool is_user_configs = false;
        bool is_configs_ok = false;
        string line;
        string task_name;
        string refresh_name;
        ifstream file;
        file.open(home_path + "/.nomp/.task.tmp");
        if (file.is_open()) {
            configs.push_back("task_name="); 
            configs.push_back("task_id="); 
            configs.push_back("refresh_name="); 
            configs.push_back("refresh_id="); 
        } else {
            file.open(home_path + "/.nomp/config");
            if (file.is_open())
                is_user_configs = true;
        }
        if (file.is_open()) {
            for (size_t i = 0; i < configs.size(); i++) {
                while (getline(file, line)) {
                    if (line.find(configs[i]) != string::npos) {
                        if ((line.erase(0, configs[i].size())).size() != 0) {
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
                            is_configs_ok = true;
                        }
                        break;
                    }
                }
                if ((i < (configs.size() - 1)) && is_configs_ok)
                    is_configs_ok = false;
                else
                    break;
            }
            file.close();
        }
        if (is_configs_ok) {
            if (authenticate(true)) {
                if (is_user_configs) {
                    c_field = 0;
                    is_authentication = false;
                    ui.main();
                } else {
                    c_field = 0;
                    is_authentication = false;
                    is_task_running = true;
                    is_task_resumed = true;
                    ui.main();
                    set_field_buffer(ui.fields[10], 0, task_name.c_str());
                    set_field_buffer(ui.fields[11], 0, refresh_name.c_str());
                    curs_set(0);
                    auto_refresh();
                }
            } else {
                is_configs_ok = false;
            }
        }
        if (!is_configs_ok) {
            vector<string>().swap(user_configs);
            user_configs.push_back("localhost");
            user_configs.push_back("9390");
            user_configs.push_back("");
            user_configs.push_back("");
            ui.authenticate(&user_configs);
        }
        clear_vectors();
    } else if (write){
        ofstream file;
        if (is_report) {
            file.open(home_path + "/.nomp/reports/" + ids[5] + "." + xret[1]);
            if (file.is_open()) {
                vector<BYTE> data = base64_decode(xret[2]);
                for (size_t i = 0; i < data.size(); i++)
                    file << data[i];
                file.close();
                ui.status("EXPORTED " + home_path + "/.nomp/reports/" + ids[5] + "." + xret[1], n_tabs);
            } else {
                ui.status("EXPORT ERROR", n_tabs);
            }
        } else {
            configs.push_back("task_name="); 
            configs.push_back("task_id="); 
            configs.push_back("refresh_name="); 
            configs.push_back("refresh_id="); 
            file.open(home_path + "/.nomp/.task.tmp");
            if (file.is_open()) {
                for (size_t i = 0; i < configs.size(); i++) {
                    if (i <= 3)
                        file << configs[i] << user_configs[i] << std::endl;
                    else if (i == 4)
                        file << configs[i] << clear_whitespaces(field_buffer(ui.fields[10], 0)) << std::endl;
                    else if (i == 5)
                        file << configs[i] << ids[3] << std::endl;
                    else if (i == 6)
                        file << configs[i] << clear_whitespaces(field_buffer(ui.fields[11], 0)) << std::endl;
                    else
                        file << configs[i] << ids[4] << std::endl;
                }
                file.close();
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

bool Nomp::cli(const string &cmd, const string &args)
{
    char buf[BUFSIZ];
    string command = cmd + " " + args + " 2>/dev/null";
   
    FILE *fp = popen(command.c_str(), "r");
    if (!fp)
        return false;
    
    while (!feof(fp))
        if (fgets(buf, BUFSIZ, fp) != NULL)
            ret += buf;

    if (pclose(fp) != 0)
        return false;

    return true;
}

bool Nomp::omp(const string &cmd)
{
    if (!socket.write_read(&cmd, &ret))
        return false;

    return true;
}

void Nomp::auto_refresh()
{
    if (is_task_running && !is_auto_refresh_blocked) {
        clear_vectors();
        xnodes.push_back("get_tasks");
        xnodes.push_back("task_id");
        validators.insert(make_pair(make_pair(10, true), make_pair(false, 3)));
        if (create_resource(true)) {
            xpaths.push_back("/get_tasks_response");
            xpaths.push_back("/get_tasks_response/task/progress");
            xpaths.push_back("/get_tasks_response/task/status");
            if (get_resource(xret[0], "", false, true)) {
                if ((xret.back() == "Requested") || (xret.back() == "Running"))
                    ui.progress(xret[1], n_tabs);
                else if (xret.back() == "Stopped")
                    ui.progress("-2", n_tabs);
                else if (xret.back() == "Done")
                    ui.progress("-1", n_tabs);
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

inline string Nomp::clear_whitespaces(const char *c)
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
    string().swap(ret);
}
