#include <string>
#include <vector>

#include <libxml++/libxml++.h>

using namespace std;
using namespace xmlpp;

class Xml
{
public:
    Xml();
    ~Xml();

    bool create(vector<string> *nodes, vector<string> *values, vector<string> *xret,
                bool is_report = false);
    bool parse(string *content, vector<string> *paths, vector<string> *xret,
               const string attr = "id", bool get_data = true, bool is_report = false);
private:
    void set_format(xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                    vector<string> **xret, uint &i, uint &max_width);
    void set_wrap(string &str, uint p, bool replace = false);
    void set_width(string &str, uint &max_width);
    void set_uppercase(string &str);
    void replace(string &str, vector<string> &finds, vector<string> &replaces);
};
