#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// node = city
// edge = path

typedef struct
{
	int val; // index of city
	char name[50]; // name of city
} t_node; // type definition of node
// creates a node wuth value and name anr returns it
t_node* NodeCreate(int val, const char* name)
{
	t_node* n = (t_node*) malloc(sizeof(t_node));
	n->val = val;
	strncpy(n->name, name, sizeof(n->name));
	return n;
}

typedef struct
{
	int src; // source node index
	int dst; // destination node index
	int len; // length of path
} t_edge; // type definition of edge
// create a new edge and return it
t_edge* EdgeCreate(int src, int dst, int len)
{
	t_edge* e = (t_edge*) malloc(sizeof(t_edge));
	e->src = src;
	e->dst = dst;
	e->len = len;
	return e;
}

// graph struct
typedef struct
{
	int node_count;
	t_node nodes[1000];
	int edge_count;
	t_edge edges[1000];
} t_graph;

// create a new graph and return it
t_graph* GraphCreate()
{
	t_graph* g = (t_graph*) malloc(sizeof(t_graph));
	g->edge_count = 0;
	g->node_count = 0;
	
	return g;
}
// add a node into graph
void GraphAddNode(t_graph* g, int val, const char* name)
{
	t_node* n = &(g->nodes[g->node_count]);
	n->val = val;
	strcpy(n->name, name);

	++g->node_count;
}
// add a edge into graph
void GraphAddEdge(t_graph* g, int src, int dst, int len)
{
	t_edge* e = &(g->edges[g->edge_count]);
	e->src = src;
	e->dst = dst;
	e->len = len;

	++g->edge_count;
}
// get name of given node index
const char* GraphGetNodeName(t_graph* g, int n)
{
	// for all nodes in given graph
	for (size_t i = 0; i < g->node_count; i++)
	{
		// if index equals given index
		if (g->nodes[i].val == n)
		{
			// return that node's name
			return g->nodes[i].name;
		}
	}

	return "";
}

// path struct
typedef struct s_path
{
	int indices[1000]; // node indices in path, from source to destination
	int index_count; // nount of node indices
	int total_len; // total length of that path
} t_path;
t_path* CreatePath()
{
	t_path* p = (t_path*) malloc(sizeof(t_path));
	memset(p, 0, sizeof(t_path));
	return p;
}
// add a new node index into path
void PathAddIndex(t_path* p, int index)
{
	// set index into path's empty area and increase index count of path
	p->indices[p->index_count++] = index;
}
// returns if given path includes node with the given index
int PathHasIndex(t_path* p, int index)
{
	// for all nodes in path
	for (size_t i = 0; i < 1000; i++)
	{
		if (p->indices[i] == 0) // if no more indices
			break; // break loop and return false
		if (p->indices[i] == index) // if we find index
			return 1; // return true
	}
	return 0;
}
// copy given path and return new one
t_path* PathCopy(t_path* path)
{
	// allocate memory and copy all data from given into new allocated
	t_path* p = (t_path*) malloc(sizeof(t_path));
	memcpy(p, path, sizeof(t_path));
	return p;
}

// tries to find a valid path from source node index into destination node index,
// if find, prints the path and total lenght of path
// if cannot find, prints 'not found'
void FindPath(t_graph* g, int src, int dst)
{
	printf("Path %10s ->%10s :: ", GraphGetNodeName(g, src), GraphGetNodeName(g, dst));

	// create array to store paths
	t_path* paths[1000];
	memset(paths, 0, sizeof(t_path*) * 1000);
	// create fist path with given source node
	paths[0] = CreatePath();
	PathAddIndex(paths[0], src);
	int path_count = 1;

	// if there is more path
	while (path_count)
	{
		t_path* path = paths[0];

		// try to find edges which starts from last node of path

		 // for all edges
		for (size_t i = 0; i < g->edge_count; i++)
		{
			t_edge e = g->edges[i];

			// if that edge continues with out path
			if (e.src == path->indices[path->index_count-1])
			{
				// if we already arrived that node, continue to another edge
				if (PathHasIndex(path, e.dst))
					continue;

				if (e.dst == dst) // if we find destination node
				{
					// print path and distance
					for (size_t j = 0; j < path->index_count; j++)
					{
						printf("%s, ", GraphGetNodeName(g, path->indices[j]));
					}
					printf("%s | Total distance: %d\n", GraphGetNodeName(g, e.dst), path->total_len + e.len);
					return;
				}
				else
				{
					// create a copy of that path
					t_path* new_path = PathCopy(path);
					// add that node into path
					PathAddIndex(new_path, e.dst);
					new_path->total_len += e.len;
					paths[path_count++] = new_path;
				}
			}
		}

		// remove first path (move afters into zero starting index)
		for (size_t i = 0; i < path_count-1; i++)
		{
			paths[i] = paths[i+1];
		}
		--path_count;
	}

	printf("Not found.\n");
}

