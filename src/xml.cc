#include "xml.h"

#include <form.h>

#include <sstream>
#include <iomanip>
#include <stdexcept>

using std::stringstream;
using std::istringstream;

Xml::Xml()
{
}

Xml::~Xml()
{
}

bool Xml::create(const vector<string> *nodes, vector<string> *values,
                 vector<string> *xret, bool only_root)
{
    size_t n = (nodes->size() - 1);

    try {
        xmlpp::Document doc;
        xmlpp::Element *root = doc.create_root_node((*nodes)[0]);

        if ((nodes->size() > 2) && !only_root) {
            xmlpp::Element *childs[n]; 
            for (size_t i = 0; i < n; i++) {
                if ((*nodes)[0] == "authenticate") {
                    if (i == 0)
                        childs[0] = root->add_child_element((*nodes)[i + 1]);
                    else
                        childs[i] = childs[0]->add_child_element((*nodes)[i + 1]);
                } else {
                    childs[i] = root->add_child_element((*nodes)[i + 1]);
                }
                if ((*values)[i].find("__attr__") != string::npos)
                    childs[i]->set_attribute("id", (*values)[i].erase((*values)[i].size() - 8));
                else
                    childs[i]->add_child_text((*values)[i]);
            }
        } else {
            for (size_t i = 0; i < n; i++)
                root->set_attribute((*nodes)[i + 1], (*values)[i].erase((*values)[i].size() - 8));
        }
        
        xret->push_back(doc.write_to_string());
    } catch (...) {
        return false;
    }

    return true;
}

bool Xml::parse(const string *content, const vector<string> *paths, vector<string> *xret,
                const string &attr, bool get_details, bool is_report)
{   
    size_t offset;
    int max_width;
    string value;

    if (is_report) {
        offset = 5;
        max_width = 14;
    } else {    
        offset = 2;
        max_width = 20;
    }
    
    try {
        xmlpp::DomParser parser;
        parser.parse_memory(*content);
        xmlpp::Node *root = parser.get_document()->get_root_node();
        
        xmlpp::Node::NodeSet node;
        xmlpp::Element *element;
        xmlpp::Attribute *attribute;

        for (size_t n = 0; n < paths->size(); n++) {
            node = root->find((*paths)[n]);
            if (node.size() == 0) {
                break;
            } else {
                for (size_t i = 1; i <= node.size(); i++) {
                    element = (xmlpp::Element *)node.at(i - 1);
                    if (n == 0) {
                        attribute = element->get_attribute("status_text");
                        if ((attribute->get_value()).find("OK") == string::npos)
                            return false;
                        else
                            break;
                    } else {
                        attribute = element->get_attribute(attr);
                    }
                    if (get_details) {
                        if (attribute && ((n - 1) < offset)) {
                            xret->push_back(attribute->get_value());
                        } else {
                            if ((n - 1) >= offset) {
                                set_format(&node, &element, &xret, i, max_width);
                            } else {
                                value = element->get_first_child_text()->get_content();
                                if (node.at(i - 1)->get_path() ==
                                    "/get_reports_response/report/report/results/result[" +
                                    std::to_string(i) + "]/name") {
                                    set_wrap(value, (COLS - 55), true);
                                } else if (node.at(i - 1)->get_path() ==
                                           "/get_reports_response/report/report/results/result[" +
                                           std::to_string(i) + "]/host") {
                                    set_wrap(value, 16, true);
                                }
                                xret->push_back(value);
                            }
                        }
                    } else {
                        if (attribute) 
                            xret->push_back(attribute->get_value());
                        if (element->has_child_text())
                            xret->push_back(element->get_first_child_text()->get_content());
                        else
                            xret->push_back("");
                    }
                }
            }
        }
    } catch (...) {
        return false;
    }
    
    return true;
}

