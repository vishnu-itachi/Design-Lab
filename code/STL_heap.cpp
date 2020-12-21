#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct node
{
    int destnode;
    int weight;
};

struct graph
{
    int num_vertices;
    map<int, list<struct node *>> adj_list;
};

graph *create_graph(int vertices)
{
    graph *Graph = new graph;
    Graph->num_vertices = vertices;
    return Graph;
}

void add_edge(graph *Graph, int s, int d, int w)
{
    struct node *newnode1 = new node;
    newnode1->destnode = d;
    newnode1->weight = w;
    Graph->adj_list[s].push_back(newnode1);

    // for undirected graph
    struct node *newnode2 = new node;
    newnode2->destnode = s;
    newnode2->weight = w;
    Graph->adj_list[d].push_back(newnode2);
}

void display_graph(graph *Graph)
{
    for (auto v = 0; v < (Graph->num_vertices); v++)
    {

        list<struct node *> adj = Graph->adj_list[v];
        for (auto &vertex : adj)
        {
            cout << v << " " << vertex->destnode << " " << vertex->weight << endl;
        }
    }
}

void dijkstra(graph *Graph, int source)
{
    //Starting the clock.
    clock_t Start, Time;
    Start = clock();
    int nodes = Graph->num_vertices;
    vector<int> distance(nodes, INT_MAX);
    // vector<bool> visited(nodes, false);
    set<pair<int, int>> setds;

    setds.insert(make_pair(0, source));
    distance[source] = 0;

    while (!setds.empty())
    {
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());
        int u = tmp.second;

        list<struct node *> adj = Graph->adj_list[u];
        for (auto &i : adj)
        {
            int v = i->destnode;
            int weight = i->weight;

            if (distance[v] > distance[u] + weight)
            {
                if (distance[v] != INT_MAX)
                    setds.erase(setds.find(make_pair(distance[v], v)));

                distance[v] = distance[u] + weight;
                setds.insert(make_pair(distance[v], v));
            }
        }
    }
    Time = clock() - Start;
    cout << "Total execution time using STL set : " << (double)Time / (double)CLOCKS_PER_SEC << " secs\n";

    ofstream myfile;
    myfile.open("output_stl.txt", std::fstream::out | std::fstream::trunc);

    for (int i = 0; i < Graph->num_vertices; i++)
    {
        myfile << i << " : " << distance[i] << "\n";
    }
    myfile.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Please specify graph file name" << std::endl;
        return 0;
    }
    string filename = argv[1];
    std::ifstream infile(filename);
    if (!infile.good())
    {
        cout << "Incorrect File name given" << endl;
        return 0;
    }
    int i = 0;
    int source, nodes, edges;
    int s, d, w;
    std::string line;
    graph *Graph;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        if (i == 0)
        {
            iss >> source;
        }
        else if (i == 1)
        {
            iss >> nodes >> edges;
            Graph = create_graph(nodes);
            // cout << nodes << " " << edges << std::endl;
        }
        else
        {
            iss >> s >> d >> w;
            // cout << s << " " << w << endl;
            add_edge(Graph, s, d, w);
        }
        i++;
    }

    // display_graph(Graph);
    dijkstra(Graph, source);
    // cout << "Completed" << endl;

    return 0;
}