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

uint Packet::parseInfo(ubyte* buffer, uint len, string& name, string& method) {
    assert(len >= 10);

    ushort len1 = Bytes::peek<ushort>(buffer, 6);
    if (len1 > 0) {
        assert(len >= 8 + len1);
        name = Bytes::peek<string>(buffer, 8, len1);
    }

    ushort len2 = Bytes::peek<ushort>(buffer, 8 + len1);
    if (len2 > 0) {
        assert(len >= 10 + len1 +len2);
        method = Bytes::peek<string>(buffer, 10 + len1, len2);
    }

    return 10 + len1 + len2;
}

void Packet::parse(vector<Any>& result, ubyte* buffer, uint len, ushort magic, CryptType crypt, const string& key, const RSAKeyInfo& rsaKey, string& name, string& method) {
    assert(len >= 10);

    ushort t_magic;
    int t_len;
    t_magic = Bytes::peek<ushort>(buffer, 0);
    t_len = Bytes::peek<int>(buffer, 2);

    if ((t_magic != magic) || (t_len > len - 6)) {
        cout << "magic or len error: " << t_magic << ", " << t_len << endl;
        throw;
    }

    string md5 = MD5Utils.GenerateMD5(buffer, len -  2);
    ubyte md5_buf[2];
    strToByte_hex(md5, md5_buf, 4);

    for (int i = 0; i < 2; i++) {
        if (buffer[len - 2 + i] != md5_buf[i]) {
            cout << "MD5 valid error." << endl;
            throw;
        }
    }

    uint tlv_pos = parseInfo(buffer, len, name, method);

    ubyte* de;
    uint de_len;

    if (crypt == CryptType::NONE) {
        de = buffer + tlv_pos;
        de_len = len - tlv_pos - 2;
    } else if (crypt == CryptType::XTEA) {
        int* xtea_key = (int*)key.c_str();
        crypt::tea::xtea::XTEA xtea(xtea_key, 64);
        de = new ubyte[len - tlv_pos - 2];
        de_len = xtea.decrypt(buffer + tlv_pos, len - tlv_pos - 2, de);
    } else if (crypt == CryptType::AES) {
        ubyte* aes_key = (ubyte*)key.c_str();
        AES128 aes(aes_key, 24);
        de = new ubyte[len - tlv_pos - 2];
        de_len = aes.decrypt(buffer + tlv_pos, len - tlv_pos - 2, de);
    } else {    // CryptType::RSA
        de = new ubyte[(len - tlv_pos - 2) * 2];
        de_len = RSA::decrypt(rsaKey, buffer + tlv_pos, len - tlv_pos - 2, de);
    }

    result.clear();
    uint pos = 0;
    while (pos < de_len) {
        ubyte typeId = de[pos];
        pos++;
        Any any;

               if (typeId == TypeID<char>()) {
            any = Bytes::peek<char>(de, pos);
            pos += sizeof(char);
        } else if (typeId == TypeID<ubyte>() || typeId == 0x40) {
            any = Bytes::peek<ubyte>(de, pos);
            pos += sizeof(ubyte);
        } else if (typeId == TypeID<short>()) {
            any = Bytes::peek<short>(de, pos);
            pos += sizeof(short);
        } else if (typeId == TypeID<ushort>()) {
            any = Bytes::peek<ushort>(de, pos);
            pos += sizeof(ushort);
        } else if (typeId == TypeID<int>()) {
            any = Bytes::peek<int>(de, pos);
            pos += sizeof(int);
        } else if (typeId == TypeID<uint>()) {
            any = Bytes::peek<uint>(de, pos);
            pos += sizeof(uint);
        } else if (typeId == TypeID<int64>()) {
            any = Bytes::peek<int64>(de, pos);
            pos += sizeof(int64);
        } else if (typeId == TypeID<uint64>()) {
            any = Bytes::peek<uint64>(de, pos);
            pos += sizeof(uint64);
        } else if (typeId == TypeID<float>()) {
            any = Bytes::peek<float>(de, pos);
            pos += sizeof(float);
        } else if (typeId == TypeID<double>()) {
            any = Bytes::peek<double>(de, pos);
            pos += sizeof(double);
        } else if (typeId == TypeID<bool>()) {
            any = Bytes::peek<bool>(de, pos);
            pos += sizeof(bool);
        } else if (typeId == TypeID<string>()) {
            int temp = Bytes::peek<int>(de, pos);
            pos += 4;
            any = Bytes::peek<string>(de, pos, temp);
            pos += temp;
        } else {
            assert(0); // Data types id that are not supported: typeId
        }

        result.push_back(any);
    }

    if (crypt != CryptType::NONE) delete[] de;
}

}
