#include <stdlib.h>
#include <limits.h>

#include "graphs.h"

size_t breadth_first_traverse(const graph_t *graph,
                             void (*action)(const vertex_t *v, size_t depth))
{
    size_t n, head = 0, tail = 0, max = 0;
    int *visited;
    size_t *depths;
    int *queue;
    vertex_t **map;
    vertex_t *v;
    edge_t *e;
    size_t i;

    if (!graph || !action || graph->nb_vertices == 0)
        return (0);

    n = graph->nb_vertices;
    visited = calloc(n, sizeof(int));
    depths = malloc(n * sizeof(size_t));
    queue = malloc(n * sizeof(int));
    map = malloc(n * sizeof(vertex_t *));
    if (!visited || !depths || !queue || !map)
        goto fail;

    /* build index->vertex map */
    v = graph->vertices;
    while (v)
    {
        if (v->index < n)
            map[v->index] = v;
        v = v->next;
    }

    for (i = 0; i < n; i++)
        depths[i] = (size_t)-1;

    /* enqueue start vertex */
    queue[tail++] = (int)map[0]->index;
    visited[map[0]->index] = 1;
    depths[map[0]->index] = 0;

    while (head < tail)
    {
        int cur = queue[head++];
        vertex_t *cv = map[cur];

        action(cv, depths[cur]);
        if (depths[cur] > max)
            max = depths[cur];

        e = cv->edges;
        while (e)
        {
            if (!visited[e->dest->index])
            {
                visited[e->dest->index] = 1;
                depths[e->dest->index] = depths[cur] + 1;
                queue[tail++] = (int)e->dest->index;
            }
            e = e->next;
        }
    }

    free(visited);
    free(depths);
    free(queue);
    free(map);
    return (max);

fail:
    free(visited);
    free(depths);
    free(queue);
    free(map);
    return (0);
}
