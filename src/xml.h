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
                vector<string> *xret, bool only_root = false);
    bool parse(const string *content, const vector<string> *paths,
               vector<string> *xret, const string &attr = "id",
               bool get_details = true, bool is_report = false);

private:
    void set_format(const xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                    vector<string> **xret, size_t i, int max_width);
    void set_wrap(string &str, size_t p, bool replace = false);
    void set_width(string &str, int max_width);
    void to_uppercase(string &str);
    void replace(string &str, vector<string> &targets, vector<string> &replaces);
};

#endif
