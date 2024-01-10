/*
* Generates a weighted connected graph
* Edges are added randomly
* For weighting, each node is randomly assigned a coordinate and the edge weight is based on their distance
*
* Usage:
* ./a [n] [m] [mn] [mx] [lim1] [lim2] [out]
* - n = number of nodes
* - m = number of edges
* - mn = minimum coordinate
* - mx = maximum coordinate
* - lim1 = time_per_request and time_per_packet is scaled to lim1
* - lim2 = distance is scaled to lim2
* - out = output file
*
* Ex:
* ./a 100 1000 1 1000 20 100 connected.txt
*/
#include <bits/stdc++.h>
using namespace std;

//const int SEED = 1434;
const int SEED = 1437;

mt19937 rng(SEED);

template<class T>
inline int random(T l, T r) { return uniform_int_distribution<T>(l, r)(rng); }

vector<pair<int, int>> genTree(int n, int rt = 1){
    vector<pair<int, int>> e(n - 1);
    for(int i = 2; i <= n; i++){
        int a = random(1, i - 1);
        int b = i;
        e[i - 2] = {(a + rt - 1)%n + 1, (b + rt - 1)%n + 1};
    }
    return e;
}

vector<pair<int, int>> genGraph(int n, int m){
    int mx = n*(n - 1)/2;
    m = min(m, mx);
    vector<pair<int, int>> e;
    map<pair<int, int>, bool> vis;
    e = genTree(n, random(1, n));
    for(pair<int, int> &i : e){
        if(i.first > i.second) swap(i.first, i.second);
        vis[i] = true;
        if(random(0, 1)) swap(i.first, i.second);
    }
    for(int i = n - 1; i < m; i++){
        int a = random(1, n - 1);
        int b = random(a + 1, n);
        if(a > b) swap(a, b);
        while(vis[{a, b}]){
            a = random(1, n - 1);
            b = random(a + 1, n);
            if(a > b) swap(a, b);
        }
        vis[{a, b}] = true;
        if(random(0, 1)) swap(a, b);
        e.push_back({a, b});
    }
    return e;
}

int dist(pair<int, int> a, pair<int, int> b){
    return (a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second);
}

int main(int argc, char *argv[]){
    assert(argc == 8);
    ofstream o(argv[7]);
    int n = stoi(argv[1]);
    int m = stoi(argv[2]);
    int mn = stoi(argv[3]);
    int mx = stoi(argv[4]);
    int lim1 = stoi(argv[5]);
    int lim2 = stoi(argv[6]);
    vector<pair<int, int>> coord(n);
    for(int i = 0; i < n; i++) coord[i] = {random(mn, mx), random(mn, mx)};
    vector<pair<int, int>> e = genGraph(n, m);
    for(int i = 1; i <= n; i++){
        o << "create_router " << random(1, lim1) << " " << random(1, lim1) << "\n";
    }
    int div = dist({mn, mn}, {mx, mx});
    for(pair<int, int> i : e){
        o << "create_connection " << i.first - 1 << " " << i.second - 1 << " ";
        double scale = (double)dist(coord[i.first - 1], coord[i.second - 1])/div;
        scale *= lim2;
        o << (int)scale << "\n";
    }
}