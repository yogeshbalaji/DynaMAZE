// Code for generating random mazes
// Author: Yogesh Balaji


// Maze should satisfy the condition that there should be at least one path from start to end node
// Maze model as a graph:
//	Nodes of the graph are the points in a square grid - set of all sample points in which the user can potentially be present
//	Edges are added randomly till the path condition is satisfied
//	Density of the graph - Fraction of edges to be present

#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>	
#include <set>
#include "draw_maze.cpp"

using namespace std;

struct edge
{
	int node1;
	int node2;
};

struct Graph_node
{
	int Node_id;
	int parent;
};


const int Grid_length = 20;
const int Grid_width = 20;

inline bool operator<(const edge& lhs, const edge& rhs)
{
  return (long int)(lhs.node1*10000+lhs.node2) < (long int)(rhs.node1*10000+rhs.node2);
}

int locate_vector_index(int n2, int n1, vector< vector<int> > Adjacency_list)
{
	for(int i=0;i<Adjacency_list[n2].size();i++)
	{
		if(Adjacency_list[n2][i] == n1)
			return i;
	}
	return -1;	
}

vector< vector<int> > create_graph(int Node_Index[][Grid_width+1], float Graph_density)
{
	vector< vector<int> > Adjacency_list;
	vector<edge> Edge_list;
	
	// Creating indices
	int K = 1;
	for(int i=1;i<=Grid_length;i++)
		for(int j=1;j<=Grid_width;j++)
		{
			Node_Index[i][j] = K;
			K++;
		}
	
	// Initializing adjacency list for the graph
	vector<int> Tmp;
	Adjacency_list.push_back(Tmp);
	for(int i=1;i<=Grid_length;i++)
		for(int j=1;j<=Grid_width;j++)
			Adjacency_list.push_back(Tmp);

	// Creating list of all edges
	for(int i=1;i<=Grid_length;i++)
	{
		for(int j=1;j<=Grid_width;j++)
		{
			edge Tmp2;
			if(i+1<=Grid_length)
			{
				Tmp2.node1 = Node_Index[i][j];
				Tmp2.node2 = Node_Index[i+1][j];
				Edge_list.push_back(Tmp2);
			}
			if(j+1<=Grid_width)
			{
				Tmp2.node1 = Node_Index[i][j];
				Tmp2.node2 = Node_Index[i][j+1];
				Edge_list.push_back(Tmp2);
			}
				
		}
	}
	
	// Adding edges randomly till the density
	int Count = 0;
	int Num_edges = Edge_list.size();
	srand(time(NULL));
	
	while(Count<ceil(Graph_density*Num_edges))
	{
		int Edge_to_add = rand()%Edge_list.size();
		int n1 = Edge_list[Edge_to_add].node1;
		int n2 = Edge_list[Edge_to_add].node2;
		Adjacency_list[n1].push_back(n2);
		Adjacency_list[n2].push_back(n1);
		Edge_list.erase(Edge_list.begin()+Edge_to_add);
		Count++;
	}
	
	return Adjacency_list;	
}

// Funcion to check if a node is connected to end state

int check_connectivity(vector< vector<int> >Adjacency_list, int test_node)
{
	int visited[Grid_length*Grid_width+1] = {0};
	stack<Graph_node> S;
	
	
	Graph_node Root;
	Root.Node_id = test_node;
	Root.parent = test_node;
	S.push(Root);
	
	Graph_node Tmp;
	int start = 0;
	
	while(S.size()>0)
	{
		Graph_node Aux = S.top();
		S.pop();
		
		visited[Aux.Node_id] = 1;
		if(Aux.Node_id == Grid_length*Grid_width)
		{
			return 1;
		}			
			
		for(int i=0;i<Adjacency_list[Aux.Node_id].size();i++)
		{
			if(visited[Adjacency_list[Aux.Node_id][i]] == 0)
			{
				Tmp.Node_id = Adjacency_list[Aux.Node_id][i];
				Tmp.parent = Aux.Node_id;
				S.push(Tmp);
			}
			
		}
	}
	return 0;
}

// Function to get all paths from a node to destination. Output stored as set of edges for each path

int get_path(vector<vector<int> > Adjacency_list, int u, int d, int visited[], int path[], int *path_length)
{
    // We apply DFS to store all paths from source to destination in sets. We then recursively remove edges till the number of distinct paths from source to dest is num_paths
    
    stack<Graph_node> S;
    
    Graph_node Root;
	Root.Node_id = u;
	Root.parent = u;
	S.push(Root);
	
	Graph_node Tmp;
	int Parent_map[Grid_length*Grid_width+1] = {0};
	Parent_map[Root.Node_id] = -1;
	while(S.size()>0)
	{
		Graph_node Aux = S.top();
		S.pop();		
		   
		//for(int i=0;i<path_index;i++)
		//    cout<<path[i]<<"  ";
		//cout<<endl;
				
		visited[Aux.Node_id] = 1;
		if(Aux.Node_id == Grid_length*Grid_width)
		{
		    int Trav = Aux.Node_id;
		    path[0] = Trav;
		    int k = 1;
		    while(1)
		    {
		    	if(Parent_map[Trav]==-1)
		    		break;
		        path[k] = Parent_map[Trav];
		        Trav = Parent_map[Trav];
		        k++;
		    }
		    *path_length = k;
		    return 1;
		}			
			
		for(int i=0;i<Adjacency_list[Aux.Node_id].size();i++)
		{
			if(visited[Adjacency_list[Aux.Node_id][i]] == 0)
			{
				Parent_map[Adjacency_list[Aux.Node_id][i]] = Aux.Node_id;
				Tmp.Node_id = Adjacency_list[Aux.Node_id][i];
				Tmp.parent = Aux.Node_id;
				S.push(Tmp);
			}
			
		}

	}
	return 0;
	
}


