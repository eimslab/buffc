#include <iostream>
#include <vector>

#include "packet.h"

using namespace std;
using namespace buffc;

int main()
{
    RSAKeyPair keyPair = RSA::generateKeyPair(256);
    RSAKeyInfo rsaKey = RSA::decodeKey(keyPair.privateKey);

    vector<ubyte> result;

    Packet::build(result, 1229, CryptType::NONE, "111111111122222222220000", rsaKey, "abcde", "abcde", 1, "abcde", 2L);

    for (int i = 0; i < result.size(); i++) {
        cout << (ushort)result[i] << ", ";
    }

    cout << endl;

    return 0;
}
