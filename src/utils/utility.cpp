#include <stdlib.h>

#include "utility.h"

size_t strToByte_hex(const string& input, ubyte* result, size_t max_len)
{
    size_t len = ((max_len == 0) ? input.length() : max_len) / 2;
    char* p = (char*)input.c_str();

    for (size_t i = 0 ; i < len; i++)
    {
        char t[3];
        t[0] = p[i * 2];
        t[1] = p[i * 2 + 1];
        t[2] = 0;
        uint b;
        sscanf(t, "%X", &b);
        result[i] = (ubyte)b;
    }

    return len;
}

string byteToStr_hex(ubyte* input, size_t len)
{
    char* t = new char[len * 2];

    for (size_t i = 0; i < len; i++)
    {
        sprintf(t + i * 2, "%02X", input[i]);
    }

    string ret(t, 0, len * 2);
    delete[] t;

    return ret;
}