// Function to remove edges to satisfy num_paths criterion

vector< vector<int> > remove_edges(vector< vector<int> > Adjacency_list, int num_paths, int source_node, int destination)
{
    int Num_path_count = 0;		
    vector<edge> Edges_removed;
    while(1)
    {
        int visited[Grid_length*Grid_width] = {0};
        int path[Grid_length*Grid_width] = {0};
        int path_length = 0;
        int reached = get_path(Adjacency_list,1,Grid_length*Grid_width, visited, path, &path_length);

        if(reached == 0)
            break;
        edge Tmp;
        
        int Index_remove = ceil(path_length/2) + rand()%((int)ceil(path_length/2)) - ceil(path_length/4);
        Tmp.node1 = path[Index_remove];
        Tmp.node2 = path[Index_remove+1];
        //cout<<"Removing indices: "<<Nlist[Tmp.node1].col<<" , "<<Nlist[Tmp.node1].row<<" and "<<Nlist[Tmp.node2].col<<" , "<<Nlist[Tmp.node2].row<<endl;
        Edges_removed.push_back(Tmp);
        
        
        int Rem1 = locate_vector_index(path[Index_remove], path[Index_remove+1], Adjacency_list);
        int Rem2 = locate_vector_index(path[Index_remove+1], path[Index_remove], Adjacency_list);
        Adjacency_list[path[Index_remove]].erase(Adjacency_list[path[Index_remove]].begin()+Rem1);
        Adjacency_list[path[Index_remove+1]].erase(Adjacency_list[path[Index_remove+1]].begin()+Rem2);  
    }
    
    while(Edges_removed.size()>0 && Num_path_count<num_paths)
    {
        int Ind = Edges_removed.size()-1;
        int n1 = Edges_removed[Ind].node1;
        int n2 = Edges_removed[Ind].node2;
        Adjacency_list[n1].push_back(n2);
        Adjacency_list[n2].push_back(n1);
        Num_path_count++;
    }
    
    
    return Adjacency_list;
    
}



vector< vector<int> > remove_cycles(int Node_Index[][Grid_width+1], vector< vector<int> > Adjacency_list)
{
    for(int i=1;i<Grid_length;i++)
    {
        for(int j=1;j<Grid_width;j++)
        {
            int n1 = Node_Index[i][j];
            int n2 = Node_Index[i+1][j];
            int n3 = Node_Index[i][j+1];
            int n4 = Node_Index[i+1][j+1];

            if(locate_vector_index(n1,n2,Adjacency_list)>-1 && locate_vector_index(n1,n3,Adjacency_list)>-1 && locate_vector_index(n3,n4,Adjacency_list)>-1 && locate_vector_index(n2,n4,Adjacency_list)>-1) // cycle found
            {
                int Tmp_rand = rand()%4+1;
                if(Tmp_rand==1)
                {
                    int Rm1 = locate_vector_index(n1, n2, Adjacency_list);
                    int Rm2 = locate_vector_index(n2, n1, Adjacency_list);
                    Adjacency_list[n1].erase(Adjacency_list[n1].begin()+Rm1);
                    Adjacency_list[n2].erase(Adjacency_list[n2].begin()+Rm2);                    
                }
                else if(Tmp_rand==2)
                {
                    int Rm1 = locate_vector_index(n1, n3, Adjacency_list);
                    int Rm2 = locate_vector_index(n3, n1, Adjacency_list);
                    Adjacency_list[n1].erase(Adjacency_list[n1].begin()+Rm1);
                    Adjacency_list[n3].erase(Adjacency_list[n3].begin()+Rm2);                    
                }
                else if(Tmp_rand==3)
                {
                    int Rm1 = locate_vector_index(n3, n4, Adjacency_list);
                    int Rm2 = locate_vector_index(n4, n3, Adjacency_list);
                    Adjacency_list[n3].erase(Adjacency_list[n3].begin()+Rm1);
                    Adjacency_list[n4].erase(Adjacency_list[n4].begin()+Rm2);                    
                }
                else
                {
                    int Rm1 = locate_vector_index(n2, n4, Adjacency_list);
                    int Rm2 = locate_vector_index(n4, n2, Adjacency_list);
                    Adjacency_list[n2].erase(Adjacency_list[n2].begin()+Rm1);
                    Adjacency_list[n4].erase(Adjacency_list[n4].begin()+Rm2);                    
                }
            }
        }
    }
    return Adjacency_list;
}



int main()
{
	float Graph_density = 0.7;
	
	int Node_Index[Grid_length+1][Grid_width+1];
	int reachable = 0;
	vector< vector<int> > Adjacency_list;
	
	do
	{
	    Adjacency_list = create_graph(Node_Index, Graph_density);
		Adjacency_list = remove_cycles(Node_Index, Adjacency_list);
	}while(check_connectivity(Adjacency_list,1)==0);
	
	int Path[Grid_length*Grid_width];
    int Path_index = 0;    
    vector < set<int> > Path_list;
    
    Adjacency_list = remove_edges(Adjacency_list, 2, 1, Grid_length*Grid_width);
    //cout<<"Final connectivity: "<<check_connectivity(Adjacency_list,1)<<endl;
    int Num_pixels_bwn_nodes = 24;
	int Border = 50;
	int Speed = 4;
	
    Mat Img = draw_maze(Grid_length, Grid_width, Adjacency_list, Num_pixels_bwn_nodes, Border);
    move_ball(Grid_length, Grid_width, Adjacency_list, Img, Num_pixels_bwn_nodes, Border, Speed);
	return 0;	
}
