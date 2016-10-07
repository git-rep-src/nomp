#include <stdexcept>

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

vector<string> Xml::parse(string *content, vector<string> *paths, const string attr_name, bool get_data)
{
    uint nn;
    int index = 1;
    bool is_data = false;
    string buf_data;
    string buf_name;
    string buf_name_upper;
    vector<string> ret;
   
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
                        buf_name = node.at(i - 1)->get_name();
                        for (uint i = 0; i < buf_name.size(); i++)
                            buf_name_upper += toupper(buf_name[i]); 
                        buf_data.append(". " + buf_name_upper + ":  " + element->get_first_child_text()->get_content() + "\n");
                        buf_name.clear();
                        buf_name_upper.clear();
                    } else {
                        ret.push_back(element->get_first_child_text()->get_content() + "\n");
                    }
                }
            }
            if (is_data) {
                ret.push_back(buf_data);
                buf_data.clear();
            }
        }
    } catch (const std::exception& ex) {
        ret.clear();
        ret.push_back("__ERROR__");
    }
    
    return ret;
}
