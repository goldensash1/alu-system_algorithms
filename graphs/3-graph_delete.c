#include <stdlib.h>

#include "graphs.h"

/**
 * graph_delete - delete a graph and free all its memory
 * @graph: pointer to the graph to delete
 *
 * Description: Frees all edges, vertices and finally the graph structure.
 */
void graph_delete(graph_t *graph)
{
	vertex_t *v, *next_v;
	edge_t *e, *next_e;

	if (!graph)
		return;

	v = graph->vertices;
	while (v)
	{
		next_v = v->next;
		e = v->edges;
		while (e)
		{
			next_e = e->next;
			free(e);
			e = next_e;
		}
		if (v->content)
			free(v->content);
		free(v);
		v = next_v;
	}

	free(graph);
}
