#include <iostream>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "xml.h"

using namespace std;

Xml::Xml()
{
}

Xml::~Xml()
{
}

int Xml::parse()
{
    locale::global(locale(""));

    try {
        xmlpp::TextReader reader("example.xml");

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
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
