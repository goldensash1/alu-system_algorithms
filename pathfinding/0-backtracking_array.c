#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfinding.h"

/**
 * is_valid - Check if a cell is within bounds and walkable
 * @map: The map
 * @rows: Number of rows
 * @cols: Number of columns
 * @x: X coordinate (column)
 * @y: Y coordinate (row)
 * @visited: 2D array tracking visited cells
 *
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid(char **map, int rows, int cols,
		int x, int y, int **visited)
{
	if (x < 0 || x >= cols || y < 0 || y >= rows)
		return (0);
	if (map[y][x] == '1')
		return (0);
	if (visited[y][x])
		return (0);
	return (1);
}

/**
 * backtrack - Recursive backtracking helper
 * @map: The map
 * @rows: Number of rows
 * @cols: Number of columns
 * @current: Current position
 * @target: Target position
 * @visited: 2D array tracking visited cells
 * @path: Queue to store the final path
 * @stack: Array used as a path stack
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
		if (is_valid(map, rows, cols, nx, ny, visited))
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
 * free_visited - Free the visited 2D array
 * @visited: The visited array
 * @rows: Number of rows
 */
static void free_visited(int **visited, int rows)
{
	int i;

	for (i = 0; i < rows; i++)
		free(visited[i]);
	free(visited);
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

	stack = malloc(rows * cols * sizeof(point_t));
	if (!stack)
	{
		free_visited(visited, rows);
		return (NULL);
	}

	path = queue_create();
	if (!path)
	{
		free(stack);
		free_visited(visited, rows);
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
	free_visited(visited, rows);
	return (path);
}
