#include <string>
#include <vector>

#include "ui.h"
#include "xml.h"
#include "command.h"

using namespace std;

class Nomp
{
public:
    Nomp();
    ~Nomp();

private:
    Ui ui;
    Xml xml;
    Command command;

    bool is_logged;
    bool is_task_running;
    
    const vector<string> times = {"30  sec", "60  sec", "120 sec", "300 sec",
                                  "30", "60", "120", "300"};
    
    const string path_port_lists = "/get_port_lists_response/port_list"; 
    const string path_configs = "/get_configs_response/config"; 
    const string path_targets = "/get_targets_response/target"; 
    const string path_tasks = "/get_tasks_response/task"; 
    const string path_report_formats = "/get_report_formats_response/report_format"; 

    string port_list_id;
    string config_id;
    string target_id;
    string task_id;
    string time_id;
    string report_format_id;
    string cmd;
    string ret_exec;
    
    vector<int> f;
    vector<string> user_configs;
    vector<string> nodes_xml;
    vector<string> values_xml;
    vector<string> paths_xml;
    vector<string> ret_xml;

    void driver();
    void refresh();
    void refresh_sleep();
};
