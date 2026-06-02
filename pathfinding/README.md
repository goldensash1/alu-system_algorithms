# Pathfinding Algorithms

This directory contains implementations of three popular pathfinding algorithms: Backtracking on Arrays, Backtracking on Graphs, and Dijkstra's algorithm.

## Files

- `pathfinding.h` - Header file with data structures and function prototypes
- `queue.c` - Queue implementation for storing paths
- `0-backtracking_array.c` - Backtracking algorithm for 2D arrays
- `1-backtracking_graph.c` - Backtracking algorithm for graphs
- `2-dijkstra_graph.c` - Dijkstra's shortest path algorithm for graphs

## Algorithms

### 0. Backtracking - Array
Searches for the first path from a starting point to a target point within a two-dimensional array using recursive backtracking.

**Function:** `queue_t *backtracking_array(char **map, int rows, int cols, point_t const *start, point_t const *target);`

**Parameters:**
- `map`: 2D array where '0' is walkable and '1' is blocked
- `rows`: Number of rows
- `cols`: Number of columns
- `start`: Starting point coordinates
- `target`: Target point coordinates

**Returns:** Queue containing the path from start to target, or NULL if no path exists

**Neighbor exploration order:** RIGHT, BOTTOM, LEFT, TOP

### 1. Backtracking - Graph
Searches for the first path from a starting vertex to a target vertex in a graph using recursive backtracking.

**Function:** `queue_t *backtracking_graph(graph_t *graph, vertex_t const *start, vertex_t const *target);`

**Parameters:**
- `graph`: The graph to search
- `start`: Starting vertex
- `target`: Target vertex

**Returns:** Queue containing vertex names forming a path from start to target, or NULL if no path exists

### 2. Dijkstra's Algorithm - Graph
Finds the shortest path from a starting vertex to a target vertex in a weighted graph using Dijkstra's algorithm.

**Function:** `queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start, vertex_t const *target);`

**Parameters:**
- `graph`: The graph to search (edges must have weights)
- `start`: Starting vertex
- `target`: Target vertex

**Returns:** Queue containing vertex names forming the shortest path from start to target, or NULL if no path exists

## Data Structures

### point_t
```c
typedef struct point_s {
    int x;  // Column coordinate
    int y;  // Row coordinate
} point_t;
```

### queue_t
```c
typedef struct queue_s {
    queue_node_t *front;
    queue_node_t *back;
} queue_t;
```

## Notes

- The backtracking_array explores neighbors in order: RIGHT (x+1), BOTTOM (y+1), LEFT (x-1), TOP (y-1)
- Backtracking finds the first working path, not necessarily the shortest
- Dijkstra's algorithm guarantees the shortest path in terms of edge weights
- All algorithms print progress information during execution

## Compilation

To compile all pathfinding modules:
```bash
gcc -Wall -Wextra -Werror -pedantic -o program main.c 0-backtracking_array.c 1-backtracking_graph.c 2-dijkstra_graph.c queue.c ../graphs/*.c
```

## Performance

- Backtracking Array: O(rows × cols × 4) in worst case (explores all cells)
- Backtracking Graph: O(V + E) in worst case (DFS)
- Dijkstra's Graph: O(V²) using simple array-based minimum selection

Where V is number of vertices and E is number of edges.
