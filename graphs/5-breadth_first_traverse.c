#include <stdlib.h>

#include "graphs.h"

/**
 * free_bfs_buffers - free memory used by breadth-first traversal
 * @visited: visited flags array
 * @depths: depths array
 * @queue: queue array
 * @map: index-to-vertex map
 */
static void free_bfs_buffers(int *visited, size_t *depths, int *queue,
	vertex_t **map)
{
	free(visited);
	free(depths);
	free(queue);
	free(map);
}

/**
 * build_index_map - build an array that maps indices to vertices
 * @graph: graph to map
 * @map: array of vertex pointers indexed by vertex index
 * @n: number of vertices in the graph
 */
static void build_index_map(const graph_t *graph, vertex_t **map, size_t n)
{
	vertex_t *v;

	v = graph->vertices;
	while (v)
	{
		if (v->index < n)
			map[v->index] = v;
		v = v->next;
	}
}

/**
 * init_bfs_state - initialize BFS arrays and seed the first vertex
 * @graph: graph to traverse
 * @visited: visited flags array
 * @depths: depths array
 * @queue: queue array
 * @map: index-to-vertex map
 * @tail: queue tail index
 *
 * Return: 1 on success, 0 on failure
 */
static int init_bfs_state(const graph_t *graph, int *visited, size_t *depths,
	int *queue, vertex_t **map, size_t *tail)
{
	size_t i;

	build_index_map(graph, map, graph->nb_vertices);
	for (i = 0; i < graph->nb_vertices; i++)
		depths[i] = (size_t)-1;

	if (!map[0])
		return (0);

	queue[(*tail)++] = (int)map[0]->index;
	visited[map[0]->index] = 1;
	depths[map[0]->index] = 0;
	return (1);
}

/**
 * bfs_visit - process queued vertices in breadth-first order
 * @action: function called for each visited vertex
 * @visited: visited flags array
 * @depths: depths array
 * @queue: queue array
 * @map: index-to-vertex map
 * @head: queue head index
 * @tail: queue tail index
 *
 * Return: biggest vertex depth found
 */
static size_t bfs_visit(void (*action)(const vertex_t *v, size_t depth),
	int *visited, size_t *depths, int *queue, vertex_t **map,
	size_t head, size_t tail)
{
	size_t max;
	vertex_t *current;
	edge_t *edge;

	max = 0;
	while (head < tail)
	{
		current = map[queue[head++]];
		action(current, depths[current->index]);
		if (depths[current->index] > max)
			max = depths[current->index];
		edge = current->edges;
		while (edge)
		{
			if (!visited[edge->dest->index])
			{
				visited[edge->dest->index] = 1;
				depths[edge->dest->index] = depths[current->index] + 1;
				queue[tail++] = (int)edge->dest->index;
			}
			edge = edge->next;
		}
	}
	return (max);
}

/**
 * breadth_first_traverse - traverse graph in breadth-first order
 * @graph: pointer to the graph to traverse
 * @action: function called for each visited vertex
 *
 * Return: biggest vertex depth, or 0 on failure
 */
size_t breadth_first_traverse(const graph_t *graph,
	void (*action)(const vertex_t *v, size_t depth))
{
	size_t n, tail, result;
	int *visited;
	size_t *depths;
	int *queue;
	vertex_t **map;

	if (!graph || !action || graph->nb_vertices == 0)
		return (0);

	n = graph->nb_vertices;
	tail = 0;
	result = 0;
	visited = calloc(n, sizeof(int));
	depths = malloc(n * sizeof(size_t));
	queue = malloc(n * sizeof(int));
	map = malloc(n * sizeof(vertex_t *));
	if (!visited || !depths || !queue || !map)
	{
		free_bfs_buffers(visited, depths, queue, map);
		return (0);
	}

	if (!init_bfs_state(graph, visited, depths, queue, map, &tail))
	{
		free_bfs_buffers(visited, depths, queue, map);
		return (0);
	}

	result = bfs_visit(action, visited, depths, queue, map, 0, tail);
	free_bfs_buffers(visited, depths, queue, map);
	return (result);
}
