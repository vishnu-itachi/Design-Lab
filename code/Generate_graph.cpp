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

graph *generate_graph(int nodes, int density, string filename)
{
    graph *Graph = create_graph(nodes);
    ofstream input_file;

    input_file.open(filename, std::ios_base::app);
    map<pair<int, int>, int> edges;
    long long e = nodes * ((nodes - 1) / 2.0);
    e *= (density / 100.0);
    int num_edges = int(e);
    int count = 0;
    input_file << nodes << " " << num_edges << endl;
    while (count < num_edges)
    {
        int source, dest, weight;
        source = rand() % nodes;
        dest = rand() % nodes;
        weight = (rand() % 500) + 1;

        if (source > dest)
            swap(source, dest);
        // cout << source << " " << dest << endl;
        pair<int, int> p(source, dest);
        if (edges.find(p) == edges.end() && source != dest)
        {
            // cout << "working";
            edges[p] = 1;
            add_edge(Graph, source, dest, weight);
            input_file << source << " " << dest << " " << weight << endl;
            count++;
        }
    }
    input_file.close();
    return Graph;
}

void DFS(graph *Graph, int source, vector<bool> &visited)
{
    visited[source] = true;
    list<struct node *> adj = Graph->adj_list[source];
    for (auto &v : adj)
    {
        if (visited[v->destnode] == false)
            DFS(Graph, v->destnode, visited);
    }
}

int connected(graph *Graph)
{
    vector<bool> visited(Graph->num_vertices, false);
    DFS(Graph, 0, visited);
    for (auto &&t : visited)
    {
        if (t == false)
            return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{

    ofstream input_file;

    if (argc != 4)
    {
        std::cout << "Please specify n(number of nodes), d(density), s(source node)" << std::endl;
        return 0;
    }
    int nodes = atoi(argv[1]);
    int density = atoi(argv[2]);
    int source_node = atoi(argv[3]);
    string filename = "user_input_" + to_string(nodes) + "_" + to_string(density) + ".txt";
    input_file.open(filename, std::fstream::out | std::fstream::trunc);
    input_file << source_node << endl;
    input_file.close();

    graph *input_graph = generate_graph(nodes, density, filename);
    while (!connected(input_graph))
    {

        input_file.open(filename, std::fstream::out | std::fstream::trunc);
        input_file << source_node << endl;
        input_file.close();
        input_graph = generate_graph(nodes, density, filename);
    }

    cout << "Graph Generation Completed" << endl;

    return 0;
}