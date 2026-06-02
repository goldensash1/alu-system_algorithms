#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathfinding.h"

/**
 * is_valid - Check if a cell is valid and walkable
 * @map: The map
 * @rows: Number of rows
 * @cols: Number of columns
 * @x: X coordinate (column)
 * @y: Y coordinate (row)
 * @visited: 2D array tracking visited cells
 *
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid(char **map, int rows, int cols, int x, int y, int **visited)
{
    if (x < 0 || x >= cols || y < 0 || y >= rows)
        return (0);
    if (map[y][x] == '1')  /* Wall */
        return (0);
    if (visited[y][x])  /* Already visited */
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
 * @path: Queue storing the path
 *
 * Return: 1 if path found, 0 otherwise
 */
static int backtrack(char **map, int rows, int cols, point_t current,
                     point_t const *target, int **visited, queue_t *path)
{
    point_t *point;

    printf("Checking coordinates [%d, %d]\n", current.x, current.y);
    visited[current.y][current.x] = 1;
    
    /* Allocate and enqueue the current point */
    point = malloc(sizeof(point_t));
    if (point)
    {
        point->x = current.x;
        point->y = current.y;
        enqueue(path, point);
    }

    /* Check if we reached the target */
    if (current.x == target->x && current.y == target->y)
        return (1);

    /* Direction order: RIGHT, BOTTOM, LEFT, TOP */
    /* x increases rightward (column), y increases downward (row) */
    int directions[4][2] = {
        {1, 0},   /* RIGHT - x increases */
        {0, 1},   /* BOTTOM - y increases */
        {-1, 0},  /* LEFT - x decreases */
        {0, -1}   /* TOP - y decreases */
    };

    for (int i = 0; i < 4; i++)
    {
        int new_x = current.x + directions[i][0];
        int new_y = current.y + directions[i][1];

        if (is_valid(map, rows, cols, new_x, new_y, visited))
        {
            point_t next = {new_x, new_y};
            if (backtrack(map, rows, cols, next, target, visited, path))
                return (1);
        }
    }

    return (0);
}

/**
 * backtracking_array - Find a path in a 2D array using backtracking
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
    int i;

    if (!map || rows <= 0 || cols <= 0 || !start || !target)
        return (NULL);

    /* Allocate visited array */
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

    /* Create path queue */
    path = queue_create();
    if (!path)
    {
        for (i = 0; i < rows; i++)
            free(visited[i]);
        free(visited);
        return (NULL);
    }

    /* Perform backtracking */
    if (!backtrack(map, rows, cols, *start, target, visited, path))
    {
        /* No path found, free the queue and return NULL */
        void *content;
        while ((content = dequeue(path)))
            free(content);
        free(path);
        path = NULL;
    }

    /* Free visited array */
    for (i = 0; i < rows; i++)
        free(visited[i]);
    free(visited);

    return (path);
}
