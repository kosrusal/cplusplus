#include <unordered_map>
#include <vector>
#include <string>

struct SuperKey {
    std::string StrPart;
    int IntPart;
    float FloatPart;

    bool operator==(const SuperKey& rhs) const {
        return this->StrPart == rhs.StrPart && this->IntPart == rhs.IntPart && this->FloatPart == rhs.FloatPart;
    }
};

template <>
struct std::hash<SuperKey> {
    size_t operator ()(const SuperKey& s) const {
        std::size_t h1 = std::hash<std::string>{}(s.StrPart) ;
        std::size_t h2 = std::hash<int>{}(s.IntPart);
        std::size_t h3 = std::hash<float>{}(s.FloatPart);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};


/*
 * Напишите хэш-функцию, реализовав специализацию шаблона std::hash для типа SuperKey
 * Напишите функцию PopulateHashMap, которая добавляет в hashMap пары ключ-значение из данного вектора toAdd
 */
void PopulateHashMap(
    std::unordered_map<SuperKey, std::string>& hashMap,
    const std::vector<std::pair<SuperKey, std::string>>& toAdd
);

