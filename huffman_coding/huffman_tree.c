#include <stdlib.h>

#include "huffman.h"

/**
 * free_tree - free a Huffman tree recursively
 * @node: tree node to free
 */
static void free_tree(binary_tree_node_t *node)
{
	if (!node)
		return;

	free_tree(node->left);
	free_tree(node->right);
	free(node->data);
	free(node);
}

/**
 * huffman_tree - build a Huffman tree from symbols and frequencies
 * @data: array of characters
 * @freq: array of associated frequencies
 * @size: number of elements in both arrays
 *
 * Return: pointer to the root of the Huffman tree, or NULL on failure
 */
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size)
{
	heap_t *priority_queue;
	binary_tree_node_t *root;

	if (!data || !freq || size == 0)
		return (NULL);

	priority_queue = huffman_priority_queue(data, freq, size);
	if (!priority_queue)
		return (NULL);

	while (priority_queue->size > 1)
	{
		if (!huffman_extract_and_insert(priority_queue))
		{
			heap_delete(priority_queue, (void (*)(void *))free_tree);
			return (NULL);
		}
	}

	root = (binary_tree_node_t *)priority_queue->root->data;
	free(priority_queue->root);
	free(priority_queue);
	return (root);
}
