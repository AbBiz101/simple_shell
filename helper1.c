#include "allHeader.h"

/**
 * valueOfEnvVar - Find the value of an environment variable.
 * @info: Pointer to the structure containing environment variables.
 * @name: The name of the environment variable to search for.
 * Return: Pointer to the value of the environment variable if found, else NULL.
 */
char *valueOfEnvVar(infoTable *info, const char *name)
{
    listType *node = info->env;
    char *i;
    while (node)
    {
        i = needleStartWithHaystack(node->str, name);
        if (i && *i) return i;
        node = node->next;
    }
    return NULL;
}

/**
 * numToStr - Convert a long integer to a string representation in the specified base.
 * @num: The long integer to convert.
 * @base: The number base to use for conversion (e.g., 10 for decimal).
 * @flags: Flags to control conversion behavior (e.g., IF_UNSIGNED to treat num as unsigned).
 * Return: Pointer to the resulting string representation.
 */
char *numToStr(long int num, int base, int flags)
{
    static char *arr;
    static char buff[50];
    char sign = 0;
    char *ptr;
    unsigned long n = num;
    if (!(flags & IF_UNSIGNED) && num < 0)
    {
        n = -num;
        sign = '-';
    }
    arr = flags & TO_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buff[49]; 
    *ptr = '\0';
    do {
        *--ptr = arr[n % base];
        n /= base;
    } while (n != 0);
    if (sign)
        *--ptr = sign;
    return ptr;
}

/**
 * getHistoryFiles - Get the path to the history file.
 * @info: Pointer to the structure containing environment variables and history file name.
 * Return: Pointer to a dynamically allocated string containing the path to the history file.
 */
char *getHistoryFiles(infoTable *info)
{
    char *buf, *dir;
    dir = valueOfEnvVar(info, "HOME=");
    if (!dir) return (NULL);
    buf = malloc(sizeof(char) * (strLength(dir) + strLength(FILE_HIST) + 2));
    if (!buf) return NULL;
    buf[0] = '\0';
    copyStr(buf, dir);
    concatTwoStr(buf, "/");
    concatTwoStr(buf, FILE_HIST);
    return buf;
}

/**
 * envListPopulate - Populate the environment list in the 'info' structure.
 * @info: Pointer to the structure where the environment list will be populated.
 * Return: Always 0 (indicating successful completion).
 */
int envListPopulate(infoTable *info)
{
    listType *node = NULL;
    size_t i;
    for (i = 0; environ[i]; i++)
    {
        addNodeEnd(&node, environ[i], 0);
    }
    info->env = node;
    return 0;
}

/**
 * historyWriter - Write the history of commands to the history file.
 * @info: Pointer to the structure containing the history list and file information.
 * Return: 1 on successful write, -1 on failure.
 */
int historyWriter(infoTable *info)
{
    ssize_t fd;
    char *fileName = getHistoryFiles(info);
    listType *node = NULL;
    if (!fileName) return -1;
    fd = open(fileName, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(fileName);
    if (fd == -1) return -1;
    for (node = info->history; node; node = node->next)
    {
        printInputStr(node->str, fd);
        charCToFD('\n', fd);
    }
    charCToFD(FLASH_BUFFER, fd);
    close(fd);
    return 1;
}

