#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "pathfinding.h"

/**
 * struct dijk_node_s - Per-vertex data for Dijkstra's algorithm
 * @vertex: Pointer to the graph vertex
 * @distance: Shortest known distance from start
 * @previous: Previous vertex on the shortest path
 * @visited: 1 if this vertex has been finalised
 */
typedef struct dijk_node_s
{
	vertex_t *vertex;
	int distance;
	vertex_t *previous;
	int visited;
} dijk_node_t;

/**
 * init_data - Initialise Dijkstra data array from graph vertex list
 * @data: Array to initialise
 * @graph: The graph
 * @start: Starting vertex (gets distance 0)
 */
static void init_data(dijk_node_t *data, graph_t *graph,
		vertex_t const *start)
{
	vertex_t *v;
	int i;

	v = graph->vertices;
	i = 0;
	while (v && i < (int)graph->nb_vertices)
	{
		data[i].vertex = v;
		data[i].distance = (v == start) ? 0 : INT_MAX;
		data[i].previous = NULL;
		data[i].visited = 0;
		v = v->next;
		i++;
	}
}

/**
 * pick_min - Find the unvisited vertex with the smallest distance
 * @data: The data array
 * @n: Number of entries
 *
 * Return: Index of the minimum vertex, or -1 if none reachable
 */
static int pick_min(dijk_node_t *data, int n)
{
	int i, best, best_dist;

	best = -1;
	best_dist = INT_MAX;
	for (i = 0; i < n; i++)
	{
		if (!data[i].visited && data[i].distance < best_dist)
		{
			best_dist = data[i].distance;
			best = i;
		}
	}
	return (best);
}

/**
 * relax_edges - Update neighbour distances from the current vertex
 * @data: The data array
 * @n: Number of entries
 * @cur_idx: Index of the current vertex in data
 */
static void relax_edges(dijk_node_t *data, int n, int cur_idx)
{
	edge_t *edge;
	int j, new_dist, dest_idx;

	edge = data[cur_idx].vertex->edges;
	while (edge)
	{
		dest_idx = -1;
		for (j = 0; j < n; j++)
		{
			if (data[j].vertex == edge->dest)
			{
				dest_idx = j;
				break;
			}
		}
		if (dest_idx != -1 && !data[dest_idx].visited)
		{
			new_dist = data[cur_idx].distance + edge->weight;
			if (new_dist < data[dest_idx].distance)
			{
				data[dest_idx].distance = new_dist;
				data[dest_idx].previous = data[cur_idx].vertex;
			}
		}
		edge = edge->next;
	}
}

/**
 * reconstruct - Build the result queue from target back to start
 * @data: The data array
 * @n: Number of entries
 * @target: The target vertex
 *
 * Return: Queue with path from start to target, or NULL on failure
 */
static queue_t *reconstruct(dijk_node_t *data, int n,
		vertex_t const *target)
{
	queue_t *path;
	char **arr;
	vertex_t *cur;
	char *copy;
	int len, i, j;

	arr = malloc((n + 1) * sizeof(char *));
	if (!arr)
		return (NULL);

	cur = (vertex_t *)target;
	len = 0;
	while (cur)
	{
		copy = strdup(cur->content);
		if (!copy)
		{
			for (i = 0; i < len; i++)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		arr[len++] = copy;
		cur = NULL;
		for (j = 0; j < n; j++)
		{
			if (data[j].vertex == (vertex_t *)target)
			{
				cur = data[j].previous;
				target = cur;
				break;
			}
		}
	}

	path = queue_create();
	if (!path)
	{
		for (i = 0; i < len; i++)
			free(arr[i]);
		free(arr);
		return (NULL);
	}

	for (i = len - 1; i >= 0; i--)
		enqueue(path, arr[i]);

	free(arr);
	return (path);
}

/**
 * dijkstra_graph - Find the shortest path using Dijkstra's algorithm
 * @graph: The graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path found
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
		vertex_t const *target)
{
	dijk_node_t *data;
	queue_t *path;
	int n, i, min_idx, target_idx;

	if (!graph || !start || !target)
		return (NULL);

	n = (int)graph->nb_vertices;
	data = malloc(n * sizeof(dijk_node_t));
	if (!data)
		return (NULL);

	init_data(data, graph, start);

	for (i = 0; i < n; i++)
	{
		min_idx = pick_min(data, n);
		if (min_idx == -1)
			break;
		data[min_idx].visited = 1;
		printf("Checking %s, distance from %s is %d\n",
			data[min_idx].vertex->content,
			start->content,
			data[min_idx].distance);
		relax_edges(data, n, min_idx);
	}

	target_idx = -1;
	for (i = 0; i < n; i++)
	{
		if (data[i].vertex == target)
		{
			target_idx = i;
			break;
		}
	}

	if (target_idx == -1 || data[target_idx].distance == INT_MAX)
	{
		free(data);
		return (NULL);
	}

	path = reconstruct(data, n, target);
	free(data);
	return (path);
}