int main(int argc, char const *argv[])
{
	// create graph
	t_graph* g = GraphCreate();

	// add nodes into graph
	// dont use 0 as a node index!
	GraphAddNode(g, 1, "Prague");
	GraphAddNode(g, 2, "Helsinki");
	GraphAddNode(g, 3, "Beijing");
	GraphAddNode(g, 4, "Tokyo");
	GraphAddNode(g, 5, "Jakarta");
	GraphAddNode(g, 6, "London");
	GraphAddNode(g, 7, "New York");

	// add edges into graph
	GraphAddEdge(g, 1, 2, 1845); // p h
	GraphAddEdge(g, 2, 4, 7815); // h t
	GraphAddEdge(g, 3, 4, 1303); // b t
	GraphAddEdge(g, 4, 5, 5782); // t j
	GraphAddEdge(g, 5, 3, 4616); // j b
	GraphAddEdge(g, 3, 6, 8132); // b l
	GraphAddEdge(g, 1, 6, 1264); // p l
	GraphAddEdge(g, 7, 6, 5567); // n l
	GraphAddEdge(g, 3, 7, 11550); // b n
	GraphAddEdge(g, 4, 7, 10838); // t n
	GraphAddEdge(g, 6, 4, 9566); // l t

	printf("Cities:\n");
	for (size_t i = 0; i < g->node_count; i++)
	{
		printf("%3d: %s\n", g->nodes[i].val-1, g->nodes[i].name);
	}

	int src = 0, dst = 0;
	printf("Enter first (source) city code: ");
	scanf("%d", &src);
	printf("Enter second (destination) city code: ");
	scanf("%d", &dst);

	FindPath(g, src+1, dst+1);

	// lets do the thing, bcs why not
	// FindPath(g, 1, 2);
	// FindPath(g, 1, 3);
	// FindPath(g, 1, 4);
	// FindPath(g, 1, 5);
	// FindPath(g, 1, 6);
	// FindPath(g, 1, 7);

	// FindPath(g, 2, 1);
	// FindPath(g, 2, 3);
	// FindPath(g, 2, 4);
	// FindPath(g, 2, 5);
	// FindPath(g, 2, 6);
	// FindPath(g, 2, 7);

	// FindPath(g, 3, 1);
	// FindPath(g, 3, 2);
	// FindPath(g, 3, 4);
	// FindPath(g, 3, 5);
	// FindPath(g, 3, 6);
	// FindPath(g, 3, 7);

	// FindPath(g, 4, 1);
	// FindPath(g, 4, 2);
	// FindPath(g, 4, 3);
	// FindPath(g, 4, 5);
	// FindPath(g, 4, 6);
	// FindPath(g, 4, 7);

	// FindPath(g, 5, 1);
	// FindPath(g, 5, 2);
	// FindPath(g, 5, 3);
	// FindPath(g, 5, 4);
	// FindPath(g, 5, 6);
	// FindPath(g, 5, 7);

	// FindPath(g, 6, 1);
	// FindPath(g, 6, 2);
	// FindPath(g, 6, 3);
	// FindPath(g, 6, 4);
	// FindPath(g, 6, 5);
	// FindPath(g, 6, 7);

	// FindPath(g, 7, 1);
	// FindPath(g, 7, 2);
	// FindPath(g, 7, 3);
	// FindPath(g, 7, 4);
	// FindPath(g, 7, 5);
	// FindPath(g, 7, 6);

	return 0;
}

