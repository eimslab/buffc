#pragma once

#include <iostream>

#include "types.h"

using namespace std;

size_t strToByte_hex(const string&, ubyte*, size_t = 0);
string byteToStr_hex(ubyte*, size_t);
