#include <iomanip>
#include <sstream>
#include <stdexcept>

//#include <iostream>//

#include "xml.h"

Xml::Xml()
{
}

Xml::~Xml()
{
}

bool Xml::create(std::vector<std::string> *nodes, std::vector<std::string> *values,
                 std::vector<std::string> *xret, bool is_root)
{
    int n = (nodes->size() - 1);
    
    try {
        xmlpp::Document doc;
        xmlpp::Element *root = doc.create_root_node((*nodes)[0]);
        
        if ((nodes->size() > 2) && !is_root) {
            xmlpp::Element *childs[n]; 
            for (int i = 0; i < n; i++) {
                childs[i] = root->add_child_element((*nodes)[i + 1]);
                if ((*values)[i].find("__attr__") != std::string::npos)
                    childs[i]->set_attribute("id", (*values)[i].erase((*values)[i].size() - 8));
                else
                    childs[i]->add_child_text((*values)[i]);
            }
        } else {
            for (int i = 0; i < n; i++)
                root->set_attribute((*nodes)[i + 1], (*values)[i].erase((*values)[i].size() - 8));
        }

        xret->push_back(doc.write_to_string());
    } catch (...) {
        return false;
    }

    return true;
}

bool Xml::parse(std::string *content, std::vector<std::string> *paths,
                std::vector<std::string> *xret, const std::string attr,
                bool get_data, bool is_report)
{   
    uint offset;
    uint max_width;
    std::string value;

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
    
        for (uint n = 0; n < paths->size(); n++) {
            node = root->find((*paths)[n]);
            for (uint i = 1; i <= node.size(); i++) {
                xmlpp::Element *element = (xmlpp::Element *)node.at(i - 1);
                const xmlpp::Attribute *attribute = element->get_attribute(attr);
                if (get_data) {
                    if (attribute && (n < offset)) {
                        xret->push_back(attribute->get_value());
                    } else {
                        if (n >= offset) {
                            set_format(&node, &element, &xret, i, max_width);
                        } else {
                            value = element->get_first_child_text()->get_content();
                            if (node.at(i - 1)->get_path() ==
                                "/get_reports_response/report/report/results/result[" + std::to_string(i) + "]/name")
                                set_wrap(value, 120, true);
                            else if (node.at(i - 1)->get_path() ==
                                     "/get_reports_response/report/report/results/result[" + std::to_string(i) + "]/host")
                                set_wrap(value, 15, true);
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

void Xml::set_format(xmlpp::Node::NodeSet *node, xmlpp::Element **element,
                     std::vector<std::string> **xret, uint &i, uint &max_width)
{
    std::string name;
    std::string value = "-";
    std::stringstream ss;
    std::vector<std::string> targets;
    std::vector<std::string> replaces;

    if ((node->at(i - 1)->get_path() == "/get_tasks_response/task[" + std::to_string(i) + "]/scanner/name") ||
        (node->at(i - 1)->get_path() == "/get_targets_response/target[" + std::to_string(i) + "]/port_list/name")) {
        name = "PORTS";
    } else if (node->at(i - 1)->get_path() == "/get_tasks_response/task[" + std::to_string(i) + "]/config/name") {
        name = "SCAN";
    } else if (node->at(i - 1)->get_path() == "/get_tasks_response/task[" + std::to_string(i) + "]/target/name") {
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
        set_wrap(value, 43); // TODO: ENVIAR COLS EN LUGAR DE 40.
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
        if (n != std::string::npos)
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
        set_wrap(value, 100); // TODO: ENVIAR COLS EN LUGAR DE 155.
    } else if ((name == "description") && (value != "-")) {
        if (max_width == 13)
            set_wrap(value, 100); // TODO: ENVIAR COLS EN LUGAR DE 155.
        else
            set_wrap(value, 43); // TODO: ENVIAR COLS EN LUGAR DE 40.
    }

    set_width(value, max_width);
    set_uppercase(name);
    
    ss << std::left << std::setw(max_width) << std::setfill(' ') << name << value << std::endl;
    
    (*xret)->push_back(ss.str());
}

void Xml::set_wrap(std::string &str, uint p, bool replace)
{
    std::istringstream f(str);
    std::stringstream ss;    
    std::string line;
    uint n = 1;
    
    while (getline(f, line)) {
        if (line.size() >= p) {
            if (replace) {
                line.replace((p - 3), (line.size() - (p - 3)), "...");
            } else {
                for (uint i = 0; i < line.size(); i++) {
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

void Xml::set_width(std::string &str, uint &max_width)
{
    std::istringstream f(str);
    std::stringstream ss;
    std::string line;
    bool is_first = true; 
    
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

void Xml::set_uppercase(std::string &str)
{
    std::string uppercase;

    for (uint i = 0; i < str.size(); i++)
        uppercase += toupper(str[i]);
    
    str = uppercase;
}

void Xml::replace(std::string &str, std::vector<std::string> &targets,
                  std::vector<std::string> &replaces)
{
    for (uint n = 0; n < targets.size(); n++) {
        for (std::string::size_type i = 0; (i = str.find(targets[n], i)) != std::string::npos;) {
            str.replace(i, targets[n].length(), replaces[n]);
            i += replaces[n].length();
        }
    }
    
    targets.clear();
    replaces.clear();
}
