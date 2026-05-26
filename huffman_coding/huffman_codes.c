#include <stdio.h>
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
 * print_codes - recursively print Huffman codes
 * @node: current tree node
 * @code: current code buffer
 * @depth: current code depth
 */
static void print_codes(binary_tree_node_t *node, char *code, size_t depth)
{
	symbol_t *symbol;

	if (!node)
		return;

	if (!node->left && !node->right)
	{
		symbol = (symbol_t *)node->data;
		if (depth == 0)
		{
			code[0] = '0';
			depth = 1;
		}
		code[depth] = '\0';
		printf("%c: %s\n", symbol->data, code);
		return;
	}

	code[depth] = '0';
	print_codes(node->left, code, depth + 1);
	code[depth] = '1';
	print_codes(node->right, code, depth + 1);
}

/**
 * huffman_codes - build a Huffman tree and print Huffman codes
 * @data: array of characters
 * @freq: array of associated frequencies
 * @size: number of elements in both arrays
 *
 * Return: 1 on success, 0 on failure
 */
int huffman_codes(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *root;
	char *code;
	int result;

	if (!data || !freq || size == 0)
		return (0);

	root = huffman_tree(data, freq, size);
	if (!root)
		return (0);

	code = malloc((size + 1) * sizeof(char));
	if (!code)
	{
		free_tree(root);
		return (0);
	}

	print_codes(root, code, 0);
	free(code);
	free_tree(root);
	result = 1;
	return (result);
}
