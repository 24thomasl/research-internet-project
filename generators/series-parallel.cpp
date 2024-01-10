/*
* Generates a weighted connected graph
* Edges are added randomly
* For weighting, each node is randomly assigned a coordinate and the edge weight is based on their distance
*
* Usage:
* ./a [n] [out]
* - n = number of nodes
* - m = number of edges
* - out = output file
*
* Ex:
* ./a 100 series-parallel.txt
*/
#include <bits/stdc++.h>
using namespace std;

const int SEED = 1434;

mt19937 rng(SEED);

int main(int argc, char *argv[]){
    assert(argc == 3);
    ofstream o(argv[2]);
    int n = stoi(argv[1]);
    for(int i = 1; i <= n; i++){
        o << "create_router " << 5 << " " << 1000 << "\n";
    }
    for(int i = 1; i < n - 1; i++){
        o << "create_connection " << i << " " << i + 1 << " 1\n";
    }
    o << "create_connection " << n - 1 << " " << n << " 40\n";
    o << "create_connection " << 1 << " " << n << " 40\n";
}