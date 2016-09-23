#include <vector>

using namespace std;

class Xml
{
public:
    Xml();
    ~Xml();

    vector<string> parse(string *str_xml, vector<string> *paths);
};
