#include <stdlib.h>

#include "graphs.h"

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

size_t depth_first_traverse(const graph_t *graph,
                           void (*action)(const vertex_t *v, size_t depth))
{
    int *visited;
    vertex_t *start;
    size_t max = 0;
    size_t i;

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
