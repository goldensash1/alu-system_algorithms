#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
 * relax_edges - Update neighbour distances from the current vertex
 * @data: The data array
 * @n: Number of entries
 * @cur_idx: Index of the current vertex in data
 */
static void relax_edges(dijk_node_t *data, int n, int cur_idx)
{
	edge_t *edge;
	int new_dist, dest_idx;

	edge = data[cur_idx].vertex->edges;
	while (edge)
	{
		for (dest_idx = 0; dest_idx < n; dest_idx++)
			if (data[dest_idx].vertex == edge->dest)
				break;
		if (dest_idx < n && !data[dest_idx].visited)
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
 * reconstruct - Build result queue tracing previous links from target to start
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
		j = 0;
		while (j < n && data[j].vertex != cur)
			j++;
		cur = j < n ? data[j].previous : NULL;
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
	vertex_t *v;
	queue_t *path = NULL;
	int n, i, j, min_idx;

	if (!graph || !start || !target)
		return (NULL);
	n = (int)graph->nb_vertices;
	data = malloc(n * sizeof(dijk_node_t));
	if (!data)
		return (NULL);
	for (v = graph->vertices, i = 0; v && i < n; v = v->next, i++)
	{
		data[i].vertex = v;
		data[i].distance = (v == start) ? 0 : INT_MAX;
		data[i].previous = NULL;
		data[i].visited = 0;
	}
	for (i = 0; i < n; i++)
	{
		min_idx = -1;
		for (j = 0; j < n; j++)
			if (!data[j].visited && data[j].distance != INT_MAX
					&& (min_idx < 0 || data[j].distance < data[min_idx].distance))
				min_idx = j;
		if (min_idx == -1)
			break;
		data[min_idx].visited = 1;
		printf("Checking %s, distance from %s is %d\n",
			data[min_idx].vertex->content, start->content,
			data[min_idx].distance);
		relax_edges(data, n, min_idx);
	}
	j = 0;
	while (j < n && data[j].vertex != target)
		j++;
	if (j < n && data[j].distance != INT_MAX)
		path = reconstruct(data, n, target);
	free(data);
	return (path);
}
