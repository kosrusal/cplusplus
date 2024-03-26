#include <list>
#include <unordered_map>

using namespace std;

template <typename K, typename V>
class LruCache {
private:
    list<K> item_;
    unordered_map<K, pair<V, typename list<K>::iterator>> item_map;
    size_t max_size_;

public:
    LruCache(size_t max_size) : max_size_(max_size) {}

    void Put(const K& key, const V& value) {
        auto pos = item_map.find(key);
        if (pos == item_map.end()) {
            item_.push_front(key);
            item_map.insert(make_pair(key, make_pair(value, item_.begin())));
            if (item_map.size() > max_size_) {
                item_map.erase(item_.back());
                item_.pop_back();
            }
        }
        else {
            item_.erase(pos->second.second);
            item_.push_front(key) ;
            item_map[key] = { value, item_.begin() };
        }
    }

    bool Get(const K& key, V* value) {
        auto pos = item_map.find(key);
        if (pos == item_map.end())
            return false;
        item_.erase(pos->second.second);
        item_.push_front(key);
        item_map[key] = { pos->second.first, item_.begin() };
        *value = (pos->second.first);
        return true;

    }

};
