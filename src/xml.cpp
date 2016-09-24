#include <iostream>

#include <libxml++/libxml++.h>
#include "xml.h"

using namespace xmlpp;

Xml::Xml()
{
}

Xml::~Xml()
{
}

vector<string> Xml::parse(string *str_xml, vector<string> *paths)
{
    DomParser parser;
    vector<string> ret;

    parser.parse_memory(*str_xml);
    Node *root = parser.get_document()->get_root_node();

    NodeSet node = root->find((*paths)[0]);

    for (uint i = 0; i < node.size(); i++) {
        Element *element = (Element *)node.at(i);
        ret.push_back(element->get_child_text()->get_content());
    }

    return ret;
}

/*
#include <libxml++/parsers/textreader.h>

    locale::global(locale(""));

    try {
        xmlpp::TextReader reader((const unsigned char *)ret.c_str(), ret.size());

        while (reader.read()) {
            cout << "--- node ---" << endl;
            cout << "name: " << reader.get_name() << endl;
            cout << "depth: " << reader.get_depth() << endl;

            if (reader.has_attributes()) {
                cout << "attributes: " << endl;
                reader.move_to_first_attribute();
                do {
                    cout << "  " << reader.get_name() << ": " << reader.get_value() << endl;
                } while (reader.move_to_next_attribute());
                reader.move_to_element();
            } else {
                cout << "no attributes" << endl;
            }

            if (reader.has_value())
                cout << "value: '" << reader.get_value() << "'" << endl;
            else
                cout << "novalue" << endl;
        }
    } catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << endl;
        return -1;
    }

    return 0;
*/
