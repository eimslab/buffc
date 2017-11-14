#ifndef _Included_BUFFC_PACKET_H
#define _Included_BUFFC_PACKET_H
#include <iostream>/////////////////
#include <type_traits>

#include <src/utils/typedefine.h>
#include <src/tea/xtea.h>
#include <src/aes.h>
#include <src/rsa.h>

#include "utils/bytes.h"
#include "utils/md5.h"
#include "utils/utility.h"

using namespace std;
using namespace crypt::tea::xtea;
using namespace crypt::rsa;
using namespace crypt::aes;
using namespace buffc::utils;

namespace buffc {

enum CryptType {
    NONE = 0,
    XTEA = 1,
    AES  = 2,
    RSA  = 3
};

template <typename T> ubyte TypeID() {
         if (is_same<typename std::decay<T>::type, char>   ::value)
        return 0x01;
    else if (is_same<typename std::decay<T>::type, ubyte>  ::value)
        return 0x02;
    else if (is_same<typename std::decay<T>::type, short>  ::value)
        return 0x03;
    else if (is_same<typename std::decay<T>::type, ushort> ::value)
        return 0x04;
    else if (is_same<typename std::decay<T>::type, int>    ::value)
        return 0x05;
    else if (is_same<typename std::decay<T>::type, uint>   ::value)
        return 0x06;
    else if (is_same<typename std::decay<T>::type, int64>  ::value)// || (is_same<typename std::decay<T>::type, long>::value))
        return 0x07;
    else if (is_same<typename std::decay<T>::type, uint64> ::value)// || (is_same<typename std::decay<T>::type, unsigned long>::value))
        return 0x08;
    else if (is_same<typename std::decay<T>::type, float>  ::value)
        return 0x20;
    else if (is_same<typename std::decay<T>::type, double> ::value)
        return 0x21;
//    else if (is_same<typename std::decay<T>::type, real> ::value)
//        return 0x22;
    else if (is_same<typename std::decay<T>::type, bool>   ::value)
        return 0x30;
//    else if (is_same<typename std::decay<T>::type, char> ::value)
//        return 0x40;
    else if (is_same<typename std::decay<T>::type, string> ::value)// || is_same<typename std::decay<T>::type, char*> ::value || is_same<typename std::decay<T>::type, const char*>::value)
        return 0x41;
    else {
        assert(0);
    }
}

class BufferBuilder {
private:
    vector<ubyte>* buffer;

public:
    BufferBuilder(vector<ubyte>*);
    void setBuffer(vector<ubyte>*);

    template <typename T>
    uint put(const T& value, bool isWriteTypeInfo, bool isWriteLengthInfo, int lengthBytes) {
        assert(lengthBytes == 0 || lengthBytes == 2 || lengthBytes == 4);

        if (isWriteTypeInfo) {
            Bytes::write<ubyte>(TypeID<T>(), *buffer, -1);
        }

        uint len = sizeof(T);

        if (isWriteLengthInfo && lengthBytes > 0) {
            if (lengthBytes == 2)
                Bytes::write<ushort>((ushort)len, *buffer, -1);
            else
                Bytes::write<uint>(len, *buffer, -1);
        }

        Bytes::write<T>(value, *buffer, -1);

        return len;
    }
};

class Packet {
private:

    template <typename T>
    static void unpacker(BufferBuilder& bb, const T& t) {
        bb.put<T>(t, true, false, 0);
    }

    static void unpacker(BufferBuilder& bb) {
    }

    template <typename T>
    static void unpacker(BufferBuilder& bb, const T*& t) {
        assert(0);
    }

    template <typename T, typename... Params>
    static void unpacker(BufferBuilder& bb, const T& t, Params... params) {
        unpacker(bb, t);
        unpacker(bb, params...);
    }

