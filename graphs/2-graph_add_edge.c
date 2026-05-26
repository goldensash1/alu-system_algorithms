#include <stdlib.h>
#include <string.h>

#include "graphs.h"

/**
 * find_vertex_by_name - find a vertex by its content
 * @graph: pointer to the graph to search
 * @name: string to match against vertex->content
 *
 * Return: pointer to the vertex, or NULL if not found
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
 * add_edge_node - append an edge node from src to dest
 * @src: source vertex
 * @dest: destination vertex
 *
 * Return: 1 on success, 0 on failure
 */
static int add_edge_node(vertex_t *src, vertex_t *dest)
{
	edge_t *e = malloc(sizeof(edge_t));
	edge_t *cur;

	if (!e)
		return (0);

	e->dest = dest;
	e->next = NULL;
	if (!src->edges)
		src->edges = e;
	else
	{
		cur = src->edges;
		while (cur->next)
			cur = cur->next;
		cur->next = e;
	}
	src->nb_edges++;
	return (1);
}

/**
 * graph_add_edge - add edge(s) between two vertices identified by name
 * @graph: pointer to the graph
 * @src: name of source vertex
 * @dest: name of destination vertex
 * @type: edge type (UNIDIRECTIONAL or BIDIRECTIONAL)
 *
 * Return: 1 on success, 0 on failure
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

	/* add src -> dest */
	if (!add_edge_node(v_src, v_dest))
		return (0);

	/* if bidirectional, add dest -> src; rollback on failure */
	if (type == BIDIRECTIONAL)
	{
		if (!add_edge_node(v_dest, v_src))
		{
			/* rollback last edge added to v_src */
			edge_t *tmp = v_src->edges, *prev = NULL;
			if (!tmp)
				return (0);
			while (tmp->next)
			{
				prev = tmp;
				tmp = tmp->next;
			}
			if (prev)
				prev->next = NULL;
			else
				v_src->edges = NULL;
			free(tmp);
			v_src->nb_edges--;
			return (0);
		}
	}

	return (1);
}
