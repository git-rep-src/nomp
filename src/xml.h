#ifndef XML_H
#define XML_H

#include <libxml++/libxml++.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

class Xml
{
public:
    Xml();
    ~Xml();

    bool create(const vector<string> *nodes, vector<string> *values,
                vector<string> *xret, const bool &is_root = false);
    bool parse(const string *content, const vector<string> *paths,
               vector<string> *xret, const string &attr = "id",
               const bool &get_data = true, const bool &is_report = false);

private:
    void format(const xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                vector<string> **xret, const size_t &i, const int &max_width);
    void wrap(string &str, const size_t &p, const bool replace = false);
    void width(string &str, const int &max_width);
    void uppercase(string &str);
    void replace(string &str, vector<string> &targets, vector<string> &replaces);
};

#endif
