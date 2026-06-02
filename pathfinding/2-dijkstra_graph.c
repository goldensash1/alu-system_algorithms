#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graphs.h"
#include "pathfinding.h"

/**
 * struct dijkstra_data_s - Data structure for Dijkstra's algorithm
 * @vertex: Pointer to the vertex
 * @distance: Distance from start
 * @previous: Previous vertex in path
 * @visited: Whether this vertex has been finalized
 */
typedef struct dijkstra_data_s
{
    vertex_t *vertex;
    int distance;
    vertex_t *previous;
    int visited;
} dijkstra_data_t;

/**
 * dijkstra_graph - Find shortest path using Dijkstra's algorithm
 * @graph: The graph to search
 * @start: Starting vertex
 * @target: Target vertex
 *
 * Return: Queue containing the path, or NULL if no path found
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
                        vertex_t const *target)
{
    dijkstra_data_t *data;
    queue_t *path;
    vertex_t *current;
    edge_t *edge;
    int i, j;
    int min_distance;
    int min_index;
    int current_index;

    if (!graph || !start || !target)
        return (NULL);

    /* Allocate and initialize data array */
    data = malloc(graph->nb_vertices * sizeof(dijkstra_data_t));
    if (!data)
        return (NULL);

    current = graph->vertices;
    i = 0;
    while (current && i < (int)graph->nb_vertices)
    {
        data[i].vertex = current;
        data[i].distance = (current == start) ? 0 : INT_MAX;
        data[i].previous = NULL;
        data[i].visited = 0;
        current = current->next;
        i++;
    }

    /* Main Dijkstra loop */
    for (i = 0; i < (int)graph->nb_vertices; i++)
    {
        /* Find unvisited vertex with minimum distance */
        min_distance = INT_MAX;
        min_index = -1;

        for (j = 0; j < (int)graph->nb_vertices; j++)
        {
            if (!data[j].visited && data[j].distance < min_distance)
            {
                min_distance = data[j].distance;
                min_index = j;
            }
        }

        if (min_index == -1 || min_distance == INT_MAX)
            break;

        /* Process the vertex with minimum distance */
        data[min_index].visited = 1;
        current = data[min_index].vertex;
        printf("Checking %s, distance from %s is %d\n", current->content,
               start->content, data[min_index].distance);

        /* Update distances to neighbors */
        edge = current->edges;
        while (edge)
        {
            /* Find the index of the destination vertex */
            current_index = -1;
            for (j = 0; j < (int)graph->nb_vertices; j++)
            {
                if (data[j].vertex == edge->dest)
                {
                    current_index = j;
                    break;
                }
            }

            if (current_index != -1 && !data[current_index].visited)
            {
                int new_distance = data[min_index].distance + edge->weight;
                if (new_distance < data[current_index].distance)
                {
                    data[current_index].distance = new_distance;
                    data[current_index].previous = current;
                }
            }
            edge = edge->next;
        }
    }

    /* Find target vertex */
    int target_index = -1;
    for (i = 0; i < (int)graph->nb_vertices; i++)
    {
        if (data[i].vertex == target)
        {
            target_index = i;
            break;
        }
    }

    /* Check if path exists */
    if (target_index == -1 || data[target_index].distance == INT_MAX)
    {
        free(data);
        return (NULL);
    }

    /* Reconstruct path */
    path = queue_create();
    if (!path)
    {
        free(data);
        return (NULL);
    }

    /* Build path from target to start, collect in array */
    char **path_vertices = malloc((graph->nb_vertices + 1) * sizeof(char *));
    if (!path_vertices)
    {
        free(path);
        free(data);
        return (NULL);
    }

    current = (vertex_t *)target;
    int path_length = 0;
    while (current)
    {
        char *city_copy = strdup(current->content);
        if (!city_copy)
        {
            int k;
            for (k = 0; k < path_length; k++)
                free(path_vertices[k]);
            free(path_vertices);
            void *content;
            while ((content = dequeue(path)))
                free(content);
            free(path);
            free(data);
            return (NULL);
        }
        path_vertices[path_length++] = city_copy;

        /* Find next vertex in path */
        vertex_t *prev = NULL;
        for (j = 0; j < (int)graph->nb_vertices; j++)
        {
            if (data[j].vertex == current)
            {
                prev = data[j].previous;
                break;
            }
        }
        current = prev;
    }

    /* Enqueue in reverse order (from start to target) */
    for (i = path_length - 1; i >= 0; i--)
    {
        enqueue(path, path_vertices[i]);
    }

    free(path_vertices);
    free(data);
    return (path);
}
