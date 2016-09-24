#include <string>
#include <vector>

#include "ui.h"
#include "xml.h"
#include "exec.h"

using namespace std;

class Nomp
{
public:
    Nomp();
    ~Nomp();

private:
    Ui ui;
    Xml xml;
    Exec exec;

    const string path_port_lists = "/get_port_lists_response/port_list/name"; 
    const string path_targets = "/get_targets_response/target/name"; 
    const string path_configs = "/get_configs_response/config/name"; 

    string port_list_id;
    string target_id;
    string config_id;

    bool is_logged;

    string ret_cmd;
    vector<int> f;
    vector<string> user_configs;
    vector<string> paths_xml;
    vector<string> ret_xml;

    void driver();
    const string build_command(const string arg); 
};
