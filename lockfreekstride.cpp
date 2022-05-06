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
int k = 8;
int Init_N = 8;

void *thread_call(void * ids)
{
    int base = *((int*)ids);
    int prev, ret;

    for(int i=base; i<=DNA_sequence.length() - k; i+=k)
    {
        string seq=DNA_sequence.substr(i,k);
        prev=kmer[seq];
        
        ret= __sync_val_compare_and_swap(&kmer[seq], prev, prev+1);
        while(ret != prev)
        {
            prev=ret;
            ret= __sync_val_compare_and_swap(&kmer[seq], prev, prev+1);
        }
    }
}


void *Init_hash(void* id)
{
    int I, start, end;
    I = *((int*)id);

    start = I * (DNA_sequence.length() / Init_N);
    if(I == Init_N - 1)
        end = DNA_sequence.length() - k;
    else
        end = (I + 1) * (DNA_sequence.length() / Init_N);

    for(int i = start; i < end; i++)
        kmer[DNA_sequence.substr(i, k)] = 0;
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
    // cout<<"parlllele"<<endl;


    
    pthread_t threads_in[Init_N];
    int Arguments_in[Init_N];
    for(int i = 0; i < Init_N; i++)
        Arguments_in[i] = i;


    for(int i=0; i< Init_N ; i++)
        pthread_create(&threads_in[i], NULL, Init_hash,(void *)&Arguments_in[i]);
    
    for(int i=0; i< Init_N; i++)
        pthread_join(threads_in[i], NULL);
        
        
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
