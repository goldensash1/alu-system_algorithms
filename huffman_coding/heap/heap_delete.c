#include <stdlib.h>

#include "heap.h"

/**
 * free_heap_nodes - recursively free nodes of a binary tree
 * @node: node to free
 * @free_data: function used to free node data, or NULL
 */
static void free_heap_nodes(binary_tree_node_t *node,
	void (*free_data)(void *))
{
	if (!node)
		return;

	free_heap_nodes(node->left, free_data);
	free_heap_nodes(node->right, free_data);
	if (free_data)
		free_data(node->data);
	free(node);
}

/**
 * heap_delete - delete a heap and all of its nodes
 * @heap: heap to delete
 * @free_data: function used to free node data, or NULL
 */
void heap_delete(heap_t *heap,
	void (*free_data)(void *))
{
	if (!heap)
		return;

	free_heap_nodes(heap->root, free_data);
	free(heap);
}
