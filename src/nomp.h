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
    
    const vector<string> times = {
        "30", "60", "120", "300",
        " 30 sec", " 60 sec", "120 sec", "300 sec",
        "REFRESH  30 seconds", "REFRESH  60 seconds",
        "REFRESH  2 minutes", "REFRESH  5 minutes"
    };
   
    string port_list_id;
    string config_id;
    string target_id;
    string task_id;
    string refresh_id;
    string report_id;
    string report_format_id;
    string cmd;
    string cret;

    vector<string> user_configs;
    vector<string> xnodes;
    vector<string> xvalues;
    vector<string> xpaths;
    vector<string> xret;
    vector<int> i_fields;

    void driver();
    void get(string cmd_opt, bool get_data = true,
             string attr_name = "id", bool is_report = false);
    void refresh();
    void refresh_sleep();
};
