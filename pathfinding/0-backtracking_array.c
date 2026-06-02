#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfinding.h"

/**
 * enqueue - Add an element to the back of a queue
 * @queue: Pointer to the queue
 * @content: Data to add
 *
 * Return: 1 on success, 0 on failure
 */
int enqueue(queue_t *queue, void *content)
{
	queue_node_t *node;

	if (!queue)
		return (0);
	node = malloc(sizeof(queue_node_t));
	if (!node)
		return (0);
	node->content = content;
	node->next = NULL;
	if (!queue->back)
	{
		queue->front = node;
		queue->back = node;
	}
	else
	{
		queue->back->next = node;
		queue->back = node;
	}
	return (1);
}

/**
 * dequeue - Remove and return the front element of a queue
 * @queue: Pointer to the queue
 *
 * Return: Content pointer, or NULL if empty
 */
void *dequeue(queue_t *queue)
{
	queue_node_t *node;
	void *content;

	if (!queue || !queue->front)
		return (NULL);
	node = queue->front;
	content = node->content;
	queue->front = node->next;
	if (!queue->front)
		queue->back = NULL;
	free(node);
	return (content);
}

/**
 * alloc_visited - Allocate and zero-initialise a 2D visited array
 * @rows: Number of rows
 * @cols: Number of columns
 *
 * Return: Allocated array, or NULL on failure
 */
static int **alloc_visited(int rows, int cols)
{
	int **visited;
	int i;

	visited = malloc(rows * sizeof(int *));
	if (!visited)
		return (NULL);
	for (i = 0; i < rows; i++)
	{
		visited[i] = malloc(cols * sizeof(int));
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
		memset(visited[i], 0, cols * sizeof(int));
	}
	return (visited);
}

/**
 * backtrack - Recursive backtracking helper
 * @map: The map
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current position
 * @target: Target position
 * @visited: Visited tracking array
 * @path: Queue to store final path
 * @stack: Path stack for current branch
 * @depth: Current recursion depth
 *
 * Return: 1 if path found, 0 otherwise
 */
static int backtrack(char **map, int rows, int cols, point_t current,
		point_t const *target, int **visited, queue_t *path,
		point_t *stack, int depth)
{
	int dirs[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int i, nx, ny;
	point_t next;
	point_t *pt;

	printf("Checking coordinates [%d, %d]\n", current.x, current.y);
	visited[current.y][current.x] = 1;
	stack[depth] = current;
	if (current.x == target->x && current.y == target->y)
	{
		for (i = 0; i <= depth; i++)
		{
			pt = malloc(sizeof(point_t));
			if (!pt)
				return (0);
			*pt = stack[i];
			enqueue(path, pt);
		}
		return (1);
	}
	for (i = 0; i < 4; i++)
	{
		nx = current.x + dirs[i][0];
		ny = current.y + dirs[i][1];
		if (nx >= 0 && nx < cols && ny >= 0 && ny < rows
				&& map[ny][nx] != '1' && !visited[ny][nx])
		{
			next.x = nx;
			next.y = ny;
			if (backtrack(map, rows, cols, next, target,
					visited, path, stack, depth + 1))
				return (1);
		}
	}
	return (0);
}

/**
 * backtracking_array - Find path in a 2D array using backtracking
 * @map: 2D array where '0' is walkable and '1' is blocked
 * @rows: Number of rows
 * @cols: Number of columns
 * @start: Starting point
 * @target: Target point
 *
 * Return: Queue containing the path, or NULL if no path found
 */
queue_t *backtracking_array(char **map, int rows, int cols,
		point_t const *start, point_t const *target)
{
	queue_t *path;
	int **visited;
	point_t *stack;
	void *content;
	int i;

	if (!map || rows <= 0 || cols <= 0 || !start || !target)
		return (NULL);
	visited = alloc_visited(rows, cols);
	if (!visited)
		return (NULL);
	stack = malloc(rows * cols * sizeof(point_t));
	if (!stack)
	{
		for (i = 0; i < rows; i++)
			free(visited[i]);
		free(visited);
		return (NULL);
	}
	path = calloc(1, sizeof(queue_t));
	if (!path)
	{
		free(stack);
		for (i = 0; i < rows; i++)
			free(visited[i]);
		free(visited);
		return (NULL);
	}
	if (!backtrack(map, rows, cols, *start, target, visited, path, stack, 0))
	{
		while ((content = dequeue(path)))
			free(content);
		free(path);
		path = NULL;
	}
	free(stack);
	for (i = 0; i < rows; i++)
		free(visited[i]);
	free(visited);
	return (path);
}
