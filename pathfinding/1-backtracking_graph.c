#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfinding.h"

/**
 * struct visited_info_s - Info about visited vertices
 * @vertex: Pointer to the vertex
 * @parent: Parent vertex in the path
 */
typedef struct visited_info_s
{
    vertex_t *vertex;
    vertex_t *parent;
} visited_info_t;

/**
 * is_visited - Check if a vertex has been visited
 * @visited: Array of visited info
 * @vertex: The vertex to check
 * @count: Number of visited vertices
 *
 * Return: 1 if visited, 0 otherwise
 */
static int is_visited(visited_info_t *visited, vertex_t const *vertex, int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (visited[i].vertex == vertex)
            return (1);
    }
    return (0);
}

/**
 * get_parent - Get parent of a vertex
 * @visited: Array of visited info
 * @vertex: The vertex
 * @count: Number of visited vertices
 *
 * Return: Parent vertex or NULL
 */
static vertex_t *get_parent(visited_info_t *visited, vertex_t const *vertex,
                             int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (visited[i].vertex == vertex)
            return (visited[i].parent);
    }
    return (NULL);
}

/**
 * backtrack - Recursive backtracking helper for graph
 * @current: Current vertex
 * @target: Target vertex
 * @visited: Array of visited info
 * @count: Pointer to number of visited vertices
 * @capacity: Capacity of visited array
 *
 * Return: 1 if path found, 0 otherwise
 */
static int backtrack(vertex_t const *current, vertex_t const *target,
                     visited_info_t *visited, int *count, int capacity)
{
    edge_t *edge;

    printf("Checking %s\n", current->content);

    /* Add current to visited */
    if (*count >= capacity)
        return (0);
    visited[*count].vertex = (vertex_t *)current;
    visited[*count].parent = NULL;
    (*count)++;

    /* Check if we reached target */
    if (current == target)
        return (1);

    /* Explore neighbors */
    edge = current->edges;
    while (edge)
    {
        if (!is_visited(visited, edge->dest, *count))
        {
            if (backtrack(edge->dest, target, visited, count, capacity))
            {
                /* Mark the parent for path reconstruction */
                int i;
                for (i = 0; i < *count; i++)
                {
                    if (visited[i].vertex == edge->dest)
                    {
                        visited[i].parent = (vertex_t *)current;
                        break;
                    }
                }
                return (1);
            }
        }
        edge = edge->next;
    }

    return (0);
}

/**
 * backtracking_graph - Find a path in a graph using backtracking
 * @graph: The graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path found
 */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
                            vertex_t const *target)
{
    queue_t *path;
    visited_info_t *visited;
    int count = 0;
    vertex_t *current;
    char *city_copy;

    if (!graph || !start || !target)
        return (NULL);

    /* Allocate visited array */
    visited = malloc(graph->nb_vertices * sizeof(visited_info_t));
    if (!visited)
        return (NULL);

    /* Perform backtracking */
    if (!backtrack(start, target, visited, &count,
                   (int)graph->nb_vertices))
    {
        /* No path found */
        free(visited);
        return (NULL);
    }

    /* Create path queue from visited info */
    path = queue_create();
    if (!path)
    {
        free(visited);
        return (NULL);
    }

    /* Build path by tracing from target to start, collect in array, then reverse */
    char **path_vertices = malloc((count + 1) * sizeof(char *));
    if (!path_vertices)
    {
        free(path);
        free(visited);
        return (NULL);
    }

    /* Reconstruct path from target to start */
    current = (vertex_t *)target;
    int path_length = 0;
    while (current)
    {
        city_copy = strdup(current->content);
        if (!city_copy)
        {
            int i;
            for (i = 0; i < path_length; i++)
                free(path_vertices[i]);
            free(path_vertices);
            free(path);
            free(visited);
            return (NULL);
        }
        path_vertices[path_length++] = city_copy;
        current = get_parent(visited, current, count);
    }

    /* Enqueue in reverse order (from start to target) */
    int i;
    for (i = path_length - 1; i >= 0; i--)
    {
        enqueue(path, path_vertices[i]);
    }

    free(path_vertices);
    free(visited);
    return (path);
}
