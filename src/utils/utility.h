#ifndef _Included_BUFFC_UTILITY_H
#define _Included_BUFFC_UTILITY_H

#include <stdlib.h>
#include <iostream>

#include <src/utils/typedefine.h>

using namespace std;

uint strToByte_hex(const string&, ubyte*, uint = -1);
string byteToStr_hex(ubyte*, uint);

#endif
