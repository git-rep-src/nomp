#ifndef XML_H
#define XML_H

#include <libxml++/libxml++.h>

class Xml
{
public:
    Xml();
    ~Xml();

    bool create(const std::vector<std::string> *nodes, std::vector<std::string> *values,
                std::vector<std::string> *xret, const bool &is_root = false);
    bool parse(const std::string *content, const std::vector<std::string> *paths,
               std::vector<std::string> *xret, const std::string &attr = "id",
               const bool &get_data = true, const bool &is_report = false);
private:
    void format(const xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                std::vector<std::string> **xret, const std::size_t &i,
                const int &max_width);
    void wrap(std::string &str, const std::size_t &p, const bool replace = false);
    void width(std::string &str, const int &max_width);
    void uppercase(std::string &str);
    void replace(std::string &str, std::vector<std::string> &targets,
                 std::vector<std::string> &replaces);
};

#endif
