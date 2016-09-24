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

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        throw ("ERROR"); //TODO: enviar el msg de error.
    try {
        while (!feof(pipe))
            if (fgets(buf, BUFSIZ, pipe) != NULL)
                ret += buf;
    } catch (const string e) {
        pclose(pipe);
        return e;
    }
    
    if (pclose(pipe) != 0)
        ret = "ERROR";

    return ret;
}
