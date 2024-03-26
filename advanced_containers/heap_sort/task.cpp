#include "task.h"

using namespace std;

vector<int> HeapSort(const vector<int>& v) {
    vector<int> r;
    std::priority_queue<int> q;
    for (int n : v) 
        q.push(n);
    while (!q.empty()) {
        r.push_back(q.top());
        q.pop();
    }
    for (int i = 0, j = r.size()-1; i < j; i++, j--) 
        swap(r[i], r[j]);
    return r;
}
