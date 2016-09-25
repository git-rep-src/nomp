#include <stdexcept>

#include "command.h"

Command::Command()
{
}

Command::~Command()
{
}

const string Command::create(vector<string> *user_configs, const string args) 
{
    const string ret = "omp -h " + (*user_configs)[0] +
                       " -p " + (*user_configs)[1] +
                       " -u " + (*user_configs)[2] + 
                       " -w " + (*user_configs)[3] + 
                       " -X '" + args + "'";
    
    return ret;
}

string Command::execute(const string cmd)
{
    char buf[BUFSIZ];
    string ret = "";

    FILE *fp = popen(cmd.c_str(), "r");
    if (!fp)
        throw ("ERROR: Failed to run command");
    try {
        while (!feof(fp))
            if (fgets(buf, BUFSIZ, fp) != NULL)
                ret += buf;
    } catch (const string e) {
        pclose(fp);
        return e;
    }
    if (pclose(fp) != 0)
        ret = "ERROR";

    return ret;
}
