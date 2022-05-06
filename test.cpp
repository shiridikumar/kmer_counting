#include <bits/stdc++.h>

using namespace std;


int main()
{
    map<string, int> A;
    A["ABC"] = 5;
    printf("%p\n",&A["ABC"]);
    printf("%d\n", *(&A["ABC"]));
}