#include "client.h"

namespace buffc {
namespace rpc {

template <>
char Client::HandleCallResult<char>(vector<Any>& res_params) {
    return res_params[0].cast<char>();
}

template <>
unsigned char Client::HandleCallResult<unsigned char>(vector<Any>& res_params) {
    return res_params[0].cast<unsigned char>();
}

template <>
short Client::HandleCallResult<short>(vector<Any>& res_params) {
    return res_params[0].cast<short>();
}

template <>
ushort Client::HandleCallResult<ushort>(vector<Any>& res_params) {
    return res_params[0].cast<ushort>();
}

template <>
int Client::HandleCallResult<int>(vector<Any>& res_params) {
    return res_params[0].cast<int>();
}

template <>
unsigned int Client::HandleCallResult<unsigned int>(vector<Any>& res_params) {
    return res_params[0].cast<unsigned int>();
}

template <>
long Client::HandleCallResult<long>(vector<Any>& res_params) {
    return res_params[0].cast<long>();
}

template <>
unsigned long Client::HandleCallResult<unsigned long>(vector<Any>& res_params) {
    return res_params[0].cast<unsigned long>();
}

template <>
long long Client::HandleCallResult<long long>(vector<Any>& res_params) {
    return res_params[0].cast<long long>();
}

template <>
unsigned long long Client::HandleCallResult<unsigned long long>(vector<Any>& res_params) {
    return res_params[0].cast<unsigned long long>();
}

template <>
float Client::HandleCallResult<float>(vector<Any>& res_params) {
    return res_params[0].cast<float>();
}

template <>
double Client::HandleCallResult<double>(vector<Any>& res_params) {
    return res_params[0].cast<double>();
}

template <>
long double Client::HandleCallResult<long double>(vector<Any>& res_params) {
    return res_params[0].cast<long double>();
}

template <>
bool Client::HandleCallResult<bool>(vector<Any>& res_params) {
    return res_params[0].cast<bool>();
}

template <>
string Client::HandleCallResult<string>(vector<Any>& res_params) {
    return res_params[0].cast<string>();
}

}
}
