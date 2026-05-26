#include <stdio.h>

#include "graphs.h"

void graph_display(const graph_t *graph)
{
    const vertex_t *v;
    const edge_t *e;

    if (!graph)
        return;

    printf("Number of vertices: %lu\n", graph->nb_vertices);
    v = graph->vertices;
    while (v)
    {
        printf("[%lu] %s", v->index, v->content ? v->content : "(null)");
        e = v->edges;
        while (e)
        {
            printf(" ->%lu", e->dest->index);
            e = e->next;
        }
        printf("\n");
        v = v->next;
    }
}
