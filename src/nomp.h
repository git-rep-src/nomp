#ifndef NOMP_H
#define NOMP_H

#include "ssl_socket.h"
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
    SSL_socket socket;
    Ui ui;

    int c_field;
    int n_tabs;

    bool is_authentication;
    bool is_task_running;
    bool is_task_resumed;
    bool is_auto_refresh_blocked;
    
    string ret;

    vector<string> ids;
    vector<string> user_configs;
    vector<string> xnodes;
    vector<string> xvalues;
    vector<string> xpaths;
    vector<string> xret;
    const vector<string> auto_refresh_times =
    {"30", "60", "120", "300", "30 sec", "1 min", "2 min", "5 min"};
    
    map<pair<int, bool>, pair<bool, int>> validators;

    void driver();
    bool authenticate(bool has_user_configs = false);
    bool get_resource(const string &cmd, const string &attr = "id",
                      bool get_details = true, bool is_report = false);
    bool create_resource(bool only_cmd = false);//
    bool validate_fields(vector<string> &v);
    void fill_items(bool is_report);
    void disk(bool read, bool write, bool is_report);
    bool cli(const string &cmd, const string &args);
    bool omp(const string &cmd);
    void auto_refresh();
    void auto_refresh_sleep();
    string clear_whitespace(const char *c);
    void clear_vectors();
};

#endif
