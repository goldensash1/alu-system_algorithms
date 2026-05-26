#include <stdlib.h>

#include "graphs.h"

/**
 * dfs_visit - recursive helper for depth-first traversal
 * @graph: pointer to the graph (kept for signature compatibility)
 * @v: current vertex to visit
 * @visited: array of visited flags indexed by vertex->index
 * @depth: current depth from the start vertex
 * @action: callback to run for each visited vertex
 *
 * Return: maximum depth found in this branch
 */
static size_t dfs_visit(const graph_t *graph, const vertex_t *v,
	int *visited, size_t depth,
	void (*action)(const vertex_t *v, size_t depth))
{
	edge_t *e;
	size_t max = depth, tmp;

	if (!v)
		return (0);

	visited[v->index] = 1;
	action(v, depth);

	e = v->edges;
	while (e)
	{
		if (!visited[e->dest->index])
		{
			tmp = dfs_visit(graph, e->dest, visited, depth + 1, action);
			if (tmp > max)
				max = tmp;
		}
		e = e->next;
	}
	return (max);
}

/**
 * depth_first_traverse - traverse graph in depth-first order
 * @graph: pointer to the graph to traverse
 * @action: function called for each visited vertex
 *
 * Return: biggest vertex depth, or 0 on failure
 */
size_t depth_first_traverse(const graph_t *graph,
	void (*action)(const vertex_t *v, size_t depth))
{
	int *visited;
	vertex_t *start;
	size_t max = 0;

	if (!graph || !action || graph->nb_vertices == 0)
		return (0);

	visited = calloc(graph->nb_vertices, sizeof(int));
	if (!visited)
		return (0);

	start = graph->vertices;
	if (!start)
	{
		free(visited);
		return (0);
	}

	max = dfs_visit(graph, start, visited, 0, action);

	free(visited);
	return (max);
}
