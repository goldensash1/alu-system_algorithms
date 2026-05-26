#include <stdlib.h>
#include <string.h>

#include "graphs.h"

/**
 * find_vertex_by_name - helper to find vertex by content
 */
static vertex_t *find_vertex_by_name(const graph_t *graph, const char *name)
{
    vertex_t *v;

    if (!graph || !name)
        return (NULL);

    v = graph->vertices;
    while (v)
    {
        if (v->content && strcmp(v->content, name) == 0)
            return (v);
        v = v->next;
    }
    return (NULL);
}

/**
 * add_edge_node - add an edge node from src to dest vertex
 * Returns 1 on success, 0 on failure
 */
static int add_edge_node(vertex_t *src, vertex_t *dest)
{
    edge_t *e = malloc(sizeof(edge_t));

    if (!e)
        return (0);
    e->dest = dest;
    e->next = src->edges;
    src->edges = e;
    src->nb_edges++;
    return (1);
}

/**
 * graph_add_edge - add edge(s) between two vertices identified by name
 */
int graph_add_edge(graph_t *graph, const char *src, const char *dest, edge_type_t type)
{
    vertex_t *v_src, *v_dest;

    if (!graph || !src || !dest)
        return (0);

    v_src = find_vertex_by_name(graph, src);
    v_dest = find_vertex_by_name(graph, dest);
    if (!v_src || !v_dest)
        return (0);

    if (!add_edge_node(v_src, v_dest))
        return (0);

    if (type == BIDIRECTIONAL)
    {
        if (!add_edge_node(v_dest, v_src))
        {
            /* rollback */
            edge_t *tmp = v_src->edges;
            v_src->edges = tmp->next;
            free(tmp);
            v_src->nb_edges--;
            return (0);
        }
    }

    return (1);
}
