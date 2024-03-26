#include "task.h"

bool is_palindrome(const std::string& s) {
    long unsigned int i, j, f = 0;
    if (s == "")
        return 1;
    for (i = 0, j = s.size()-1; i < j; i++, j--) {
        while (i < s.size() && (s[i] == ' ' || s[i] == '?'))
            i++;
        while (j > 0 && (s[j] == ' ' || s[j] == '?'))
            j--;
        if (i > j && f == 0) {
            f = 1;
            return 1;
        }

        char a = s[i]; char b = s[j];

        if (a > 64 && a < 91)
            a += 32;
        if (b > 64 && b < 91)
            b += 32;

        if (a != b)
            return 0;
    }
    return 1;
}
