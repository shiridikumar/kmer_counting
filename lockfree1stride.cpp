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

void *thread_call(void * id)
{
    int I, start, end, prev, ret;
    I = *((int*)id);

    start = I * (DNA_sequence.length() / Num_Threads);
    if(I == Num_Threads - 1)
        end = DNA_sequence.length() - k + 1;
    else
        end = (I + 1) * (DNA_sequence.length() / Num_Threads);


    for(int i=start; i<end ; i++)
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

    for (int i = 0; i <= MaxLen; i ++)
        kmer[DNA_sequence.substr(i, k)] = 0;




    
    //time(&start);
    pthread_t threads[Num_Threads];
    int Arguments[Num_Threads];
    for(int i = 0; i <Num_Threads; i++)
        Arguments[i] = i;


    for(int i=0; i<Num_Threads ; i++)
        pthread_create(&threads[i], NULL, thread_call,(void *)&Arguments[i]);
    
    for(int i=0; i<Num_Threads; i++)
        pthread_join(threads[i], NULL);

    time(&end);





    int size = 0;
    int totalkmer = 0;
    for(auto it=kmer.begin();it!=kmer.end();it++)
    {
        totalkmer+=(*it).second;
        if((*it).second != 0)
            size++;
    }
    cout<<(double)(end-start)<<","<<size<<","<<totalkmer<<endl;
}
