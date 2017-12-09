#include "utility.h"

uint strToByte_hex(const string& input, ubyte* result, uint max_len) {
    uint len = (max_len <= 0 ? input.length() : max_len) / 2;
    char* p = (char*)input.c_str();

    for (uint i = 0 ; i < len; i++) {
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

string byteToStr_hex(ubyte* input, uint len) {
    char t[len * 2];

    for (uint i = 0; i < len; i++) {
        sprintf(t + i * 2, "%02X", input[i]);
    }

    return string(t, 0, len * 2);
}
