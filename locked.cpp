#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

using namespace std;

string DNA_sequence;
unordered_map<string, int> kmer;
unordered_map<string, pthread_spinlock_t> kmerlocks;

int k = 8;

void *thread_call(void * ids)
{
    int base = *((int*)ids);
    int prev, ret;

    for(int i=base; i<=DNA_sequence.length() - k; i+=k)
    {
        string seq=DNA_sequence.substr(i,k);
        int ret = pthread_spin_trylock(&kmerlocks[seq]);
        prev = kmer[seq];
        while (ret != 0)
        {
            prev = kmer[seq];
            ret = pthread_spin_trylock(&kmerlocks[seq]); //__sync_bool_compare_and_swap(&kmer[seq], prev, prev + 1);
        }
        kmer[seq]+=1;
        pthread_spin_unlock(&kmerlocks[seq]);  
    }
}


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
    int totalkmer=0;

    for (int i = 0; i <= MaxLen; i ++)
    {
        kmer[DNA_sequence.substr(i, k)] = 0;
        pthread_spin_init(&kmerlocks[DNA_sequence.substr(i, k)], 1);
    }


    
    //time(&start);
    pthread_t threads[k];
    int Arguments[k];
    for(int i = 0; i < k; i++)
        Arguments[i] = i;


    for(int i=0; i<k ; i++)
        pthread_create(&threads[i], NULL, thread_call,(void *)&Arguments[i]);
    
    for(int i=0; i<k; i++)
        pthread_join(threads[i], NULL);

    time(&end);





    int size = 0;
    for(auto it=kmer.begin();it!=kmer.end();it++)
    {
        totalkmer+=(*it).second;
        if((*it).second != 0)
            size++;
    }
    cout<<(double)(end-start)<<","<<size<<","<<totalkmer<<endl;
}
