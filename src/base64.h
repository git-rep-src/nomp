#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef unsigned char BYTE;

vector<BYTE> base64_decode(string const &);

#endif
