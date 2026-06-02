#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfinding.h"

/**
 * struct visited_node_s - Tracks a visited vertex and its path parent
 * @vertex: Pointer to the visited vertex
 * @parent: Parent vertex in the discovered path
 */
typedef struct visited_node_s
{
	vertex_t *vertex;
	vertex_t *parent;
} visited_node_t;

/**
 * is_visited - Check if a vertex has already been visited
 * @visited: Array of visited nodes
 * @vertex: The vertex to check
 * @count: Number of entries in visited
 *
 * Return: 1 if visited, 0 otherwise
 */
static int is_visited(visited_node_t *visited,
		vertex_t const *vertex, int count)
{
	int i;

	for (i = 0; i < count; i++)
	{
		if (visited[i].vertex == vertex)
			return (1);
	}
	return (0);
}

/**
 * get_parent - Retrieve the parent of a vertex in the path
 * @visited: Array of visited nodes
 * @vertex: The vertex whose parent to find
 * @count: Number of entries in visited
 *
 * Return: Parent vertex pointer, or NULL
 */
static vertex_t *get_parent(visited_node_t *visited,
		vertex_t const *vertex, int count)
{
	int i;

	for (i = 0; i < count; i++)
	{
		if (visited[i].vertex == vertex)
			return (visited[i].parent);
	}
	return (NULL);
}

/**
 * bt_recurse - Recursive backtracking helper for graph search
 * @current: Current vertex being explored
 * @target: Target vertex
 * @visited: Array of visited nodes
 * @count: Pointer to the number of visited entries
 * @capacity: Maximum capacity of visited array
 *
 * Return: 1 if path found, 0 otherwise
 */
static int bt_recurse(vertex_t const *current, vertex_t const *target,
		visited_node_t *visited, int *count, int capacity)
{
	edge_t *edge;
	int i;

	printf("Checking %s\n", current->content);

	if (*count >= capacity)
		return (0);
	visited[*count].vertex = (vertex_t *)current;
	visited[*count].parent = NULL;
	(*count)++;

	if (current == target)
		return (1);

	edge = current->edges;
	while (edge)
	{
		if (!is_visited(visited, edge->dest, *count))
		{
			if (bt_recurse(edge->dest, target, visited, count, capacity))
			{
				for (i = 0; i < *count; i++)
				{
					if (visited[i].vertex == edge->dest)
					{
						visited[i].parent = (vertex_t *)current;
						break;
					}
				}
				return (1);
			}
		}
		edge = edge->next;
	}
	return (0);
}

/**
 * build_path - Build queue from target back to start using parent links
 * @visited: Array of visited nodes
 * @count: Number of entries
 * @target: The target vertex
 *
 * Return: Queue containing the path, or NULL on failure
 */
static queue_t *build_path(visited_node_t *visited, int count,
		vertex_t const *target)
{
	queue_t *path;
	char **arr;
	vertex_t *cur;
	char *copy;
	int len, i;

	arr = malloc((count + 1) * sizeof(char *));
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
		cur = get_parent(visited, cur, count);
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
 * backtracking_graph - Find a path in a graph using backtracking
 * @graph: The graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path found
 */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
		vertex_t const *target)
{
	visited_node_t *visited;
	queue_t *path;
	int count;

	if (!graph || !start || !target)
		return (NULL);

	visited = malloc(graph->nb_vertices * sizeof(visited_node_t));
	if (!visited)
		return (NULL);

	count = 0;
	if (!bt_recurse(start, target, visited, &count,
			(int)graph->nb_vertices))
	{
		free(visited);
		return (NULL);
	}

	path = build_path(visited, count, target);
	free(visited);
	return (path);
}
