#include <string>

using namespace std;

class Encryption {
    public:
        string encrypt(string msg, string key) {
            string tmp(key);
            while (key.size() < msg.size())
                key += tmp;

            for (string::size_type i = 0; i < msg.size(); ++i)
                msg[i] ^= key[i];
            return msg;
        }

        string decrypt(string msg, string key) {
            return encrypt(msg, key);
        }
};