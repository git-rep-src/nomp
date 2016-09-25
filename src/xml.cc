#include <stdexcept>
#include <libxml++/libxml++.h>
#include <libxml++/attribute.h>
#include <iostream>//

#include "xml.h"

using namespace xmlpp;

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
        xmlpp::Element *childs[n]; 
        
        for (int i = 0; i < n; i++) {
            childs[i] = root->add_child_element((*nodes)[i + 1]);
            if ((*values)[i].find("_attr") != string::npos)
                childs[i]->set_attribute("id", (*values)[i].erase((*values)[i].size() - 5));
            else
                childs[i]->add_child_text((*values)[i]);
        }

        ret.push_back(doc.write_to_string());
    } catch(const std::exception& ex) {
        ret.push_back("ERROR");
    }

    return ret;
}

vector<string> Xml::parse(string *xml, vector<string> *paths, const string attr_name)
{
    DomParser parser;
    vector<string> ret;

    parser.parse_memory(*xml);
    Node *root = parser.get_document()->get_root_node();

    for (uint n = 0; n < paths->size(); n++) {
        xmlpp::Node::NodeSet node = root->find((*paths)[n]);
        for (uint i = 0; i < node.size(); i++) {
            Element *element = (Element *)node.at(i);
            const Attribute *attribute = element->get_attribute(attr_name);
            if (attribute)
                ret.push_back(attribute->get_value());
            else
                ret.push_back(element->get_first_child_text()->get_content());
        }
    }

    return ret;
}
