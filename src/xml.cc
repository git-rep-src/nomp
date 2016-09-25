#include <stdexcept>
#include <libxml++/libxml++.h>
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
    int n = values->size();
    vector<string> ret;

    try {
        xmlpp::Document doc;
        xmlpp::Element *root = doc.create_root_node((*nodes)[0]);
        xmlpp::Element *childs[n]; 
        
        for (int i = 0; i < n; i++) {
            childs[i] = root->add_child_element((*nodes)[i + 1]);
            childs[i]->add_child_text((*values)[i]);
        }

        ret.push_back(doc.write_to_string());
    } catch(const std::exception& ex) {
        ret.push_back("ERROR: ");
    }

    return ret;
}

vector<string> Xml::parse(string *str, vector<string> *paths)
{
    DomParser parser;
    vector<string> ret;

    parser.parse_memory(*str);
    Node *root = parser.get_document()->get_root_node();

    xmlpp::Node::NodeSet node = root->find((*paths)[0]);

    for (uint i = 0; i < node.size(); i++) {
        Element *element = (Element *)node.at(i);
        ret.push_back(element->get_first_child_text()->get_content());
    }

    return ret;
}
