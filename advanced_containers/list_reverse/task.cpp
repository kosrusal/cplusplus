#include "task.h"


void ReverseList(std::list<int>& l) {
    std::list<int> k;
    while (!l.empty()) {
        k.push_back(l.back());
        l.pop_back();
    }
    l.clear();
    for (auto c : k) {
        l.push_back(c);
    }
}
