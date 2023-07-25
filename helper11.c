#include "allHeader.h"

/**
 * freePointer - Free a pointer and set it to NULL.
 * @ptr: Pointer to the pointer that needs to be freed.
 * Return: 1 if the pointer was successfully freed, 0 otherwise.
 */
int freePointer(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * commentToNull - Replace characters after '#' with null character '\0'.
 * @buf: Input buffer.
 * Return: None.
 */
void commentToNull(char *buf)
{
	for (int i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}

/**
 * envVarInit - Initialize or update an environment variable in info.
 * @info: Pointer to the infoTable structure.
 * @var: The name of the environment variable.
 * @value: The value of the environment variable.
 * Return: 0 on success, 1 on failure.
 */
int envVarInit(infoTable *info, char *var, char *value)
{
	char *buf = NULL;
	listType *node;
	char *p;

	if (!var || !value)
		return (1);

	buf = malloc(strLength(var) + strLength(value) + 2);
	if (!buf)
		return (1);
	copyStr(buf, var);
	concatTowStr(buf, "=");
	concatTowStr(buf, value);
	node = info->env;
	while (node)
	{
		p = needleStartWithHaystack(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = strDuplicate(buf);
			info->envChanged = 1;
			free(buf);
			return (0);
		}
		node = node->next;
	}
	addNodeEnd(&(info->env), strDuplicate(buf), 0);
	free(buf);
	info->envChanged = 1;
	return (0);
}

/**
 * historyListBuilder - Build a linked list of command history entries in info.
 * @info: Pointer to the infoTable structure.
 * @buf: The command string to add to the history.
 * @linecount: The line count of the command.
 * Return: 0 on success, 1 on failure.
 */
int historyListBuilder(infoTable *info, char *buf, int lineCount)
{
	listType *node = NULL;
	if (!info || !buf)
		return (1);
	if (info->history)
		node = info->history;
	node = addNodeEnd(&node, strDuplicate(buf), lineCount);
	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * currentEnvVar - Print the current environment variables.
 * @info: Pointer to the infoTable structure.
 * Return: Always returns 0.
 */
int currentEnvVar(infoTable *info)
{
	if (!info)
		return (0);
	strListPrinter(info->env);
	return (0);
}

