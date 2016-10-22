#ifndef NOMP_H
#define NOMP_H

#include <map>

#include "ui.h"

class Nomp
{
public:
    Nomp();
    ~Nomp();

private:
    Ui ui;

    unsigned int c_field;

    bool is_login;
    bool is_task_running;
    bool is_auto_refresh_blocked;
    
    std::string oret;

    std::vector<std::string> ids;
    std::vector<std::string> user_configs;
    std::vector<std::string> xnodes;
    std::vector<std::string> xvalues;
    std::vector<std::string> xpaths;
    std::vector<std::string> xret;
    const std::vector<std::string> auto_refresh_times =
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
    
    std::map<std::pair<int, bool>, std::pair<bool, int>> validators;

    void driver();
    bool get(const std::string &args, const std::string &attr = "id",
             const bool &get_data = true, const bool &is_report = false);
    bool create(const bool &exec = true);
    void write();
    bool validate(std::vector<std::string> &v);
    std::string trim(const char *c);
    void fill(const bool &is_report);
    void auto_refresh();
    void auto_refresh_sleep();
    bool omp(const std::string &args);
};

#endif
