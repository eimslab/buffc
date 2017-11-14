#include "bytes.h"

namespace buffc {
namespace utils {

template <>
uint Bytes::write<string>(const string& value, vector<ubyte>& buffer, int offset) {
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
string Bytes::peek<string>(vector<ubyte>& buffer, int offset, int count) {
    if (offset < 0) {
        offset = buffer.size();
    }
    assert((count >= 0) && (buffer.size() - offset >= count));

    ubyte* p = new ubyte[count + 1];

    uint i = 0;
    while (i < count) {
        p[i] = buffer[offset + i];
        i++;
    }

    p[i] = 0x00;
    return string((char*)p, 0, count + 1);
}

template <>
uint Bytes::write<long>(const long& value, vector<ubyte>& buffer, int offset) {
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
long Bytes::peek<long>(vector<ubyte>& buffer, int offset, int count) {
    if (offset < 0) {
        offset = buffer.size();
    }
    assert(buffer.size() - offset >= sizeof(int64));

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
uint Bytes::write<unsigned long>(const unsigned long& value, vector<ubyte>& buffer, int offset) {
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
unsigned long Bytes::peek<unsigned long>(vector<ubyte>& buffer, int offset, int count) {
    if (offset < 0) {
        offset = buffer.size();
    }
    assert(buffer.size() - offset >= sizeof(uint64));

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
