#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

using namespace std;

string DNA_sequence;
int k = 8;
map<string, int> kmer;

int main(int argc, char **argv)
{
    ifstream input(argv[1]);
    if (!input.good())
    {
        std::cerr << "Error opening: " << argv[1];
        return -1;
    }
    string line, id;
    while (std::getline(input, line))
    {
        if (line.empty())
            continue;

        if (line[0] != '>')
        {
            DNA_sequence += line;
        }
    }
    time_t start, end, mid;
    time(&start);

    // **************************************** Without multithreading ***************************************************
    cout<<"Serial access without parallelizing"<<endl;
    for (int i = 0; i <= DNA_sequence.length()-k; i ++)
    {
        // cout<<DNA_sequence.substr(i,k)<<endl;
        kmer[DNA_sequence.substr(i, k)] += 1;
    }
    time(&mid);
    int totalkmer = 0;

    for(auto it=kmer.begin();it!=kmer.end();it++){
        totalkmer+=(*it).second;
    }
    cout<<(double)(mid-start)<<","<<kmer.size()<<","<<totalkmer<<endl;
}