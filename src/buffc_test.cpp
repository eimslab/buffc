#include <iostream>
#include <vector>

#include "packet.h"
#include "sample.h"

using namespace std;
using namespace buffc;

int main()
{
//    RSAKeyPair keyPair = RSA::generateKeyPair(256);
//    RSAKeyInfo rsaKey = RSA::decodeKey(keyPair.privateKey);
//
//    vector<ubyte> result;
//
//    Packet::build(result, 1229, CryptType::NONE, "111111111122222222220000", rsaKey, "abcde", "abcde", 1, "abcde", 2L);
//
//    for (int i = 0; i < result.size(); i++) {
//        cout << (ushort)result[i] << ", ";
//    }
//
//    cout << endl;
//
//    /////////////////////
//
//    ubyte buffer[] = {4, 205, 0, 0, 0, 40, 0, 5, 97, 98, 99, 100, 101, 0, 5, 97, 98, 99, 100, 101, 5, 0, 0, 0, 1, 65, 0, 0, 0, 5, 97, 98, 99, 100, 101, 7, 0, 0, 0, 0, 0, 0, 0, 2, 46, 254};
//    string name, method;
//    vector<Any> result2;
//    Packet::parse(result2, buffer, 46, 1229, CryptType::NONE, "111111111122222222220000", rsaKey, name, method);
//    cout << name << ", " << method << endl;
//
//    cout << result2.size() << endl;
//    cout << result2[0].cast<int>() << endl;
//    cout << result2[1].cast<string>() << endl;
//    cout << result2[2].cast<int64>() << endl;

    //////////////////////

    Sample sample;
    sample.id = 1;
    sample.name = "abcde";
    sample.age = 10;
    vector<ubyte> serialized;
    sample.serialize(serialized);
    for (int i = 0; i < serialized.size(); i++) {
        cout << (ushort)serialized[i] << ", ";
    }
    cout << endl;

    Sample sample2 = Message::deserialize<Sample>(serialized.data(), serialized.size());
    cout << sample2.id << ", " << sample2.name << ", " << sample2.age << endl;

    return 0;
}
