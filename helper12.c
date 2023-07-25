#include "allHeader.h"

/**
 * historyRenumber - Renumber the history entries.
 * @info: Pointer to the infoTable structure.
 * Return: The number of history entries after renumbering.
 */
int historyRenumber(infoTable *info)
{
	listType *node = info->history;
	int count = 0;
	while (node)
	{
		node->num = count++;
		node = node->next;
	}
	info->histCount = count;
	return count;
}

/**
 * isChain - Check for command chaining in the command buffer.
 * @info: Pointer to the infoTable structure.
 * @buf: The command buffer.
 * @p: Pointer to the current position in the buffer.
 * Return: 1 if a chaining operator is found, 0 otherwise.
 */
int isChain(infoTable *info, char *buf, size_t *p)
{
	size_t j = *p;
	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmdBufferType = OR_CMD;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmdBufferType = AND_CMD;
	}
	else if (buf[j] == ';')
	{
		buf[j] = 0;
		info->cmdBufferType = CHAIN_CMD;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * chainCheck - Handle command chaining logic.
 * @info: Pointer to the infoTable structure.
 * @buf: The command buffer.
 * @p: Pointer to the current position in the buffer.
 * @i: Current index in the buffer.
 * @len: Length of the buffer.
 */
void chainCheck(infoTable *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;
	if (info->cmdBufferType == AND_CMD && info->status )
	{
			buf[i] = 0;
			j = len;
	}
	if (info->cmdBufferType == OR_CMD && !info->status )
	{
			buf[i] = 0;
			j = len;
	}
	*p = j;
}

/**
 * listLength - Get the length of a linked list.
 * @h: Pointer to the head of the linked list.
 * Return: The length of the linked list.
 */
size_t listLength(const listType *h)
{
	size_t length = 0;
	while (h != NULL)
	{
		h = h->next;
		length++;
	}
	return (length);
}

/**
 * listStringTypeO - Convert a linked list of strings to an array of strings.
 * @head: Pointer to the head of the linked list.
 * Return: The array of strings (char**).
 */
char **listStringTypeO(listType *head)
{
	listType *node = head;
	size_t list_length = listLength(head), i;
	char **strs;
	char *str;
	if (!head || !list_length) return (NULL);
	strs = malloc(sizeof(char *) * (list_length + 1));
	if (!strs) return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(strLength(node->str) + 1);
		if (!str)
		{
			for (size_t j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}
		str = copyStr(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}

