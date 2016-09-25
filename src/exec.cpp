#include <stdexcept>

#include "exec.h"

Exec::Exec()
{
}

Exec::~Exec()
{
}

string Exec::request(const string cmd)
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
        ret = "ERROR:";

    return ret;
}
