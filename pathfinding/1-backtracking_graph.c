#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphs.h"
#include "pathfinding.h"

/**
 * enqueue - Add an element to the back of a queue
 * @queue: Pointer to the queue
 * @content: Data to add
 *
 * Return: 1 on success, 0 on failure
 */
int enqueue(queue_t *queue, void *content)
{
	queue_node_t *node;

	if (!queue)
		return (0);
	node = malloc(sizeof(queue_node_t));
	if (!node)
		return (0);
	node->content = content;
	node->next = NULL;
	if (!queue->back)
	{
		queue->front = node;
		queue->back = node;
	}
	else
	{
		queue->back->next = node;
		queue->back = node;
	}
	return (1);
}

/**
 * dequeue - Remove and return the front element of a queue
 * @queue: Pointer to the queue
 *
 * Return: Content pointer, or NULL if empty
 */
void *dequeue(queue_t *queue)
{
	queue_node_t *node;
	void *content;

	if (!queue || !queue->front)
		return (NULL);
	node = queue->front;
	content = node->content;
	queue->front = node->next;
	if (!queue->front)
		queue->back = NULL;
	free(node);
	return (content);
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
		for (i = 0; i < *count && visited[i].vertex != edge->dest; i++)
			;
		if (i == *count)
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
 * build_path - Build result queue tracing parent links from target to start
 * @visited: Array of visited nodes
 * @count: Number of entries
 * @target: The target vertex
 *
 * Return: Queue containing path start→target, or NULL on failure
 */
static queue_t *build_path(visited_node_t *visited, int count,
		vertex_t const *target)
{
	queue_t *path;
	char **arr;
	vertex_t *cur;
	char *copy;
	int len, i, j;

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
		j = 0;
		while (j < count && visited[j].vertex != cur)
			j++;
		cur = j < count ? visited[j].parent : NULL;
	}
	path = calloc(1, sizeof(queue_t));
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
