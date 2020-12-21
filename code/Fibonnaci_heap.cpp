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

struct fib_node
{
    int node_index;
    int degree;
    bool mark;
    int key;

    struct fib_node *parent;
    struct fib_node *child;
    struct fib_node *left;
    struct fib_node *right;
};

struct fibonnaciHeap
{
    fib_node *min_value_node;
    int num_nodes;

    fibonnaciHeap()
    {
        num_nodes = 0;
        min_value_node = NULL;
    }
};

fibonnaciHeap *make_heap()
{

    fibonnaciHeap *new_fib_heap = new fibonnaciHeap;
    new_fib_heap->num_nodes = 0;
    new_fib_heap->min_value_node = NULL;
    return new_fib_heap;
}

void insert_heap(fibonnaciHeap *fib_heap, fib_node *node, int key)
{
    fib_node *min_node = fib_heap->min_value_node;
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = false;

    // add this new node to the heap. Here we are adding to left side of min node.
    if (min_node != NULL)
    {
        fib_node *temp = min_node->left;
        min_node->left = node;
        node->right = min_node;
        node->left = temp;
        temp->right = node;
    }
    // check with current minimum
    if (min_node == NULL || min_node->key > node->key)
    {
        fib_heap->min_value_node = node;
    }

    fib_heap->num_nodes += 1;
}
// this function adds the node that was cut to the root list of nodes
void add_to_root_list(fibonnaciHeap *fib_heap, fib_node *node)
{
    fib_node *min_node = fib_heap->min_value_node;
    node->parent = NULL;
    node->mark = false;

    // we perform same operations as inserting a new node as in above funtion
    if (min_node != NULL)
    {
        fib_node *temp = min_node->left;
        min_node->left = node;
        node->right = min_node;
        node->left = temp;
        temp->right = node;

        if (min_node == NULL || min_node->key > node->key)
        {
            fib_heap->min_value_node = node;
        }
    }
    // if min node is NULL then add this node as min node
    else
    {
        fib_heap->min_value_node = node;
        node->right = node;
        node->left = node;
    }
}

fibonnaciHeap *heap_union(fibonnaciHeap *fib_heap1, fibonnaciHeap *fib_heap2)
{

    fibonnaciHeap *new_fib_heap = make_heap();
    //get min nodes
    fib_node *min_node1 = fib_heap1->min_value_node;
    fib_node *min_node2 = fib_heap2->min_value_node;

    if (min_node1 == NULL)
        new_fib_heap->min_value_node = min_node2;
    else if (min_node2 == NULL)
        new_fib_heap->min_value_node = min_node1;
    else
    {
        fib_node *temp1 = min_node1->left;
        fib_node *temp2 = min_node2->left;
        min_node1->left = temp2;
        min_node2->left = temp1;
        temp1->right = min_node2;
        temp2->right = min_node1;

        //check for min node now
        if (min_node1->key > min_node2->key)
            new_fib_heap->min_value_node = min_node2;
        else
            new_fib_heap->min_value_node = min_node1;
    }
    // add the total nodes
    new_fib_heap->num_nodes = fib_heap1->num_nodes + fib_heap2->num_nodes;

    return new_fib_heap;
}

// add child node to parent node
void add_child(fib_node *parent, fib_node *new_child)
{
    //check if parent has any child or not
    if (parent->degree == 0)
    {
        parent->child = new_child;
        new_child->right = new_child->left = new_child;
        new_child->parent = parent;
    }
    else
    {
        // get the left child and and update it since we use LCRS (left most child and right most sibling)
        fib_node *child1 = parent->child;
        fib_node *child1_temp = child1->left;
        child1->left = new_child;
        new_child->right = child1;
        new_child->left = child1_temp;
        child1_temp->right = new_child;
    }
    new_child->parent = parent;
    parent->degree++;
}

void remove_node(fib_node *node)
{
    // check if node has siblings if no remove this node by skipping that
    if (node->right != node)
    {
        node->left->right = node->right;
        node->right->left = node->left;
    }
    // node is a child then we need to set any one of sibling as child of the parent
    if (node->parent != NULL)
    {
        int p_degree = node->parent->degree;
        if (p_degree == 1)
        {
            node->parent->child = NULL;
        }
        else
        {
            node->parent->child = node->right;
        }
        //decrease parent degree
        node->parent->degree -= 1;
    }
}

// Main function which joins trees and ensures that no two trees as same degree
void heap_consolidate(fibonnaciHeap *fib_heap)
{
    int num_roots = 0;
    int count = 0;
    int degree;
    // if only 1 node do nothing
    if (fib_heap->num_nodes > 1)
    {
        // auto degtablesize = fib_heap->num_nodes;
        vector<fib_node *> dtable; // table to store degrees
        // initialisations
        fib_node *current_node = fib_heap->min_value_node;
        fib_node *existing_node, *current_con_node;

        fib_node *temp = fib_heap->min_value_node;
        fib_node *iter_node = fib_heap->min_value_node;

        //calculate nodes in root list
        do
        {
            num_roots++;
            iter_node = iter_node->right;
        } while (iter_node != temp);
        // cout << "con1: " << num_roots << endl;

        while (count < num_roots)
        {
            current_con_node = current_node;
            current_node = current_node->right;
            degree = current_con_node->degree;
            // cout << degree << endl;
            while (1)
            {
                //run loop untill all nodes of same degree are merged
                while (degree >= dtable.size())
                {
                    dtable.push_back(NULL);
                }
                if (dtable[degree] == NULL)
                {

                    dtable[degree] = current_con_node; // add the cyurrent node
                    break;
                }
                else
                {
                    // this means there is already existing node with same degree
                    existing_node = dtable[degree];
                    if (current_con_node->key > existing_node->key)
                    {
                        //swap
                        fib_node *temp1 = current_con_node;
                        current_con_node = existing_node;
                        existing_node = temp1;
                    }
                    if (existing_node == current_con_node)
                    {

                        break;
                    }
                    // we will link the nodes
                    remove_node(existing_node);
                    add_child(current_con_node, existing_node);
                    //mark value of the larger node is marked false.
                    existing_node->mark = false;
                    dtable[degree] = NULL;
                    degree++;
                }
            }
            count++;
        }

        //now we all nodes to the list
        fib_heap->min_value_node = NULL;
        for (int i = 0; i < dtable.size(); i++)
        {
            if (dtable[i] != NULL)
            {
                add_to_root_list(fib_heap, dtable[i]);
            }
        }
    }
}

