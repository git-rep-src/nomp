#include <string>
#include<vector>

using namespace std;

class Command
{
public:
    Command();
    ~Command();

    const string create(vector<string> *user_configs, const string args);
    string execute(const string cmd);
};
