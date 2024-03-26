#include "tasks.h"

#include <cmath>

int NOuter::NInner::DoSomething(int lhs, int rhs) {
    return lhs + rhs;
}

int NOuter::DoSomething(int lhs, int rhs) {
    return lhs-rhs;
}

int NOverload::ProcessTwoArgs(int lhs, int rhs) {
    return lhs+rhs;
}

char NOverload::ProcessTwoArgs(char lhs, char rhs) {
    if (lhs > rhs) return lhs;
    return rhs;
}

int NOverload::ProcessTwoArgs(int lhs, char rhs) {
    return lhs-(int)rhs;
}

unsigned int NOverflow::WithOverflow(int lhs, int rhs) {
    return lhs+rhs;
}

uint64_t NOverflow::WithOverflow(int64_t lhs, int64_t rhs) {
    return lhs-rhs;
}

int NLoop::SumInRange(const int lhs, const int rhs) {
    int64_t sum = 0;
    for (int i = lhs; i < rhs; i++)
        sum += i;
    return sum;
}

int NLoop::CountFixedBitsInRange(const int from, const int to, const int bitsCnt) {
    int a, c = 0, ans = 0;
    for (int i = from; i < to; i++)
    {
        c = 0; a = i;
        while (a > 0)
        {
            if (a%2 == 1)
                c++;
            a /= 2;
        }
        if (c == bitsCnt) ans++;
    }
    return ans;
}

double NMath::ComputeMathFormula(const double arg) {
    return fabs(((std::sin(arg))/2 + std::cos(arg)) * ((std::sin(arg))/2 + std::cos(arg)) + std::tan(arg) * std::atan(arg));
}

bool NMath::IsPositive(int arg) {
    if (arg > 0) return 1;
    return 0;
}

int NRecursion::CalculateFibonacci(const int arg) {
    int l = 3;
    int a = 0, b = 1, c;
    for (int i = l; i <= arg+1; i++)
    {
        c = a+b;
        a = b;
        b = c;
    }
    return c;
}
