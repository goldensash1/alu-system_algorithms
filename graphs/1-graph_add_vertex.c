#include <stdlib.h>
#include <string.h>

#include "graphs.h"

/**
 * vertex_exists - check whether a graph already contains a string
 * @graph: graph to search
 * @str: string to find
 *
 * Return: 1 if the string exists, otherwise 0
 */
static int vertex_exists(const graph_t *graph, const char *str)
{
	vertex_t *v;

	v = graph->vertices;
	while (v)
	{
		if (v->content && strcmp(v->content, str) == 0)
			return (1);
		v = v->next;
	}

	return (0);
}

/**
 * create_vertex - allocate and initialize a vertex
 * @graph: graph the vertex belongs to
 * @str: string content for the new vertex
 * @x: x coordinate
 * @y: y coordinate
 *
 * Return: pointer to the new vertex, or NULL on failure
 */
static vertex_t *create_vertex(graph_t *graph, const char *str, int x, int y)
{
	vertex_t *v;

	v = malloc(sizeof(vertex_t));
	if (!v)
		return (NULL);

	v->content = strdup(str);
	if (!v->content)
	{
		free(v);
		return (NULL);
	}

	v->x = x;
	v->y = y;
	v->nb_edges = 0;
	v->edges = NULL;
	v->next = NULL;
	v->index = graph->nb_vertices;
	return (v);
}

/**
 * append_vertex - append a vertex to the graph list
 * @graph: graph to update
 * @v: vertex to append
 */
static void append_vertex(graph_t *graph, vertex_t *v)
{
	vertex_t *last;

	if (!graph->vertices)
		graph->vertices = v;
	else
	{
		last = graph->vertices;
		while (last->next)
			last = last->next;
		last->next = v;
	}
}

/**
 * graph_add_vertex - add a vertex with a copy of str to graph
 * @graph: graph to add to
 * @str: string content for vertex
 * @x: x coordinate
 * @y: y coordinate
 *
 * Return: pointer to created vertex or NULL on failure
 */
vertex_t *graph_add_vertex(graph_t *graph, const char *str, int x, int y)
{
	vertex_t *v;

	if (!graph || !str)
		return (NULL);

	if (vertex_exists(graph, str))
		return (NULL);

	v = create_vertex(graph, str, x, y);
	if (!v)
		return (NULL);

	append_vertex(graph, v);

	graph->nb_vertices++;

	return (v);
}
