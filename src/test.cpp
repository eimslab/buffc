//#include <iostream>
//#include <vector>
//
//#include "packet.h"
//#include "sample.h"
//#include "rpc/client.h"
//
//using namespace std;
//using namespace buffer;
//using namespace buffer::rpc;
//
//void tcpRequestHandler(vector<ubyte>& send_buffer, vector<ubyte>& receive_buffer)
//{
//    for (int i = 0 ; i < send_buffer.size(); i++) {
//        receive_buffer.push_back(send_buffer[i]);
//    }
//}
//
//int main()
//{
////    RSAKeyPair keyPair = RSA::generateKeyPair(256);
////    RSAKeyInfo rsaKey = RSA::decodeKey(keyPair.privateKey);
//
//    Sample sample;
//    sample.id = 1;
//    sample.name = "abcde";
//    sample.age = 10;
//
//    vector<ubyte> serialized;
//    sample.serialize(serialized);
//
//    Sample sample2 = Message::deserialize<Sample>(serialized.data(), serialized.size());
//    cout << sample2.id << ", " << sample2.name << ", " << sample2.age << endl;
//
//    //////////////////////
//
//    Client::bindTcpRequestHandler(&tcpRequestHandler);
//    int r = Client::call<int>("Login", 101, "abcde");
//    cout << r << endl;
//
//    Sample sample3 = Client::call<Sample>("Login", 1, "abcde", 10);
//    cout << sample3.id << ", " << sample3.name << ", " << sample3.age << endl;
//
//    return 0;
//}
