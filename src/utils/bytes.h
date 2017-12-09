#pragma once

#include <vector>
#include <cassert>
#include <istream>

#include "types.h"

using namespace std;

namespace buffer {
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

template <>
inline uint Bytes::write<string>(const string& value, vector<ubyte>& buffer, int offset) {
    if (offset < 0) {
        offset = buffer.size();
    }
    while (buffer.size() < offset + value.length()) {
        buffer.push_back((ubyte)0);
    }

    ubyte* p = (ubyte*)value.c_str();

    uint i = 0;
    while (i < value.length()) {
        buffer[offset + i] = p[i];
        i++;
    }

    return value.length();
}

template <>
inline string Bytes::peek<string>(ubyte* buffer, int offset, int count) {
    if (offset < 0) {
        offset = 0;
    }

    ubyte* p = new ubyte[count];

    int i = 0;
    while (i < count) {
        p[i] = buffer[offset + i];
        i++;
    }

    return string((char*)p, count);
}

template <>
inline uint Bytes::write<long>(const long& value, vector<ubyte>& buffer, int offset) {
    int64 v = value;
    if (offset < 0) {
        offset = buffer.size();
    }
    while (buffer.size() < offset + sizeof(int64)) {
        buffer.push_back((ubyte)0);
    }

    ubyte* p = (ubyte*)&v;

    uint i = 0;
    while (i < sizeof(int64)) {
        buffer[offset + sizeof(int64) - i - 1] = p[i];
        i++;
    }

    return sizeof(int64);
}

template <>
inline long Bytes::peek<long>(ubyte* buffer, int offset, int count) {
    if (offset < 0) {
        offset = 0;
    }

    int64 t;
    ubyte* p = (ubyte*)&t;

    uint i = 0;
    while (i < sizeof(int64)) {
        p[i] = buffer[offset + sizeof(int64) - i - 1];
        i++;
    }

    return (long)t;
}

template <>
inline uint Bytes::write<unsigned long>(const unsigned long& value, vector<ubyte>& buffer, int offset) {
    uint64 v = value;
    if (offset < 0) {
        offset = buffer.size();
    }
    while (buffer.size() < offset + sizeof(uint64)) {
        buffer.push_back((ubyte)0);
    }

    ubyte* p = (ubyte*)&v;

    uint i = 0;
    while (i < sizeof(uint64)) {
        buffer[offset + sizeof(uint64) - i - 1] = p[i];
        i++;
    }

    return sizeof(uint64);
}

template <>
inline unsigned long Bytes::peek<unsigned long>(ubyte* buffer, int offset, int count) {
    if (offset < 0) {
        offset = 0;
    }

    uint64 t;
    ubyte* p = (ubyte*)&t;

    uint i = 0;
    while (i < sizeof(uint64)) {
        p[i] = buffer[offset + sizeof(uint64) - i - 1];
        i++;
    }

    return (unsigned long)t;
}

}
}
