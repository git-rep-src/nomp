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
                         const string attr_name = "id", bool get_data = false,
                         bool is_report = false);
    void to_upper(string &str, bool is_report = false);
    void replace(string &str, vector<string> &finds, vector<string> &replaces);
    void wrap(string &str);
};
