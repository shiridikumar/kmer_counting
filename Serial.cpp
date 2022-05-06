#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

using namespace std;

string DNA_sequence;
map<string, int> kmer;
int k = 4;


int main(int argc, char **argv)
{
    ifstream input(argv[1]);
    if (!input.good())
    {
        std::cerr << "Error opening: " << argv[1];
        return -1;
    }

    string line;
    while (std::getline(input, line))
    {
        if (line.empty() || line[0] == '>')
            continue;
        else
            DNA_sequence += line;
    }





    time_t start, end;
    time(&start);

    int MaxLen = DNA_sequence.length() - k;

    #pragma omp parallel for
    for (int i = 0; i <= MaxLen; i ++)
        kmer[DNA_sequence.substr(i, k)] = 0;





    cout<<"Serial access without parallelizing"<<endl;
    for (int i = 0; i <= MaxLen; i ++)
    {
        kmer[DNA_sequence.substr(i, k)] += 1;
    }
    time(&end);






    int totalkmer=0;
    int size = 0;
    for(auto it=kmer.begin();it!=kmer.end();it++){
        totalkmer+=(*it).second;
        if((*it).second != 0)
            size++;
    }
    cout<<(double)(end-start)<<","<<size<<","<<totalkmer<<endl;
}
