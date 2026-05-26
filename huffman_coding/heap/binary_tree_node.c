#include <stdlib.h>

#include "heap.h"

/**
 * binary_tree_node - create a new binary tree node
 * @parent: parent node of the new node
 * @data: data stored in the new node
 *
 * Return: pointer to the created node, or NULL on failure
 */
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data)
{
	binary_tree_node_t *node;

	node = malloc(sizeof(binary_tree_node_t));
	if (!node)
		return (NULL);

	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	return (node);
}
