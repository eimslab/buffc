#include "message.h"

namespace buffer {

void Message::settings(ushort magic, CryptType crypt, string key) {
    assert(crypt == CryptType::NONE || (crypt != CryptType::NONE && !key.empty())); // Must specify key when specifying the type of CryptType.

    _magic = magic;
    _crypt = crypt;
    _key   = key;

    if (crypt == CryptType::RSA) {
        _rsaKey = RSA::decodeKey(key);
    }
}

void Message::settings(ushort magic, RSAKeyInfo rsaKey) {
    _magic = magic;
    _crypt = CryptType::RSA;
    _rsaKey = rsaKey;
}

void Message::getMessageInfo(ubyte* buffer, uint len, string& name, string& method) {
    Packet::parseInfo(buffer, len, name, method);
}

void Message::deserialize(vector<Any>& result, ubyte* buffer, uint len, string& name, string& method) {
    Packet::parse(result, buffer, len, _magic, _crypt, _key, _rsaKey, name, method);
}

}
