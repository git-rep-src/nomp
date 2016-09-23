#include <string>
#include <vector>

#include "ui.h"
#include "str.h"
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

    const string path_login = "/get_version_response"; 
    const string path_targets = "/get_targets_response/target/name"; 
    
    string ret_cmd;
    vector<string> paths;
    vector<string> ret_xml;

    void driver();
};
