#ifndef NOMP_H
#define NOMP_H

#include "ui.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::pair;
using std::map;

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
    bool is_auto_refresh_blocked;
    
    string oret;

    vector<string> ids;
    vector<string> user_configs;
    vector<string> xnodes;
    vector<string> xvalues;
    vector<string> xpaths;
    vector<string> xret;
    const vector<string> auto_refresh_times =
    {
        "30",
        "60",
        "120",
        "300",
        " 30 sec",
        " 60 sec",
        "120 sec",
        "300 sec"
    };
    
    map<pair<int, bool>, pair<bool, int>> validators;

    void init();
    void driver();
    bool get(const string &args, const string &attr = "id",
             const bool &get_data = true, const bool &is_report = false);
    bool create(const bool &is_exec = true);
    void write();
    bool validate(vector<string> &v);
    string trim(const char *c);
    void fill(const bool &is_report);
    void auto_refresh();
    void auto_refresh_sleep();
    void clear_vectors();
    bool exec(const string &cmd, const string &args);
};

#endif
