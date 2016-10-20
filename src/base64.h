#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

typedef unsigned char BYTE;

std::vector<BYTE> base64_decode(std::string const&);

#endif
