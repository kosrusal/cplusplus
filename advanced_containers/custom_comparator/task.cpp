#include "task.h"

bool operator <(const SuperKey& lhs, const SuperKey& rhs) {
    if (lhs.StrPart < rhs.StrPart) 
        return true;
    if (lhs.StrPart == rhs.StrPart && lhs.IntPart < rhs.IntPart)
        return true;
    if (lhs.StrPart == rhs.StrPart && lhs.IntPart == rhs.IntPart && lhs.FloatPart < rhs.FloatPart)
        return true;

    return false;
}

void PopulateMap(
    std::map<SuperKey, std::string>& map,
    const std::vector<std::pair<SuperKey, std::string>>& toAdd
) {
    map.insert(toAdd.begin(), toAdd.end());
}
