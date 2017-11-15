#include "client.h"

namespace buffc {
namespace rpc {

//template <>
//static int call<int, typename... Params>(string method, Params... params) {
//    assert(Handler != null);        // TcpRequestHandler must be bound.
//    assert(method.length() > 0);    // Paramter method must be set.
//
//    vector<ubyte> request;
//    Message::serialize_without_msginfo(request, method, params...);
//
//    ubyte response[0xFFFF];
//    uint len = Handler(request.data(), request.size(), response);
//
//    vector<Any> res_params;
//    string name;
//    string res_method;
//    Message::deserialize(res_params, response, len, name, res_method);
//
//    //assert(method == res_method);
//
//    return res_params[0].cast<int>();
//}

}
}
