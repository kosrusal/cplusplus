#include "task.h"

char* concat(const char* lhs, const char* rhs) {
    int slhs = 0, srhs = 0;
    while (*(lhs+slhs) != '\0') {
        slhs++;
    }
    while (*(rhs+srhs) != '\0') {
        srhs++;
    }
    char* s = new char[slhs + srhs+1];
    int i = 0;
    for (i = 0; i < slhs; ++i) {
        s[i] = *lhs;
        lhs++;
    }
    for (i = 0; i < srhs; i++)  {
        s[i+slhs] = *rhs;
        rhs++;
    }
    s[i+slhs] = '\0' ;
    return s;
}