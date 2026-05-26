#include <stdlib.h>
#include <string.h>

#include "graphs.h"

/**
 * graph_add_vertex - add a vertex with a copy of str to graph
 * @graph: graph to add to
 * @str: string content for vertex
 * Return: pointer to created vertex or NULL on failure
 */
vertex_t *graph_add_vertex(graph_t *graph, const char *str)
{
    vertex_t *v, *last;
    char *copy;

    if (!graph || !str)
        return (NULL);

    /* check for duplicate */
    v = graph->vertices;
    while (v)
    {
        if (v->content && strcmp(v->content, str) == 0)
            return (NULL);
        v = v->next;
    }

    copy = strdup(str);
    if (!copy)
        return (NULL);

    v = malloc(sizeof(vertex_t));
    if (!v)
    {
        free(copy);
        return (NULL);
    }

    v->content = copy;
    v->nb_edges = 0;
    v->edges = NULL;
    v->next = NULL;
    v->index = graph->nb_vertices;

    if (!graph->vertices)
    {
        graph->vertices = v;
    }
    else
    {
        last = graph->vertices;
        while (last->next)
            last = last->next;
        last->next = v;
    }

    graph->nb_vertices++;

    return (v);
}
