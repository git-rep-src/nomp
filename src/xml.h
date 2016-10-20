#ifndef XML_H
#define XML_H

#include <libxml++/libxml++.h>

class Xml
{
public:
    Xml();
    ~Xml();

    bool create(std::vector<std::string> *nodes, std::vector<std::string> *values,
                std::vector<std::string> *xret, bool is_root = false);
    bool parse(std::string *content, std::vector<std::string> *paths,
               std::vector<std::string> *xret, const std::string attr = "id",
               bool get_data = true, bool is_report = false);
private:
    void set_format(xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                    std::vector<std::string> **xret, uint &i, uint &max_width);
    void set_wrap(std::string &str, uint p, bool replace = false);
    void set_width(std::string &str, uint &max_width);
    void set_uppercase(std::string &str);
    void replace(std::string &str, std::vector<std::string> &finds,
                 std::vector<std::string> &replaces);
};

#endif
