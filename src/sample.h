#include <vector>

#include "utils/any.h"
#include "message.h"

using namespace std;
using namespace buffc;

class Sample : Message {
public:
    static string _className;

    int32   id;
    string  name;
    int32   age;

    void setValue(vector<Any>& params) {
        id      = params[0].cast<int32>();
        name    = params[1].cast<string>();
        age     = params[2].cast<int32>();
    }

    void serialize(vector<ubyte>& buffer, string method = "") {
        Message::_serialize(buffer, "Sample", method, id, name, age);
    }
};
string Sample::_className = "Sample";
