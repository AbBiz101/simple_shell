#include "allHeader.h"

/**
 * reallocMemory - Reallocates a block of memory.
 * @ptr: Pointer to the previous malloc'ated block.
 * @old_size: Byte size of the previous block.
 * @new_size: Byte size of the new block.
 * Return: Pointer to the reallocated block of memory.
 */
void *reallocMemory(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *p;
    if (!ptr) return malloc(new_size);

    if (!new_size)
    {
        free(ptr);
        return NULL;
    }
    if (new_size == old_size) return ptr;
    p = malloc(new_size);
    if (!p) return NULL;
    if (old_size < new_size) new_size = old_size;
    while (new_size--)
        p[new_size] = ((char *)ptr)[new_size];
    free(ptr);
    return p;
}

/**
 * shellHelp - Displays help information.
 * @info: The infoTable structure containing the arguments and other data.
 *
 * Return: Always 0.
 */
int shellHelp(infoTable *info)
{
	(void)info;
	printStr("The help command is not yet implemented.\n");
	return (0);
}

/**
 * intPrinter - Prints an integer to the specified file descriptor.
 * @input: The integer to be printed.
 * @fd: The file descriptor to which the integer will be printed.
 * Return: The number of characters printed.
 */
int intPrinter(int input, int fd)
{
	int (*outputFunction)(char) = charCToFD;
	int i, count = 0;
	unsigned int _abs_, current;
	if (fd == STDERR_FILENO) outputFunction = charCToStderr;
	if (input < 0)
	{
		_abs_ = -input;
		outputFunction('-');
		count++;
	}
	else _abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			outputFunction('0' + current / i);
			count++;
		}
		current %= i;
	}
	outputFunction('0' + current);
	count++;
	return (count);
}

/**
 * freeInfoStructFields - Frees the allocated memory within the info struct.
 * @info: Pointer to the info struct.
 * @all: 1 to free all fields, 0 to free only selected fields.
 */
void freeInfoStructFields(infoTable *info, int all)
{
	if (!info) return;
	freeString(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (all)
	{
		if (!info->cmdBuffer)
			free(info->arg);
		if (info->env)
			freeListNodes(&(info->env));
		if (info->history)
			freeListNodes(&(info->history));
		if (info->alias)
			freeListNodes(&(info->alias));
		freeString(info->environ);
		info->environ = NULL;
		freePointer((void **)info->cmdBuffer);
		if (info->readfd > 2)
			close(info->readfd);
		charCToFD(FLASH_BUFFER, STDOUT_FILENO); 
	}
}

/**
 * removeEnvVar - Removes environment variables that match the given var string.
 * @info: Pointer to the info struct.
 * @var: The variable name to be removed.
 * Return: 1 if environment changed, 0 otherwise.
 */
int removeEnvVar(infoTable *info, char *var)
{
	listType *prev_node = NULL;
	listType *current_node = info->env;
	int envChanged = 0;
	if (!current_node || !var) return 0;
	while (current_node)
	{
		char *p = needleStartWithHaystack(current_node->str, var);
		if (p && *p == '=')
		{
			if (prev_node)
				prev_node->next = current_node->next;
			else
				info->env = current_node->next;
			free(current_node->str);
			free(current_node);
			envChanged = 1;
			break;
		}
		prev_node = current_node;
		current_node = current_node->next;
	}
	return envChanged;
}
