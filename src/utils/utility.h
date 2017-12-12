#pragma once

#include <stdlib.h>
#include <iostream>

#include "types.h"

using namespace std;

uint strToByte_hex(const string&, ubyte*, uint = -1);
string byteToStr_hex(ubyte*, uint);
