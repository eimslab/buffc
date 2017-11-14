#ifndef _Included_BUFFC_BYTES_H
#define _Included_BUFFC_BYTES_H

#include <vector>
#include <cassert>
#include <istream>

#include <src/utils/typedefine.h>

using namespace std;

namespace buffc {
namespace utils {

class Bytes {
public:
    template <typename T>
    static uint write(const T& value, vector<ubyte>& buffer, int offset) {
        if (offset < 0) {
            offset = buffer.size();
        }
        while (buffer.size() < offset + sizeof(T)) {
            buffer.push_back((ubyte)0);
        }

        ubyte* p = (ubyte*)&value;

        uint i = 0;
        while (i < sizeof(T)) {
            buffer[offset + sizeof(T) - i - 1] = p[i];
            i++;
        }

        return sizeof(T);
    }

    template <typename T>
    static T peek(ubyte* buffer, int offset, int count = -1) {
        if (offset < 0) {
            offset = 0;
        }

        T t;
        ubyte* p = (ubyte*)&t;

        uint i = 0;
        while (i < sizeof(T)) {
            p[i] = buffer[offset + sizeof(T) - i - 1];
            i++;
        }

        return t;
    }
};

}
}

#endif
