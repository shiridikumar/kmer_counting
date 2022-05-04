#include<iostream>
#include<bits/stdc++.h>

using namespace std;


int main(int argc, char **argv) {
    map <string,int> kmer;
    ifstream input(argv[1]);
    if (!input.good()) {
        std::cerr << "Error opening: " << argv[1];
        return -1;
    }
    string line, id, DNA_sequence;
      while (std::getline(input, line)) {
        if(line.empty())
            continue;

        if (line[0] != '>') {
            DNA_sequence += line;
        }
    }
    cout<<DNA_sequence.length();

}