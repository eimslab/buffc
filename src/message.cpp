#include "message.h"

namespace buffc {

void Message::settings(ushort magic, CryptType crypt, string key) {
    assert(crypt == CryptType::NONE || (crypt != CryptType::NONE && !key.empty())); // Must specify key when specifying the type of CryptType.

    _message_magic = magic;
    _message_crypt = crypt;
    _message_key   = key;

    if (crypt == CryptType::RSA) {
        _message_rsaKey = RSA::decodeKey(key);
    }
}

void Message::settings(ushort magic, RSAKeyInfo rsaKey) {
    _message_magic = magic;
    _message_crypt = CryptType::RSA;
    _message_rsaKey = rsaKey;
}

void Message::getMessageInfo(ubyte* buffer, uint len, string& name, string& method) {
    Packet::parseInfo(buffer, len, name, method);
}

void Message::deserialize(vector<Any>& result, ubyte* buffer, uint len, string& name, string& method) {
    Packet::parse(result, buffer, len, _message_magic, _message_crypt, _message_key, _message_rsaKey, name, method);
}

}
