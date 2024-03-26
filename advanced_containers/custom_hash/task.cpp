#include "task.h"

using namespace std;

void PopulateHashMap(
    unordered_map<SuperKey, string>& hashMap,
    const vector<pair<SuperKey, string>>& toAdd
) {
    // for (auto& c : toAdd) {
    //     hashMap[c.first] = c.second;
    // }
    for (size_t i = 0; i < toAdd.size(); i++) {
        hashMap.insert({toAdd[i].first, toAdd[i].second});
    }
}



