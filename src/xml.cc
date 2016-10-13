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
    uint quiebre = 2;
    uint max_width = 19;
    bool is_data = false;
    stringstream ss_data;
    string name;
    string value;
    vector<string> finds;
    vector<string> replaces;
    vector<string> ret;

    if (is_report) {
        quiebre = 5;
        max_width = 13; //21
    }
   
    try {
        xmlpp::DomParser parser;
        parser.parse_memory(*content);
        
        xmlpp::Node *root = parser.get_document()->get_root_node();
        xmlpp::Node::NodeSet node = root->find((*paths)[0]);
    
        for (uint n = 0; n < paths->size(); n++) {
            if (n >= quiebre)
                is_data = true;
            
            node = root->find((*paths)[n]);
            
            for (uint i = 1; i <= node.size(); i++) {
                xmlpp::Element *element = (xmlpp::Element *)node.at(i - 1);
                const xmlpp::Attribute *attribute = element->get_attribute(attr_name);
                
                if (attribute && !is_data) {
                    ret.push_back(attribute->get_value());
                } else {
                    if (is_data) {
                        name = node.at(i - 1)->get_name();
                        
                        if (element->has_child_text())
                            value = element->get_first_child_text()->get_content();
                        else
                            value = "-";
                        
                        if ((value == "NOCVE") || (value == "NOBID") || (value == "NOXREF")) {
                            value = "-";
                        }

                        if (name == "creation_time") {
                            name = "creation time";
                        } else if (name == "modification_time") {
                            name = "modification time";
                        } else if (name == "family_count") {
                            name = "family count";
                        } else if (name == "nvt_count") {
                            name = "nvt count";
                        } else if (name == "alive_tests") {
                            name = "alive tests";
                        } else if (name == "content_type") {
                            name = "content type";
                        } else if (name == "cve") {
                            if (value != "-") {
                                finds.push_back(", ");
                                replaces.push_back("\n");
                                replace(value, finds, replaces);
                                format(value);
                                value.replace((value.size() - 1), 1, " ");
                            }
                        } else if (name == "xref") {
                            name = "REFERENCES";
                            if (value == "-") {
                                value.replace(value.size(), 1, " ");
                            } else {
                                finds.push_back("URL:");
                                finds.push_back(", ");
                                replaces.push_back("");
                                replaces.push_back("\n");
                                replace(value, finds, replaces);
                                format(value);
                                value.replace((value.size() - 1), 1, " ");
                            }
                        } else if ((name == "tags") && (value != "-")) {
                            name = "DETAILS";
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
                            replaces.push_back("VECTOR");
                            replaces.push_back("VULDETECT\n");
                            replaces.push_back("INSIGHT\n");
                            replaces.push_back("TYPE\n");
                            replaces.push_back("IMPACT\n");
                            replaces.push_back("AFFECTED\n");
                            replaces.push_back("SUMMARY\n");
                            replaces.push_back("SOLUTION TYPE\n");
                            replaces.push_back("SOLUTION\n");
                            replaces.push_back("\n\n");
                            replace(value, finds, replaces);
                            value.insert(value.size(), "\n");
                            
                            size_t found = value.find("\n");
                            if (found != string::npos)
                                  value.replace(0, found+1, "");
                            
                            format(value);
                        } else if (name == "description") { 
                            if (is_report) {
                                //name.insert(name.size(), "\n");
                                //value.insert(value.size(), "\n");
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
                if (is_data) {
                    ret.push_back(ss_data.str());
                    ss_data.str(string());
                    ss_data.clear();
                }
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
            ss << setw(13) << setfill(' ') << " " << line << endl;//21
        }
    }
    
    str = ss.str();//.replace((ss.str().size() - 1), 1, "");
}
