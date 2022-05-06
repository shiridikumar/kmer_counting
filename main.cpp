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
void *thread_call(void * ids){
    int base = *((int*)ids);
    int prev;
    uint32_t ret;
    for(int i=base;i<=DNA_sequence.length()-k;i+=k){
        string seq=DNA_sequence.substr(i,k);
        prev=kmer[seq];
        
        ret=__sync_val_compare_and_swap (&kmer[seq], prev, prev+1);
        while(ret != prev)
        {
            prev=ret;
            ret=__sync_val_compare_and_swap (&kmer[seq], prev, prev+1);
        }
        // cout<<"correct update"<<endl;
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

    time_t start, end,mid;
    time(&start);



    // **************************************** Without multithreading ***************************************************
    cout<<"Serial access without parallelizing"<<endl;
    for (int i = 0; i <= DNA_sequence.length()-k; i ++)
    {
        kmer[DNA_sequence.substr(i, k)] += 1;
    }
    time(&mid);
    int totalkmer=0;


    for(auto it=kmer.begin();it!=kmer.end();it++){
        totalkmer+=(*it).second;
    }
    cout<<(double)(mid-start)<<","<<kmer.size()<<","<<totalkmer<<endl;


    // **************************************** with multithreading ***************************************************
    cout<<"Parllelizeing using pthreads **************"<<endl<<endl;
    kmer.clear();
    totalkmer=0;


    time(&start);
    pthread_t threads[k];
    int Arguments[k];
    for(int i = 0; i < k; i++)
        Arguments[i] = i;


    for(int i=0;i<k;i++)
        pthread_create(&threads[i], NULL, thread_call,(void *)&Arguments[i]);
    
    for(int i=0;i<k;i++)
        pthread_join(threads[i], NULL);

    time(&end);



      for(auto it=kmer.begin();it!=kmer.end();it++){
        totalkmer+=(*it).second;
    }
    cout<<(double)(end-start)<<","<<kmer.size()<<","<<totalkmer<<endl;
}
