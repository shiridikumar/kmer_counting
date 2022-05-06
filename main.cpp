#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

using namespace std;

string DNA_sequence;
map<string, int> kmer1;
map<string, int> kmer;
vector<string> strings;
int k = 6;
void *thread_call(void *ids)
{
    long id = (long)ids;
    int tid = (int)id;
    int base = tid;
    int prev;
    uint32_t ret;
    for (int i = base; i <=DNA_sequence.length() - k; i += k)
    {
        // cout<<i<<endl;
        string seq = DNA_sequence.substr(i, k);
        prev = kmer[seq];
        ret = __sync_bool_compare_and_swap(&kmer[seq], prev, prev + 1);
        while (ret == 0)
        {
            prev = kmer[seq];
            // cout<<"correcting race condition "<<seq<<" "<<prev<<endl;
            ret = __sync_bool_compare_and_swap(&kmer[seq], prev, prev + 1);

            // cout<<"corrected race condition "<<seq<<" "<<kmer[seq]<<endl;
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
    cout << "Serial access without parallelizing"<<endl;
    for (int i = 0; i <=DNA_sequence.length() - k; i++)
    {
        kmer1[DNA_sequence.substr(i, k)] += 1;
        kmer[DNA_sequence.substr(i,k)]=0;
    }
    // cout<<"loop finsihed"<<endl;
    // cout << omp_get_max_threads() << "***************" << endl;


// #pragma omp parallel for
//     for (int i = 0; i <= DNA_sequence.length() - k; i++)
//     {
//         kmer[DNA_sequence.substr(i, k)] = 0;
//     }

    // cout<<"pragma finished"<<endl;
    time(&mid);
    int totalkmer = 0;

    for (auto it = kmer1.begin(); it != kmer1.end(); it++)
    {
        totalkmer += (*it).second;
    }
    cout << (double)(mid - start) << "," << kmer1.size() << "," << totalkmer << endl;
    cout << "Parllelizeing using pthreads" << endl;
    totalkmer = 0;
    time(&start);
    // kmer.clear();

    pthread_t threads[k];
    // cout<<"hedda"<<endl;
    for (int i = 0; i < k; i++)
    {
        pthread_create(&threads[i], NULL, thread_call, (void *)i);
    }
    for (int i = 0; i < k; i++)
    {
        pthread_join(threads[i], NULL);
    }
    time(&end);
    for (auto it = kmer.begin(); it != kmer.end(); it++)
    {
        totalkmer += (*it).second;
    }
    int c = 0;
    for (auto it = kmer1.begin(); it != kmer1.end(); it++)
    {
        if (kmer[(*it).first] == 0)
        {
            cout << (*it).first << " " << (*it).second << " " << kmer.size() << " " << kmer1.size() << endl;
        }
        else
        {
            c += 1;
        }
    }
    cout << (double)(end - start) << "," << kmer.size() << "," << totalkmer << endl;
}
