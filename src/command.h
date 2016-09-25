#include <string>
#include<vector>

using namespace std;

class Command
{
public:
    Command();
    ~Command();

    const string build(vector<string> *user_configs, const string arg);
    string execute(const string cmd);
};
