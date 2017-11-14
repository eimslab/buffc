#include "packet.h"

namespace buffc {

BufferBuilder::BufferBuilder(vector<ubyte>* buffer) {
    this->buffer = buffer;
}

void BufferBuilder::setBuffer(vector<ubyte>* buffer) {
    this->buffer = buffer;
}

template <>
uint BufferBuilder::put<string>(const string& value, bool isWriteTypeInfo, bool isWriteLengthInfo, int lengthBytes) {
    assert(lengthBytes == 0 || lengthBytes == 2 || lengthBytes == 4);

    if (isWriteTypeInfo) {
        buffer->push_back(TypeID<string>());
    }

    uint len = value.length();
    if (isWriteLengthInfo && lengthBytes > 0) {
        if (lengthBytes == 2)
            Bytes::write<ushort>((ushort)len, *buffer, -1);
        else
            Bytes::write<uint>(len, *buffer, -1);
    }

    Bytes::write<string>(value, *buffer, -1);

    return len;
}

template <>
uint BufferBuilder::put<long>(const long& value, bool isWriteTypeInfo, bool isWriteLengthInfo, int lengthBytes) {
    assert(lengthBytes == 0 || lengthBytes == 2 || lengthBytes == 4);

    if (isWriteTypeInfo) {
        Bytes::write<ubyte>(TypeID<int64>(), *buffer, -1);
    }

    uint len = sizeof(int64);

    if (isWriteLengthInfo && lengthBytes > 0) {
        if (lengthBytes == 2)
            Bytes::write<ushort>((ushort)len, *buffer, -1);
        else
            Bytes::write<uint>(len, *buffer, -1);
    }

    Bytes::write<int64>((int64)value, *buffer, -1);

    return len;
}

template <>
uint BufferBuilder::put<unsigned long>(const unsigned long& value, bool isWriteTypeInfo, bool isWriteLengthInfo, int lengthBytes) {
    assert(lengthBytes == 0 || lengthBytes == 2 || lengthBytes == 4);

    if (isWriteTypeInfo) {
        Bytes::write<ubyte>(TypeID<uint64>(), *buffer, -1);
    }

    uint len = sizeof(uint64);

    if (isWriteLengthInfo && lengthBytes > 0) {
        if (lengthBytes == 2)
            Bytes::write<ushort>((ushort)len, *buffer, -1);
        else
            Bytes::write<uint>(len, *buffer, -1);
    }

    Bytes::write<uint64>((uint64)value, *buffer, -1);

    return len;
}

template <>
void Packet::unpacker<string>(BufferBuilder& bb, const string& t) {
    bb.put<string>(t, true, true, 4);
}

template <>
void Packet::unpacker<char>(BufferBuilder& bb, const char*& t) {
    string s(t);
    bb.put<string>(s, true, true, 4);
}

template <>
void Packet::unpacker<const char>(BufferBuilder& bb, const char*& t) {
    string s(t);
    bb.put<string>(s, true, true, 4);
}

}
