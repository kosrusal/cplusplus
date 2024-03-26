#include <string>
#include <iterator>

using namespace std;

string normalize(string_view path) {
    string ans = "";
    
    for (size_t i = 0; i < path.size(); i++) {
        ans.push_back(path[i]);
        if (i < path.size()-2 && path[i] == '/' && path[i+1] == '.' && path[i+2] =='.') {
            ssize_t uk = ans.size()-2;
            while (uk >= 0) {
                if (ans[uk] == '/') {
                    ans.erase(uk+1, ans.size()-uk);
                    break;
                }
                uk--;
            }
            uk = ans.size()-1;
            if (uk != 2 || ans[uk-1] != '.' || ans[uk-2] != '.')
                i += 2;
        }
        if (i <= path.size()-2 && path[i] == '/' && path[i+1] == '.' && path[i+2] != '.') {
            i += 1;
            continue;
        }
    }
    while (ans.find("//") != string::npos) {
        ans.replace(ans.find("//"), 2, "/");
    }
    if (ans[ans.size()-1] == '/' && ans.size() != 1)
        ans.erase(ans.size()-1, 1);

    return ans;
}