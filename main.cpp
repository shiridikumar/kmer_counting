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
void thread_call(){
    int tid=omp_get_thread_num();
    cout<<tid<<"*********"<<getppid()<<endl;
    // cout<<"ehllo "<<tid <<endl;
    int base=tid;
    for(int i=base;i<DNA_sequence.length();i+=k){
        kmer[DNA_sequence.substr(i,k)]+=1;
    }
    cout<<kmer.size()<<"***********"<<endl;
    int count=0;
    // for(auto it=kmer.begin();it!=kmer.end();it++){
    //     cout<<(*it).first<<" " <<tid<<endl;
    //     count+=1;
    //     if(count==5){
    //         break;
    //     }
    // }
}

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
     time_t start, end;
     time(&start);

    // for (int i = 0; i < DNA_sequence.length(); i ++)
    // {
    //     // cout<<DNA_sequence.substr(i,k)<<endl;
    //     kmer[DNA_sequence.substr(i, k)] += 1;
    // }
    // cout << kmer.size() << endl;
    // cout << (float)(clock() - start) / CLOCKS_PER_SEC << endl;

    pthread_t threads[100];
    pthread_t ptid;
    #pragma omp parallel for
    for(int i=0;i<k;i++){
         thread_call();
    }
    // for(int i=0;i<k;i++){
    //     pthread_join(threads[i], NULL);
    //     pthread_exit(NULL);
    // }
    time(&end);
    cout << (double)(end-start) << endl;
   

    // int rc;
}