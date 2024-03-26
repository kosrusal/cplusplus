#include <vector>
#include <memory>
#include <string>
using namespace std;

vector<shared_ptr<string>> DeDuplicate(vector<unique_ptr<string>>& in) {
    vector<shared_ptr<string>> out;

    for (size_t c = 0; c < in.size(); ++c) {
        if (out.size() == 0)
            out.push_back(make_shared<string>(*(in[c])));
        else  {
            int f = 1;
            for (size_t i = 0; i < out.size(); i++) {
                if (*(out[i]) == *(in[c])) {
                    out.push_back(out[i]);
                    f = 0;
                    break;
                }
            }
            if (f)
                out.push_back(make_shared<string>(*(in[c])));
        }
    }
    return out;
}

vector<unique_ptr<string>> Duplicate(const vector<shared_ptr<string>>& in) {
    vector<unique_ptr<string>> out;
    for (size_t c = 0; c < in.size(); ++c) {
        out.push_back(make_unique<string>(*(in[c])));
    }
    return out;
}