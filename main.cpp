#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <pthread.h>

using namespace std;

string DNA_sequence;
map<string, int> kmer;
int k = 4;
void *thread_call(void* i){
    long tid;
     tid = (long)i;
    cout<<"ehllo "<<tid <<endl;
    int base=tid;
    for(int i=0;i<DNA_sequence.length();i+=k){
        kmer[DNA_sequence.substr(i,k)]+=1;
    }
    cout<<kmer.size()<<"***********"<<endl;
    return 0;
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
    for(int i=0;i<k;i++){
         pthread_create(&threads[i], NULL, thread_call, (void *)i);
    }
    for(int i=0;i<k;i++){
         pthread_join(threads[i], NULL);
    }
    time(&end);
    cout << (double)(end-start) << endl;
   

    // int rc;
}