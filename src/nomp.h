#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "ui.h"

using namespace std;

class Nomp
{
public:
    Nomp();
    ~Nomp();

private:
    Ui ui;

    int c_field;

    bool is_login;
    bool is_task_running;
    
    string oret;
    string extension;
    stringstream status;

    vector<string> ids;
    vector<string> user_configs;
    vector<string> xnodes;
    vector<string> xvalues;
    vector<string> xpaths;
    vector<string> xret;
    const vector<string> refreshes = {"30", "60", "120", "300", " 30 sec", " 60 sec","120 sec", "300 sec"};
    
    map<pair<int, bool>, pair<bool, int>> validators;

    void driver();
    bool get(string args, string attr = "id", bool get_data = true, bool is_report = false);
    bool create(bool exec = true);
    void write();
    bool validate(vector<string> &vec);
    void fill(bool is_report);
    void refresh();
    void refresh_sleep();
    bool omp(const string args);
};
