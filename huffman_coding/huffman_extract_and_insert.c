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
 * create_parent_node - create a parent node from two children
 * @left: left child
 * @right: right child
 *
 * Return: pointer to the created parent, or NULL on failure
 */
static binary_tree_node_t *create_parent_node(binary_tree_node_t *left,
	binary_tree_node_t *right)
{
	symbol_t *symbol;
	binary_tree_node_t *parent;
	symbol_t *left_symbol;
	symbol_t *right_symbol;
	size_t freq;

	left_symbol = (symbol_t *)left->data;
	right_symbol = (symbol_t *)right->data;
	freq = left_symbol->freq + right_symbol->freq;
	symbol = symbol_create(-1, freq);
	if (!symbol)
		return (NULL);

	parent = binary_tree_node(NULL, symbol);
	if (!parent)
	{
		free(symbol);
		return (NULL);
	}

	parent->left = left;
	parent->right = right;
	left->parent = parent;
	right->parent = parent;
	return (parent);
}

/**
 * huffman_extract_and_insert - combine the two least frequent nodes
 * @priority_queue: priority queue to extract from
 *
 * Return: 1 on success, 0 on failure
 */
int huffman_extract_and_insert(heap_t *priority_queue)
{
	binary_tree_node_t *left;
	binary_tree_node_t *right;
	binary_tree_node_t *parent;

	if (!priority_queue || priority_queue->size < 2)
		return (0);

	left = (binary_tree_node_t *)heap_extract(priority_queue);
	right = (binary_tree_node_t *)heap_extract(priority_queue);
	if (!left || !right)
	{
		free_tree(left);
		free_tree(right);
		return (0);
	}

	parent = create_parent_node(left, right);
	if (!parent)
	{
		free_tree(left);
		free_tree(right);
		return (0);
	}

	if (!heap_insert(priority_queue, parent))
	{
		free_tree(parent);
		return (0);
	}

	return (1);
}
