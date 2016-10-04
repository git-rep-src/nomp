#include<string>
#include <vector>

using namespace std;

class Xml
{
public:
    Xml();
    ~Xml();

    vector<string> create(vector<string> *nodes, vector<string> *values);
    vector<string> parse(string *content, vector<string> *paths,
                         const string attr_name = "id", bool get_data = false);
};
