#include <iostream>
#include <string>

#include "ui.h"
#include "str.h"
#include "xml.h"
#include "exec.h"

using namespace std;

class Nomp
{
public:
    Nomp();
    ~Nomp();

private:
    Ui ui;
    Xml xml;
    Exec exec;
    
    string ret;
    
    void driver();
};
