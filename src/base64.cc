#include "base64.h"

static const string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(BYTE c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

vector<BYTE> base64_decode(string const &str)
{
    int i = 0;
    int n = 0;
    int in = 0;
    
    size_t size = str.size();
    
    BYTE char_array_3[3];
    BYTE char_array_4[4];
    
    vector<BYTE> ret;

    while (size-- && (str[in] != '=') && is_base64(str[in])) {
        char_array_4[i++] = str[in];
        in++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; i < 3; i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (n = i; n < 4; n++)
          char_array_4[n] = 0;
        for (n = 0; n < 4; n++)
            char_array_4[n] = base64_chars.find(char_array_4[n]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (n = 0; (n < i - 1); n++)
            ret.push_back(char_array_3[n]);
    }

    return ret;
}
