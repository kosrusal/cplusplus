#pragma once

namespace NPointers {
    int Increment(int *a);
    int Multiply(int a, int b, bool* f);
    int ScalarProduct(const int a[],const int b[], int n);
    int SizeOfMaximumSquareOfCrosses(const char* a, int y, int x);
    long long* MultiplyToLongLong(int a, int b);
    
}

namespace NReferences {
    int MultiplyInplace (int& a, int b);
    int CompareArraysByAverage (const int a[], int n, int b[]);
    
}
