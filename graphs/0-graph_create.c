#include <stdlib.h>

#include "graphs.h"

/**
 * graph_create - allocate and initialize a graph_t
 * Return: pointer to allocated graph, or NULL on failure
 */
graph_t *graph_create(void)
{
	graph_t *graph = malloc(sizeof(graph_t));

	if (!graph)
		return (NULL);

	graph->nb_vertices = 0;
	graph->vertices = NULL;

	return (graph);
}
