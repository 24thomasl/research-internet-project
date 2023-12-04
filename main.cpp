#include <bits/stdc++.h>
using namespace std;

struct Packet {
    int dest_id; // id of router destination
    int packet_id; // id of packet
    int orig_time; // time packet was sent

    Packet(){}

    Packet(int dest_id_, int packet_id_, int orig_time_){
        dest_id = dest_id_;
        packet_id = packet_id_;
        orig_time = orig_time_;
    }
};

enum EventType {
    REQUEST, // router requests to process event
    SEND // packet is sent
};

struct Event {

    int time; // time of request 
    int id; // id of relevant router
    Packet p; // relevant packet if sending
    EventType type;
    int src; // source router if sending

    Event(){}

    Event(int time_, int id_, int src_, Packet p_){
        time = time_;
        id = id_;
        type = SEND;
        p = p_;
        src = src_;
    }

    Event(int time_, int id_){
        time = time_;
        id = id_;
        type = REQUEST;
    }

    bool operator<(const Event& x) const
    {
        return time > x.time;
    }

};


const int MAX_ROUTERS = 1000;
int num_routers;

int cost[MAX_ROUTERS][MAX_ROUTERS]; // cost to go from i to j
bool connected[MAX_ROUTERS][MAX_ROUTERS]; // does there exist a connection from i to j

priority_queue<Event> global_queue;

struct Router {

    int internal_time;   // router's internal clock
    int time_per_request; // time it takes for router to process a request
    int time_per_packet; // time it takes for router to send a packet

    queue<Packet> packet_queue;

    int id; // router's id
    
    Router(){}

    Router(int time_per_request_, int time_per_packet_, int id_) {
        internal_time = 0;
        time_per_request = time_per_request_;
        time_per_packet = time_per_packet_;
        id = id_;
    }

    // {id of target, time taken to find target}
    pair<int, int> get_next_id(const Packet &p){
        int tar = p.dest_id;
        auto start = chrono::high_resolution_clock::now();

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        int dist[num_routers];
        int par[num_routers];
        for(int i = 0; i < num_routers; i++) dist[i] = 1000000000, par[i] = -1;

        pq.push({0, id});
        dist[id] = 0;

        while(!pq.empty()){
            pair<int, int> x = pq.top();
            pq.pop();
            if(x.first > dist[x.second]) continue;
            for(int i = 0; i < num_routers; i++){
                if(connected[x.second][i] && dist[i] > x.first + cost[x.second][i]){
                    dist[i] = x.first + cost[x.second][i];
                    par[i] = x.second;
                    pq.push({dist[i], i});
                }
            }
        }

        auto stop = chrono::high_resolution_clock::now();
        if(par[tar] == -1) return {-1, 0};
        while(par[tar] != id) tar = par[tar];
        return {tar, (chrono::duration_cast<chrono::microseconds>(stop - start)).count()};
    }

    void processEvent() { // processes the action the router wants to perform and returns the next event
        if(packet_queue.size() > 0){
            Packet p = packet_queue.front();
            packet_queue.pop();
            if(p.dest_id == id){
                cout << "Packet " << p.packet_id << " successfully reached router " << id << " in " << internal_time - p.orig_time << " microseconds" << endl;
            } else {
                pair<int, int> nxt = get_next_id(p);
                internal_time += nxt.second;
                if(nxt.first != -1){
                    global_queue.push(Event(internal_time + time_per_packet, nxt.first, id, p));
                } else {
                    cout << "Router " << id << " couldn't find destination for packet " << p.packet_id << endl;
                }
            }
        }
        internal_time += time_per_request;
        global_queue.push(Event(internal_time, id));
    }
};

Router routers[MAX_ROUTERS];

void process(Event e){
    if(e.type == SEND){
        routers[e.id].packet_queue.push(e.p);
        if(e.src == -1){
            cout << "Packet " << e.p.packet_id << " started at router " << e.id << " at time " << e.time << endl;
        } else {
            cout << "Packet " << e.p.packet_id << " sent from router " << e.src << " to router " << e.id << " at time " << e.time << endl;
        }
    } else {
        routers[e.id].processEvent();
    }
}
/*
create_router 1 1
create_router 1 1
create_connection 0 1 0
send_packet 0 1
*/

int main(int argc, char *argv[]){
    num_routers = 0;
    memset(connected, false, sizeof(connected));
    string op;
    int cur_packet = 0;
    int cur_time = 0;
    while(cin >> op){
        if(op == "create_router"){
            int time_per_request, time_per_packet;
            cin >> time_per_request >> time_per_packet;
            routers[num_routers] = Router(time_per_request, time_per_packet, num_routers);
            global_queue.push(Event(cur_time, num_routers));
            cout << "Created router with id " << num_routers << ", time per request " << time_per_request << ", and time per packet " << time_per_packet << endl;
            num_routers++;
        } else if(op == "create_connection"){
            int a, b, c;
            cin >> a >> b >> c;
            cost[a][b] = cost[b][a] = c;
            connected[a][b] = connected[b][a] = true;
            cout << "Created connection between router " << a << " and router " << b << " having a delay of " << c << endl;
        } else if(op == "delete_connection"){
            int a, b;
            cin >> a >> b;
            connected[a][b] = connected[b][a] = false;
            cout << "Deleted connection between router " << a << " and router " << b << endl;
        } else if(op == "send_packet"){
            int src, dest;
            cin >> src >> dest;
            Packet p = Packet(dest, cur_packet, cur_time);
            global_queue.push(Event(cur_time, src, -1, p));
            cout << "Queued packet from router " << src << " to router " << dest << " having id " << cur_packet << endl;
        } else if(op == "advance"){
            int time;
            cin >> time;
            cout << "Advancing system from time " << cur_time << " to time " << cur_time + time << "..." << endl;
            cur_time += time;
            while(global_queue.size() && global_queue.top().time <= cur_time){
                Event e = global_queue.top();
                global_queue.pop();
                process(e);
            }
        } else {
            cout << "Invalid operation" << endl;
        }
    }
}