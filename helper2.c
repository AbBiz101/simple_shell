#include "allHeader.h"

/**
 * deleteEnvVar - Delete environment variables.
 * @info: Pointer to the structure containing environment variables and arguments.
 * Return: 0 on successful deletion, 1 if there are too few arguments.
 */
int deleteEnvVar(infoTable *info)
{
    int num;
    if (info->argc == 1)
    {
        printInputStr_0("Too few arguments.\n");
        return 1;
    }
    for (num = 1; num < info->argc; num++)
    {
        removeEnvVar(info, info->argv[num]);
    }
    return 0;
}

/**
 * historyReader - Read the history of commands from the history file.
 * @info: Pointer to the structure containing history list and history file information.
 * Return: The updated count of history entries after reading from the history file.
 */
int historyReader(infoTable *info)
{
    struct stat st;
    int i, last, lineCount = 0;
    ssize_t fd, rdlen, fsize = 0;
    char *buf = NULL, *fileName = getHistoryFiles(info);
    if (!fileName) return 0;
    fd = open(fileName, O_RDONLY);
    free(fileName);
    if (fd == -1) return 0;
    if (!fstat(fd, &st)) fsize = st.st_size;
    if (fsize < 2)
    {
        close(fd);
        return 0;
    }
    buf = malloc(sizeof(char) * (fsize + 1));
    if (!buf)
    {
        close(fd);
        return 0;
    }
    rdlen = read(fd, buf, fsize);
    buf[fsize] = 0;
    close(fd);
    if (rdlen <= 0)
    {
        free(buf);
        return 0;
    }
    last = 0;
    for (i = 0; i < fsize; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            historyListBuilder(info, buf + last, lineCount++);
            last = i + 1;
        }
    }
    if (last != i)
        historyListBuilder(info, buf + last, lineCount++);
    free(buf);
    while (lineCount-- >= MAX_HIST)
        deleteIndexNode(&(info->history), 0);
    historyRenumber(info);
    return info->histCount;
}

/**
 * newEnvVar - Create or modify an environment variable.
 * @info: Pointer to the structure containing environment variables and arguments.
 * Return: 0 on successful environment variable creation/modification, 1 on error.
 */
int newEnvVar(infoTable *info)
{
    if (info->argc != 3)
    {
        printInputStr_0("Incorrect number of arguments\n");
        return 1;
    }
    if (envVarInit(info, info->argv[1], info->argv[2])) return 0; 
    return 1;
}

/**
 * strLength - Calculate the length of a null-terminated string.
 * @s: Pointer to the null-terminated string whose length is to be calculated.
 * Return: The length of the string.
 */
int strLength(char *s)
{
    int i = 0;
    if (!s) return 0;
    while (*s++)
        i++;
    return i;

/**
 * copyStr - Copy the contents of a null-terminated string from source to destination.
 * @dest: Pointer to the destination character array where the string will be copied.
 * @src:  Pointer to the null-terminated source string to be copied.
 * Return: Pointer to the destination character array (i.e., the 'dest' pointer).
 */
char *copyStr(char *dest, char *src)
{
    int i = 0;
    if (dest == src || src == NULL) return dest;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; 
    return dest;
}


