#include <string>

using namespace std;

class Exec
{
public:
    Exec();
    ~Exec();

    string request(const string cmd);
};
