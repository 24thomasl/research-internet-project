/*
* Genertes m packet send requests between various nodes
*
* Usage:
* ./a [n] [m] [out]
* - n = number of nodes
* - out = output file
*
* Ex:
* ./a 100 1000 packet.txt
*/
#include <bits/stdc++.h>
using namespace std;

const int SEED = 1434;

mt19937 rng(SEED);

template<class T>
inline int random(T l, T r) { return uniform_int_distribution<T>(l, r)(rng); }

int main(int argc, char *argv[]){
    assert(argc == 4);
    ofstream o(argv[3]);
    int n = stoi(argv[1]);
    int m = stoi(argv[2]);
    for(int i = 0; i < m; i++){
        o << "send_packet " << 1 << " " << n - 1 << "\n";
    }
}