#include <vector>

using namespace std;

class Xml
{
public:
    Xml();
    ~Xml();

    vector<string> build(vector<string> *nodes, vector<string> *values);
    vector<string> parse(string *str, vector<string> *paths);
};
