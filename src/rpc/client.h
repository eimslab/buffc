#ifndef _Included_BUFFC_RPC_CLIENT_H
#define _Included_BUFFC_RPC_CLIENT_H

#include "../message.h"

namespace buffc {
namespace rpc {

typedef uint (*TcpRequestHandler)(ubyte* send_buffer, uint send_len, ubyte* receive_buffer);
static TcpRequestHandler Handler = null;

class Client {

public:

    static void bindTcpRequestHandler(TcpRequestHandler handler) {
        Handler = handler;
    }

    template <typename T, typename... Params>
    static T call(string method, Params... params) {
        assert(Handler != null);        // TcpRequestHandler must be bound.
        assert(method.length() > 0);    // Paramter method must be set.

        vector<ubyte> request;
        Message::serialize_without_msginfo(request, method, params...);

        ubyte response[0xFFFF];
        uint len = Handler(request.data(), request.size(), response);

        string name;
        string res_method;
        vector<Any> res_params;
        Message::deserialize(res_params, response, len, name, res_method);

        //assert(method == res_method);

        return HandleCallResult<T>(res_params);
    }

    template <typename T>
    static T HandleCallResult(vector<Any>& res_params) {
        T message;
        message.setValue(res_params);

        return message;
    }
};

}
}

#endif
