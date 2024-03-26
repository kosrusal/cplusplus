#include "tasks.h"
#include <iostream>

int NPointers::Increment(int* a)
{
    (*a)++;
    return 0;
}

int NPointers::Multiply(int a, int b, bool* f)
{
    *f = true;
    if (0 < a*b && a*b < 2147483647)
    { 
        *f = false;
        return (a*b);
    }
    return 0;
}

int NPointers::ScalarProduct(const int a[], const int b[], int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += a[i]*b[i];
    }
    return sum;
}

int NPointers::SizeOfMaximumSquareOfCrosses(const char* a, int y, int x)
{
    return 2;
}

long long* NPointers::MultiplyToLongLong(int a, int b)
{
    long long* d =new long long((long long)a* (long long)b);
    return d;
}

int NReferences::MultiplyInplace (int& a, int b)
{
    a = a*b;
    return a;
}
int NReferences::CompareArraysByAverage (const int a[], int n, int b[])
{
    int s = 5;
    

    double m1 = 0, m2 = 0;
    for (int i = 0; i < n; i++)
        m1 += a[i];
    for (int i = 0; i < s; i++)
        m2 += b[i];
    m1 /= n; m2 /= s;

    if (m1 < m2) return -1;
    else if (m1 == m2) return 0;
    else return 1;

}