// extract minimum element based on min ppinter
fib_node *extract_min(fibonnaciHeap *fib_heap)
{
    fib_node *min_node = fib_heap->min_value_node;

    if (min_node != NULL)
    {
        // now we need to add all childs to root list
        int degree = min_node->degree;
        int count = 0;
        fib_node *cur_node = min_node->child;
        // cout << "degree: " << degree << endl;
        while (count < degree)
        {
            fib_node *remove_child = cur_node;
            cur_node = cur_node->right;
            add_to_root_list(fib_heap, remove_child);
            count++;
        }
        remove_node(min_node); // remove the node from root list

        fib_heap->num_nodes = fib_heap->num_nodes - 1;
        if (fib_heap->num_nodes == 0)
        {
            fib_heap->min_value_node = NULL;
        }
        else
        {
            fib_heap->min_value_node = min_node->right;
            fib_node *temp = min_node->left;
            fib_heap->min_value_node->left = temp;
            temp->right = fib_heap->min_value_node;

            heap_consolidate(fib_heap);
        }
    }
    return min_node;
}

// We need a function to cut the child whose parent mark is false
void cascate_cut(fibonnaciHeap *fib_heap, fib_node *node)
{
    fib_node *parent_node = node->parent;
    if (parent_node != NULL)
    {
        if (node->mark == false)
        {
            node->mark = true;
        }
        else
        {
            //remove node and recursively go up still u get mark as false
            remove_node(node);
            add_to_root_list(fib_heap, node);
            cascate_cut(fib_heap, parent_node);
        }
    }
}

void decrease_key(fibonnaciHeap *fib_heap, fib_node *node, int key)
{
    int previous_key = node->key;
    // if present key is bigger the order wont change
    if (key > previous_key)
        return;
    //else replace and cut

    node->key = key;
    if (node->parent != NULL)
    {
        //check for parent key
        if (node->parent->key > node->key)
        {
            fib_node *parent_node = node->parent;
            remove_node(node);
            add_to_root_list(fib_heap, node);
            cascate_cut(fib_heap, parent_node);
        }
    }
    //lastly check and update min pointer
    if (node->key < fib_heap->min_value_node->key)
        fib_heap->min_value_node = node;
}

int get_min_distace_node(graph *Graph, fibonnaciHeap *fib_heap, fib_node **node_array, vector<bool> &marked)
{

    // int min = INT_MAX;
    int min_index;

    fib_node *min_node = extract_min(fib_heap);

    if (min_node != NULL)
    {
        // get min dist node
        min_index = min_node->node_index;
        marked[min_index] = true;
        //update distances
        list<struct node *> adj = Graph->adj_list[min_index];
        for (auto &v : adj)
        {
            int adj_index = v->destnode;
            if (marked[adj_index] == false)
            {
                //update distance
                if (node_array[adj_index]->key > node_array[min_index]->key + v->weight)
                {
                    decrease_key(fib_heap, node_array[adj_index], node_array[min_index]->key + v->weight);
                }
            }
        }
    }

    return min_index;
}

void dijkstra(graph *Graph, int source)
{
    //Starting the clock.
    clock_t Start, Time;
    Start = clock();
    //create individual nodes
    int nodes = Graph->num_vertices;
    // cout << nodes;
    fib_node **node_array = new fib_node *[Graph->num_vertices];

    vector<bool> marked(nodes, false);
    fibonnaciHeap *fib_heap = new fibonnaciHeap;
    //insert all nodes into heap

    for (int i = 0; i < Graph->num_vertices; i++)
    {
        node_array[i] = new fib_node;
        if (i == source)
            insert_heap(fib_heap, node_array[i], 0);
        else
            insert_heap(fib_heap, node_array[i], INT_MAX);

        node_array[i]->node_index = i;
    }
    // cout << "nodes : " << fib_heap->num_nodes << endl;
    int count = 0;
    while (count < nodes)
    {
        get_min_distace_node(Graph, fib_heap, node_array, marked);
        // cout << "count: " << count << endl;
        count++;
    }
    Time = clock() - Start;
    cout << "Total execution time using Fibonacci Heap : " << (double)Time / (double)CLOCKS_PER_SEC << " secs\n";
    ofstream myfile;
    myfile.open("output_fibonacci.txt", std::fstream::out | std::fstream::trunc);

    for (int i = 0; i < Graph->num_vertices; i++)
    {
        myfile << i << " : " << node_array[i]->key << "\n";
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