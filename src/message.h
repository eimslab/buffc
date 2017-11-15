#ifndef _Included_BUFFC_MESSAGE_H
#define _Included_BUFFC_MESSAGE_H

#include "packet.h"

namespace buffc {

static ushort       _message_magic;
static CryptType    _message_crypt;
static string       _message_key;
static RSAKeyInfo   _message_rsaKey(BigInt(0), BigInt(0));

class Message {

protected:
    typedef char    int8;
    typedef ubyte   uint8;
    typedef short   int16;
    typedef ushort  uint16;
    typedef int     int32;
    typedef uint    uint32;
    //int64
    //uint64
    typedef float   float32;
    typedef double  float64;
    //real
    //bool
    //char
    //string

public:

    static void settings(ushort magic, CryptType crypt = CryptType::NONE, string key = "");
    static void settings(ushort magic, RSAKeyInfo rsaKey);

    template <typename... Params>
    static void serialize_without_msginfo(vector<ubyte>& buffer, string method, Params... params) {
        Packet::build(buffer, _message_magic, _message_crypt, _message_key, _message_rsaKey, "", method, params...);
    }

    static void getMessageInfo(ubyte* buffer, uint len, string& name, string& method);
    static void deserialize(vector<Any>& result, ubyte* buffer, uint len, string& name, string& method);

    template <class T>
    static T deserialize(ubyte* buffer, uint len) {
        string method;

        return deserialize<T>(buffer, len, method);
    }

    template <class T>
    static T deserialize(ubyte* buffer, uint len, string& method) {
        string name;
        vector<Any> params;
        deserialize(params, buffer, len, name, method);

        if (name.empty() || params.size() == 0) {
            cout << "Invalid message buffer." << endl;
            throw;
        }

        T message;
        if (message._className != name) {
            cout << "The type " << name << " of the incoming template is incorrect." << endl;
            throw;
        }

        message.setValue(params);

        return message;
    }

protected:

    template <typename... Params>
    void _serialize(vector<ubyte>& buffer, string name, string& method, Params... params) {
        Packet::build(buffer, _message_magic, _message_crypt, _message_key, _message_rsaKey, name, method, params...);
    }
};

}

#endif
