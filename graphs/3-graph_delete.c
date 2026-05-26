#include <stdlib.h>

#include "graphs.h"

/**
 * graph_delete - free all memory associated with graph
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
