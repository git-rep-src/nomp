#include <stdexcept>

#include "exec.h"

Exec::Exec()
{
}

Exec::~Exec()
{
}

string Exec::request(const char* cmd)
{
    char buf[BUFSIZ];
    string ret = "";

    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw runtime_error("ERROR: Command execution failed.");
    try {
        while (!feof(pipe)) {
            if (fgets(buf, BUFSIZ, pipe) != NULL)
                ret += buf;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    return ret;
}
