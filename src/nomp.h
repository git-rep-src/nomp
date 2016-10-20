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

    int c_field;

    bool is_login;
    bool is_task_running;
    bool is_refresh_blocked;
    
    std::string oret;
    std::string extension;

    std::vector<std::string> ids;
    std::vector<std::string> user_configs;
    std::vector<std::string> xnodes;
    std::vector<std::string> xvalues;
    std::vector<std::string> xpaths;
    std::vector<std::string> xret;
    const std::vector<std::string> refreshes =
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
    
    std::map<std::pair<int, bool>,std::pair<bool, int>> validators;

    void driver();
    bool get(std::string args, std::string attr = "id", bool get_data = true, bool is_report = false);
    bool create(bool exec = true);
    void write();
    bool validate(std::vector<std::string> &vec);
    void fill(bool is_report);
    void refresh();
    void refresh_sleep();
    bool omp(const std::string args);
};

#endif
