#include <stdexcept>
#include <iomanip>//
#include <iostream>//
#include <sstream>//

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
    vector<string> v_find;
    vector<string> v_replace;
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
                            v_find.push_back(", ");
                            v_replace.push_back("\n");
                            replace(value, v_find, v_replace);
                            wrap(value);
                        } else if ((name == "tags") && (value != "")) {
                            v_find.push_back("cvss_base_vector=");
                            v_find.push_back("vuldetect=");
                            v_find.push_back("insight=");
                            v_find.push_back("qod_type=");
                            v_find.push_back("impact=");
                            v_find.push_back("affected=");
                            v_find.push_back("summary=");
                            v_find.push_back("solution_type=");
                            v_find.push_back("solution=");
                            v_find.push_back("|");
                            v_replace.push_back("+ CVSS_BASE_VECTOR: ");
                            v_replace.push_back("+ VULDETECT: ");
                            v_replace.push_back("+ INSIGHT: ");
                            v_replace.push_back("+ QOD_TYPE: ");
                            v_replace.push_back("+ IMPACT: ");
                            v_replace.push_back("+ AFFECTED: ");
                            v_replace.push_back("+ SUMMARY: ");
                            v_replace.push_back("+ SOLUTION_TYPE: ");
                            v_replace.push_back("+ SOLUTION: ");
                            v_replace.push_back("\n");
                            replace(value, v_find, v_replace);
                            wrap(value);
                        } else if ((name == "description") && (value != "")) { 
                            if (is_report) {
                                wrap(value);
                            } else {
                                v_find.push_back("\n");
                                v_replace.push_back("");
                                replace(value, v_find, v_replace);
                           }
                        }
                        to_upper(name, is_report);
                        ss_data << left << setw(max_width) << setfill(' ') << name << value << endl << endl;
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

void Xml::to_upper(string &source, bool is_report)
{
    string upper;
    for (uint i = 0; i < source.size(); i++)
        upper += toupper(source[i]);
    source = upper;
    if (is_report)
        source.insert(0, "  ");
}

void Xml::replace(string &source, vector<string> &find, vector<string> &replace)
{
    for (uint n = 0; n < find.size(); n++) {
        for (string::size_type i = 0; (i = source.find(find[n], i)) != string::npos;) {
            source.replace(i, find[n].length(), replace[n]);
            i += replace[n].length();
        }
    }
    find.clear();
    replace.clear();
}


void Xml::wrap(string &source)
{
    istringstream f(source);
    stringstream ss;    
    string line;
    int n = 0; // FIX; HACERLO BIEN. 
    while (getline(f, line)) {
        if (n > 0)
            ss << left << setw(21) << setfill(' ') << " " << line << endl;
        else
            ss << left << line << endl;
        ++n;
    }
    source = ss.str();
}