void Xml::set_format(const xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                     vector<string> **xret, size_t i, int max_width)
{
    string name;
    string value = "-";
    stringstream ss;
    vector<string> targets;
    vector<string> replaces;

    if ((node->at(i - 1)->get_path() ==
        "/get_tasks_response/task/scanner/name") ||
        (node->at(i - 1)->get_path() ==
         "/get_tasks_response/task[" + std::to_string(i) + "]/scanner/name") ||
        (node->at(i - 1)->get_path() ==
         "/get_targets_response/target/port_list/name" ) ||
        (node->at(i - 1)->get_path() ==
         "/get_targets_response/target[" + std::to_string(i) + "]/port_list/name")) {
        name = "PORTS";
    } else if ((node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/config/name") ||
               (node->at(i - 1)->get_path() ==
               "/get_tasks_response/task[" + std::to_string(i) + "]/config/name")) {
        name = "SCAN";
    } else if ((node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/target/name") ||
               (node->at(i - 1)->get_path() ==
               "/get_tasks_response/task[" + std::to_string(i) + "]/target/name")) {
        name = "TARGET";
    } else if ((node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/first_report/report/timestamp") ||
               (node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/last_report/report/timestamp")) {
        name = "SCAN START";
    } else {
        name = node->at(i - 1)->get_name();
    }

    if ((*element)->has_child_text() &&
        ((*element)->get_first_child_text()->get_content() != "NOCVE") &&
        ((*element)->get_first_child_text()->get_content() != "NOBID") &&
        ((*element)->get_first_child_text()->get_content() != "NOXREF"))
        value = (*element)->get_first_child_text()->get_content();

    targets.push_back("_");
    replaces.push_back(" ");
    replace(name, targets, replaces);
   
    targets.push_back("  ");
    replaces.push_back("");
    replace(value, targets, replaces);
    
    if ((name == "comment") && (value != "-")) {
        set_wrap(value, 50);
    } else if (name == "cve") {
        targets.push_back(", ");
        replaces.push_back("\n");
        replace(value, targets, replaces);
    } else if (name == "xref") {
        name = "REFERENCES";
        targets.push_back("URL:");
        targets.push_back(", ");
        replaces.push_back("");
        replaces.push_back("\n");
        replace(value, targets, replaces);
    } else if ((name == "tags") && (value != "-")) {
        name = "DETAILS";
        size_t n = value.find("|");
        if (n != string::npos)
            value.replace(0, (n + 1), "");
        targets.push_back("vuldetect=");
        targets.push_back("insight=");
        targets.push_back("qod_type=");
        targets.push_back("impact=");
        targets.push_back("affected=");
        targets.push_back("summary=");
        targets.push_back("solution_type=");
        targets.push_back("solution=");
        targets.push_back("|");
        replaces.push_back("VULDETECT\n");
        replaces.push_back("INSIGHT\n");
        replaces.push_back("TYPE\n");
        replaces.push_back("IMPACT\n");
        replaces.push_back("AFFECTED\n");
        replaces.push_back("SUMMARY\n");
        replaces.push_back("SOLUTION TYPE\n");
        replaces.push_back("SOLUTION\n");
        replaces.push_back("\n\n");
        replace(value, targets, replaces);
        set_wrap(value, (COLS / 1.4));
    } else if ((name == "description") && (value != "-")) {
        if (max_width == 14)
            set_wrap(value, (COLS / 1.4));
        else
            set_wrap(value, 50);
    }

    set_width(value, max_width);
    to_uppercase(name);
    
    ss << std::left << std::setw(max_width) << std::setfill(' ') << name << value << std::endl << std::endl;
    
    (*xret)->push_back(ss.str());
}

void Xml::set_wrap(string &str, size_t p, bool replace)
{
    size_t n = 1;
    string line;
    stringstream ss;    
    istringstream f(str);
    
    while (getline(f, line)) {
        if (line.size() >= p) {
            if (replace) {
                line.replace((p - 3), (line.size() - (p - 3)), "...");
            } else {
                for (size_t i = 0; i < line.size(); i++) {
                    if (i > (p * n)) {
                        line.insert((p * n), "\n");
                        ++n;
                    }
                }
                n = 1;
            }
        }
        ss << line << std::endl;
    }
    
    str = ss.str();
}

inline void Xml::set_width(string &str, int max_width)
{
    bool is_first = true; 
    string line;
    stringstream ss;
    istringstream f(str);
    
    while (getline(f, line)) {
        if (is_first) {
            ss << line << std::endl;
            is_first = false;
        } else {
            ss << std::setw(max_width) << std::setfill(' ') << " " << line << std::endl;
        }
    }
    
    str = ss.str().replace((ss.str().size() - 1), 1, "");
}

inline void Xml::to_uppercase(string &str)
{
    string uppercase;

    for (size_t i = 0; i < str.size(); i++)
        uppercase += toupper(str[i]);
    
    str = uppercase;
}

inline void Xml::replace(string &str, vector<string> &targets, vector<string> &replaces)
{
    for (size_t n = 0; n < targets.size(); n++) {
        for (size_t i = 0; (i = str.find(targets[n], i)) != string::npos;) {
            str.replace(i, targets[n].length(), replaces[n]);
            i += replaces[n].length();
        }
    }
    
    vector<string>().swap(targets);
    vector<string>().swap(replaces);
}
