#include <stdlib.h>

#include "huffman.h"

/**
 * free_nested_tree - free a Huffman tree recursively
 * @node: tree node to free
 */
static void free_nested_tree(binary_tree_node_t *node)
{
	if (!node)
		return;

	free_nested_tree(node->left);
	free_nested_tree(node->right);
	free(node->data);
	free(node);
}

/**
 * nested_cmp - compare the frequency of two nested tree nodes
 * @p1: first nested node
 * @p2: second nested node
 *
 * Return: negative, zero, or positive depending on the frequencies
 */
static int nested_cmp(void *p1, void *p2)
{
	binary_tree_node_t *n1;
	binary_tree_node_t *n2;
	symbol_t *s1;
	symbol_t *s2;

	n1 = (binary_tree_node_t *)p1;
	n2 = (binary_tree_node_t *)p2;
	s1 = (symbol_t *)n1->data;
	s2 = (symbol_t *)n2->data;
	if (s1->freq < s2->freq)
		return (-1);
	if (s1->freq > s2->freq)
		return (1);
	return (0);
}

/**
 * create_leaf_node - create a nested leaf node for the queue
 * @data: character stored in the symbol
 * @freq: symbol frequency
 *
 * Return: pointer to the created nested node, or NULL on failure
 */
static binary_tree_node_t *create_leaf_node(char data, size_t freq)
{
	symbol_t *symbol;
	binary_tree_node_t *node;

	symbol = symbol_create(data, freq);
	if (!symbol)
		return (NULL);

	node = binary_tree_node(NULL, symbol);
	if (!node)
	{
		free(symbol);
		return (NULL);
	}

	return (node);
}

/**
 * huffman_priority_queue - create a priority queue for Huffman coding
 * @data: array of characters
 * @freq: array of associated frequencies
 * @size: number of elements in both arrays
 *
 * Return: pointer to the created priority queue, or NULL on failure
 */
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	heap_t *priority_queue;
	size_t i;
	binary_tree_node_t *leaf;

	if (!data || !freq || size == 0)
		return (NULL);

	priority_queue = heap_create(nested_cmp);
	if (!priority_queue)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		leaf = create_leaf_node(data[i], freq[i]);
		if (!leaf || !heap_insert(priority_queue, leaf))
		{
			if (leaf)
				free_nested_tree(leaf);
			heap_delete(priority_queue, (void (*)(void *))free_nested_tree);
			return (NULL);
		}
	}

	return (priority_queue);
}