    template <typename T, typename... Params>
    static void unpacker(BufferBuilder& bb, const T*& t, Params... params) {
        unpacker(bb, t);
        unpacker(bb, params...);
    }

public:
    template <typename... Params>
    static void build(vector<ubyte>& buffer, ushort magic, CryptType crypt, const string& key, const RSAKeyInfo& rsaKey, const string& name, const string& method, Params... params) {
        assert(name.length() <= 255);
        assert(method.length() <= 255);

        vector<ubyte> tlv;
        BufferBuilder bb(&tlv);
        unpacker(bb, params...);

        ubyte* tlv_p = tlv.data();
        uint tlv_len = tlv.size();
        ubyte* en;
        uint en_len;

        if (crypt == CryptType::NONE) {
            en = tlv_p;
            en_len = tlv_len;
        } else if (crypt == CryptType::XTEA) {
            int* xtea_key = (int*)key.c_str();
            crypt::tea::xtea::XTEA xtea(xtea_key, 64);
            en = new ubyte[tlv_len + 12];
            en_len = xtea.encrypt(tlv_p, tlv_len, en);
        } else if (crypt == CryptType::AES) {
            ubyte* aes_key = (ubyte*)key.c_str();
            AES128 aes(aes_key, 24);
            en = new ubyte[tlv_len + 20];
            en_len = aes.encrypt(tlv_p, tlv_len, en);
        } else {    // CryptType::RSA
            en = new ubyte[tlv_len * 2];
            en_len = RSA::encrypt(rsaKey, tlv_p, tlv_len, en);
        }

        buffer.clear();
        bb.setBuffer(&buffer);

        bb.put<ushort>(magic, false, false, 0);
        bb.put<int>(0, false, false, 0);     // length, seize a seat.
        bb.put<string>(name, false, true, 2);
        bb.put<string>(method, false, true, 2);
        Bytes::write<int>(buffer.size() - 2 - 4 + en_len + 2, buffer, 2);

        for (int i = 0; i < en_len; i++) {
            buffer.push_back(en[i]);
        }

        if (crypt != CryptType::NONE) delete[] en;

        string md5 = MD5Utils.GenerateMD5(buffer.data(), buffer.size());
        ubyte md5_buf[2];
        strToByte_hex(md5, md5_buf, 4);

        for (int i = 0; i < 2; i++) {
            buffer.push_back(md5_buf[i]);
        }
    }

    static uint parseInfo(ubyte* buffer, uint len, string& name, string& method);

//    static Variant[] parse(ubyte[] buffer, ushort magic, CryptType crypt, string key, Nullable!RSAKeyInfo rsaKey, out string name, out string method)
//    {
//        assert(buffer != null && buffer.length >= 10, "Incorrect buffer length.");
//
//        ushort t_magic;
//        int t_len;
//        t_magic = buffer.peek!ushort(0);
//        t_len = buffer.peek!int(2);
//
//        if ((t_magic != magic) || (t_len > buffer.length - 6))
//            return null;
//
//        buffer = buffer[0 .. t_len + 6];
//        if (strToByte_hex(MD5(buffer[0 .. $ - 2])[0 .. 4]) != buffer[$ - 2 .. $])
//            return null;
//
//        size_t tlv_pos = parseInfo(buffer, name, method);
//        buffer = buffer[tlv_pos .. $ - 2];
//
//        final switch (crypt)
//        {
//        case CryptType.NONE:
//            break;
//        case CryptType.XTEA:
//            buffer = Xtea.decrypt(buffer, key);
//            break;
//        case CryptType.AES:
//            buffer = AESUtils.decrypt!AES128(buffer, key);
//            break;
//        case CryptType.RSA:
//            buffer = RSA.decrypt(rsaKey, buffer);
//            break;
//        }
//
//        ubyte typeId;
//        int pos;
//        Variant[] ret;
//
//        void get(T)()
//        {
//            ret ~= Variant(buffer.peek!T(pos));
//            pos += T.sizeof;
//        }
//
//        while (pos < buffer.length)
//        {
//            typeId = buffer[pos];
//            pos++;
//
//            if (typeId == TypeID!byte)
//            {
//                get!byte;
//            }
//            else if (typeId == TypeID!ubyte)
//            {
//                get!ubyte;
//            }
//            else if (typeId == TypeID!short)
//            {
//                get!short;
//            }
//            else if (typeId == TypeID!ushort)
//            {
//                get!ushort;
//            }
//            else if (typeId == TypeID!int)
//            {
//                get!int;
//            }
//            else if (typeId == TypeID!uint)
//            {
//                get!uint;
//            }
//            else if (typeId == TypeID!long)
//            {
//                get!long;
//            }
//            else if (typeId == TypeID!ulong)
//            {
//                get!ulong;
//            }
//            else if (typeId == TypeID!float)
//            {
//                get!float;
//            }
//            else if (typeId == TypeID!double)
//            {
//                get!double;
//            }
//            else if (typeId == TypeID!real)
//            {
//                //get!real;
//                ret ~= Variant(ubyteToReal(buffer[pos .. pos + real.sizeof]));
//                pos += real.sizeof;
//            }
//            else if (typeId == TypeID!bool)
//            {
//                get!bool;
//            }
//            else if (typeId == TypeID!char)
//            {
//                get!char;
//            }
//            else if (typeId == TypeID!string)
//            {
//                int temp = buffer.peek!int(pos);
//                pos += 4;
//                ret ~= Variant(cast(string) buffer[pos .. pos + temp]);
//                pos += temp;
//            }
//            else
//            {
//                assert(0, "Data types id that are not supported: " ~ typeId.to!string);
//            }
//        }
//
//        return ret;
//    }
};

}

#endif
