#include "allHeader.h"

/**
 * addNodeStart - Add a new node at the beginning of a linked list.
 * @head: Pointer to the pointer of the head of the linked list.
 * @str: String to be assigned to the new node (can be NULL).
 * @num: Number to be assigned to the new node.
 * Return: Pointer to the new head of the linked list.
 */
listType *addNodeStart(listType **head, const char *str, int num)
{
	if (!head)
		return (NULL);
	listType *newNode = malloc(sizeof(listType));
	if (!newNode)
		return (NULL);
	setMemory((void *)newNode, 0, sizeof(listType));
	newNode->num = num;
	if (str)
	{
		newNode->str = strDuplicate(str);
		if (!newNode->str)
		{
			free(newNode);
			return (NULL);
		}
	}
	newNode->next = *head;
	*head = newNode;
	return (newNode);
}

/**
 * addNodeEnd - Add a new node at the end of a linked list.
 * @head: Pointer to the pointer of the head of the linked list.
 * @str: String to be assigned to the new node (can be NULL).
 * @num: Number to be assigned to the new node.
 * Return: Pointer to the new node added at the end of the linked list.
 */
listType *addNodeEnd(listType **head, const char *str, int num)
{
	if (!head)
		return (NULL);
	listType *newNode = malloc(sizeof(listType));
	if (!newNode)
		return (NULL);
	setMemory((void *)newNode, 0, sizeof(listType));
	newNode->num = num;
	if (str)
	{
		newNode->str = strDuplicate(str);
		if (!newNode->str)
		{
			free(newNode);
			return (NULL);
		}
	}
	newNode->next = NULL;
	if (*head)
	{
		listType *node = *head;
		while (node->next)
			node = node->next;
		node->next = newNode;
	}
	else
	{
		*head = newNode;
	}
	return newNode;
}

/**
 * strListPrinter - Print the strings stored in a linked list.
 * @h: Pointer to the head of the linked list.
 * Return: Number of nodes in the linked list.
 */
size_t strListPrinter(const listType *h)
{
	size_t count = 0;
	while (h)
	{
		printStr(h->str ? h->str : "(nil)");
		printStr("\n");
		h = h->next;
		count++;
	}
	return (count);
}

/**
 * deleteIndexNode - Delete a node from a linked list at the specified index.
 * @head: Pointer to the head of the linked list.
 * @index: Index of the node to delete.
 * Return: 1 if successful, 0 if the node at the given index does not exist.
 */
int deleteIndexNode(listType **head, unsigned int index)
{
	listType *node, *prevNode;
	unsigned int i = 0;
	if (!head || !*head) return (0);
	if (index == 0)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	prevNode = NULL;
	while (node)
	{
		if (i == index)
		{
			prevNode->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prevNode = node;
		node = node->next;
	}
	return (0);
}

/**
 * freeListNodes - Free all nodes of a linked list and set the head pointer to NULL.
 * @head_ptr: Pointer to the head of the linked list.
 */
void freeListNodes(listType **head_ptr)
{
	listType *current, *nextNode, *head;
	if (!head_ptr || !*head_ptr) return;
	head = *head_ptr;
	current = head;
	while (current)
	{
		nextNode = current->next;
		free(current->str);
		free(current);
		current = nextNode;
	}
	*head_ptr = NULL;
}

