#include "allHeader.h"

/**
 * listPrinter - Print the content of a linked list of strings.
 * @head: Pointer to the head of the linked list.
 * Return: The number of nodes in the list.
 */
size_t listPrinter(const listType *head)
{
	const listType *node = head;
	size_t nodeCount = 0;
	while (node)
	{
		printStr(numToStr(node->num, 10, 0));
		charCToStdout(':');
		charCToStdout(' ');
		printStr(node->str ? node->str : "(nil)");
		printStr("\n");
		node = node->next;
		nodeCount++;
	}
	return nodeCount;
}

/**
 * findNodeWithPrefix - Find a node in the linked list that starts with the given prefix.
 * @head: Pointer to the head of the linked list.
 * @prefix: Prefix to search for.
 * @c: Optional character after the prefix (-1 if any character is allowed).
 * Return: Pointer to the first node with the specified prefix and character, or NULL if not found.
 */
listType *findNodeWithPrefix(listType *head, char *prefix, char c)
{
	char *p = NULL;
	while (head)
	{
		p = needleStartWithHaystack(head->str, prefix);
		if (p && (c == -1 || *p == c))
			return head;
		head = head->next;
	}
	return (NULL);
}

/**
 * findNodeIndex - Find the index of a node in the linked list.
 * @head: Pointer to the head of the linked list.
 * @node: Pointer to the node whose index is to be found.
 * Return: The index of the node, or -1 if the node is not found in the list.
 */
ssize_t findNodeIndex(listType *head, listType *node)
{
	size_t index = 0;
	while (head)
	{
		if (head == node)
			return index;
		head = head->next;
		index++;
	}
	return (-1);
}

/**
 * aliasReplace - Replace the current command with its corresponding alias value.
 * @info: Pointer to the infoTable struct containing command information.
 * Return: 1 if successful, 0 otherwise.
 */
int aliasReplace(infoTable *info)
{
	int i;
	listType *alias_node;
	char *p;
	for (i = 0; i < 10; i++)
	{
		alias_node = nodeStarter(info->alias, info->argv[0], '=');
		if (!alias_node)
			return (0);
		free(info->argv[0]);
		p = charInStr(alias_node->str, '=');
		if (!p)
			return (0);
		p = strDuplicate(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * varsReplace - Replace variables in the command arguments with their values.
 * @info: Pointer to the infoTable struct containing command information.
 * Return: 0 (Always successful).
 */
int varsReplace(infoTable *info)
{
	int i = 0;
	listType *env_node;
	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;
		if (!strCompar(info->argv[i], "$?"))
		{
			stringReplace(&(info->argv[i]), strDuplicate(numToStr(info->status, 10, 0)));
			continue;
		}
		if (!strCompar(info->argv[i], "$$"))
		{
			stringReplace(&(info->argv[i]), strDuplicate(numToStr(getpid(), 10, 0)));
			continue;
		}
		env_node = nodeStarter(info->env, &info->argv[i][1], '=');
		if (env_node)
		{
			stringReplace(&(info->argv[i]), strDuplicate(charInStr(env_node->str, '=') + 1));
			continue;
		}
		stringReplace(&info->argv[i], strDuplicate(""));
	}
	return (0);
}

