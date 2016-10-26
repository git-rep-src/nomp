#include "xml.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>
//#include <iostream>//

using std::stringstream;
using std::istringstream;
using std::to_string;
using std::setfill;
using std::setw;
using std::left;
using std::endl;

Xml::Xml()
{
}

Xml::~Xml()
{
}

bool Xml::create(const vector<string> *nodes, vector<string> *values,
                 vector<string> *xret, const bool &is_root)
{
    size_t n = (nodes->size() - 1);
    
    try {
        xmlpp::Document doc;
        xmlpp::Element *root = doc.create_root_node((*nodes)[0]);
        
        if ((nodes->size() > 2) && !is_root) {
            xmlpp::Element *childs[n]; 
            for (size_t i = 0; i < n; i++) {
                childs[i] = root->add_child_element((*nodes)[i + 1]);
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
                const string &attr, const bool &get_data, const bool &is_report)
{   
    size_t offset;
    int max_width;
    string value;

    if (is_report) {
        offset = 5;
        max_width = 13;
    } else {    
        offset = 2;
        max_width = 19;
    }

    try {
        xmlpp::DomParser parser;
        parser.parse_memory(*content);
        
        xmlpp::Node *root = parser.get_document()->get_root_node();
        xmlpp::Node::NodeSet node = root->find((*paths)[0]);
    
        for (size_t n = 0; n < paths->size(); n++) {
            node = root->find((*paths)[n]);
            for (size_t i = 1; i <= node.size(); i++) {
                xmlpp::Element *element = (xmlpp::Element *)node.at(i - 1);
                const xmlpp::Attribute *attribute = element->get_attribute(attr);
                if (get_data) {
                    if (attribute && (n < offset)) {
                        xret->push_back(attribute->get_value());
                    } else {
                        if (n >= offset) {
                            format(&node, &element, &xret, i, max_width);
                        } else {
                            value = element->get_first_child_text()->get_content();
                            if (node.at(i - 1)->get_path() ==
                                "/get_reports_response/report/report/results/result[" +
                                 to_string(i) + "]/name")
                                wrap(value, 120, true);
                            else if (node.at(i - 1)->get_path() ==
                                     "/get_reports_response/report/report/results/result[" +
                                      to_string(i) + "]/host")
                                wrap(value, 15, true);
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
    } catch (...) {
        return false;
    }
    
    return true;
}

void Xml::format(const xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                 vector<string> **xret, const size_t &i, const int &max_width)
{
    string name;
    string value = "-";
    stringstream ss;
    vector<string> targets;
    vector<string> replaces;

    if ((node->at(i - 1)->get_path() ==
        "/get_tasks_response/task/scanner/name") ||
        (node->at(i - 1)->get_path() ==
         "/get_tasks_response/task[" + to_string(i) + "]/scanner/name") ||
        (node->at(i - 1)->get_path() ==
         "/get_targets_response/target/port_list/name" ) ||
        (node->at(i - 1)->get_path() ==
         "/get_targets_response/target[" + to_string(i) + "]/port_list/name")) {
        name = "PORTS";
    } else if ((node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/config/name") ||
               (node->at(i - 1)->get_path() ==
               "/get_tasks_response/task[" + to_string(i) + "]/config/name")) {
        name = "SCAN";
    } else if ((node->at(i - 1)->get_path() ==
               "/get_tasks_response/task/target/name") ||
               (node->at(i - 1)->get_path() ==
               "/get_tasks_response/task[" + to_string(i) + "]/target/name")) {
        name = "TARGET";
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
        wrap(value, 43); // TODO: ENVIAR COLS EN LUGAR DE 40.
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
        value.insert(value.size(), "\n\n");
        wrap(value, 100); // TODO: ENVIAR COLS EN LUGAR DE 155.
    } else if ((name == "description") && (value != "-")) {
        if (max_width == 13)
            wrap(value, 100); // TODO: ENVIAR COLS EN LUGAR DE 155.
        else
            wrap(value, 43); // TODO: ENVIAR COLS EN LUGAR DE 40.
    }

    width(value, max_width);
    uppercase(name);
    
    ss << left << setw(max_width) << setfill(' ') << name << value << endl;
    
    (*xret)->push_back(ss.str());
}

void Xml::wrap(string &str, const size_t &p, const bool replace)
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
        ss << line << endl;
    }
    
    str = ss.str();
}

inline void Xml::width(string &str, const int &max_width)
{
    bool is_first = true; 
    string line;
    stringstream ss;
    istringstream f(str);
    
    while (getline(f, line)) {
        if (is_first) {
            ss << line << endl;
            is_first = false;
        } else {
            ss << setw(max_width) << setfill(' ') << " " << line << endl;
        }
    }
    
    str = ss.str().replace((ss.str().size() - 1), 1, "");
}

inline void Xml::uppercase(string &str)
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
