#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stdlib.h>

/* Include graphs.h if not already included */
#ifndef GRAPHS_H
# include "graphs.h"
#endif

/**
 * struct point_s - A point in a 2D space
 * @x: The x coordinate
 * @y: The y coordinate
 */
typedef struct point_s
{
	int x;
	int y;
} point_t;

/**
 * struct queue_node_s - Node in a queue
 * @content: Pointer to data
 * @next: Pointer to the next node
 */
typedef struct queue_node_s
{
	void *content;
	struct queue_node_s *next;
} queue_node_t;

/**
 * struct queue_s - Queue structure
 * @front: Pointer to the first node
 * @back: Pointer to the last node
 */
typedef struct queue_s
{
	queue_node_t *front;
	queue_node_t *back;
} queue_t;

/* Queue functions */
queue_t *queue_create(void);
int enqueue(queue_t *queue, void *content);
void *dequeue(queue_t *queue);
void queue_free(queue_t *queue);

/* Pathfinding functions */
queue_t *backtracking_array(char **map, int rows, int cols,
		point_t const *start, point_t const *target);
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
		vertex_t const *target);
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
		vertex_t const *target);

#endif /* PATHFINDING_H */
