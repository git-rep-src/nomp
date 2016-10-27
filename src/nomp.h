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
    bool is_task_resumed;
    bool is_auto_refresh_blocked;
    
    string oret;

    vector<string> ids;
    vector<string> user_configs;
    vector<string> xnodes;
    vector<string> xvalues;
    vector<string> xpaths;
    vector<string> xret;
    const vector<string> auto_refresh_times =
    {"30", "60", "120", "300", " 30 sec",
     "  1 min", "  2 min", "  5 min"};
    
    map<pair<int, bool>, pair<bool, int>> validators;

    void driver();
    bool get(const string &args, const string &attr = "id",
             const bool &get_data = true, const bool &is_report = false);
    bool create(const bool &is_exec = true);
    bool validate(vector<string> &v);
    void fill(const bool &is_report);
    void disk(const bool &is_config, const bool &is_read, const bool &is_write);
    bool exec(const string &cmd, const string &args);
    void auto_refresh();
    void auto_refresh_sleep();
    string clear_whitespace(const char *c);
    void clear_vectors();
};

#endif
