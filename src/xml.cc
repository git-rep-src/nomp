#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>//

#include <libxml++/libxml++.h>

#include "xml.h"

Xml::Xml()
{
}

Xml::~Xml()
{
}

vector<string> Xml::create(vector<string> *nodes, vector<string> *values)
{
    int n = (nodes->size() - 1);
    vector<string> ret;
    
    try {
        xmlpp::Document doc;
        xmlpp::Element *root = doc.create_root_node((*nodes)[0]);
        
        if ((nodes->size() > 2) && ((*nodes)[0] != "get_reports")) {
            xmlpp::Element *childs[n]; 
            for (int i = 0; i < n; i++) {
                childs[i] = root->add_child_element((*nodes)[i + 1]);
                if ((*values)[i].find("__attr__") != string::npos)
                    childs[i]->set_attribute("id", (*values)[i].erase((*values)[i].size() - 8));
                else
                    childs[i]->add_child_text((*values)[i]);
            }
        } else {
            for (int i = 0; i < n; i++)
                root->set_attribute((*nodes)[i + 1], (*values)[i].erase((*values)[i].size() - 8));
        }

        ret.push_back(doc.write_to_string());
    } catch (const std::exception& ex) {
        ret.clear();
        ret.push_back("__ERROR__");
    }

    return ret;
}

vector<string> Xml::parse(string *content, vector<string> *paths, const string attr_name,
                          bool get_data, bool is_report)
{
    uint nn;
    int index = 1;
    uint max_width = 19;
    bool is_data = false;
    stringstream ss_data;
    string name;
    string value;
    vector<string> finds;
    vector<string> replaces;
    vector<string> ret;

    if (is_report)
        max_width = 21;
   
    try {
        xmlpp::DomParser parser;
        parser.parse_memory(*content);
        
        xmlpp::Node *root = parser.get_document()->get_root_node();
        xmlpp::Node::NodeSet node = root->find((*paths)[0]);
    
        if (get_data)
            nn = ((paths->size() + node.size()));
        else
            nn = paths->size();
    
        for (uint n = 0; n < nn; n++) {
            if (n >= paths->size()) {
                node = root->find((*paths)[0] + "[" + to_string(index) + "]//*[text()]");
                is_data = true;
                ++index;
            } else {
                node = root->find((*paths)[n]);
            }
            for (uint i = 1; i <= node.size(); i++) {
                xmlpp::Element *element = (xmlpp::Element *)node.at(i - 1);
                const xmlpp::Attribute *attribute = element->get_attribute(attr_name);
                if (attribute && !is_data) {
                    ret.push_back(attribute->get_value());
                } else {
                    if (is_data) {
                        name = node.at(i - 1)->get_name();
                        value = element->get_first_child_text()->get_content();
                        if ((name == "xref") && (value != "NOXREF")) {
                            finds.push_back(", ");
                            replaces.push_back("\n");
                            replace(value, finds, replaces);
                            format(value);
                        } else if ((name == "tags") && (value != "")) {
                            finds.push_back("cvss_base_vector=");
                            finds.push_back("vuldetect=");
                            finds.push_back("insight=");
                            finds.push_back("qod_type=");
                            finds.push_back("impact=");
                            finds.push_back("affected=");
                            finds.push_back("summary=");
                            finds.push_back("solution_type=");
                            finds.push_back("solution=");
                            finds.push_back("|");
                            replaces.push_back("CVSS_BASE_VECTOR: ");
                            replaces.push_back("VULDETECT: ");
                            replaces.push_back("INSIGHT: ");
                            replaces.push_back("QOD_TYPE: ");
                            replaces.push_back("IMPACT: ");
                            replaces.push_back("AFFECTED: ");
                            replaces.push_back("SUMMARY: ");
                            replaces.push_back("SOLUTION_TYPE: ");
                            replaces.push_back("SOLUTION: ");
                            replaces.push_back("\n\n");
                            replace(value, finds, replaces);
                            format(value);
                        } else if ((name == "description") && (value != "")) { 
                            if (is_report) {
                                format(value);
                            } else {
                                finds.push_back("\n");
                                replaces.push_back("");
                                replace(value, finds, replaces);
                           }
                        }
                        to_upper(name);
                        ss_data << left << setw(max_width) << setfill(' ') << name << value << endl;
                    } else {
                        ret.push_back(element->get_first_child_text()->get_content() + "\n");
                    }
                }
            }
            if (is_data) {
                ret.push_back(ss_data.str());
                ss_data.str(string());
                ss_data.clear();
            }
        }
    } catch (const std::exception& ex) {
        ret.clear();
        ret.push_back("__ERROR__");
    }
    
    return ret;
}

void Xml::to_upper(string &str)
{
    string upper;
    for (uint i = 0; i < str.size(); i++)
        upper += toupper(str[i]);
    str = upper;
}

void Xml::replace(string &str, vector<string> &finds, vector<string> &replaces)
{
    for (uint n = 0; n < finds.size(); n++) {
        for (string::size_type i = 0; (i = str.find(finds[n], i)) != string::npos;) {
            str.replace(i, finds[n].length(), replaces[n]);
            i += replaces[n].length();
        }
    }
    finds.clear();
    replaces.clear();
}


void Xml::format(string &str)
{
    istringstream f(str);
    stringstream ss;    
    string line;
    bool is_first_line = true; 
    
    while (getline(f, line)) {
        if (is_first_line) {
            ss << line << endl;
            is_first_line = false;
        } else {
            ss << setw(21) << setfill(' ') << " " << line << endl;
        }
    }
    str = ss.str().replace((ss.str().size() - 1), 1, "");
}
