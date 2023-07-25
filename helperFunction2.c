#include "allHeader.h"

/**
 * unsetAlias - Unsets an alias associated with a specified string.
 * @info: parameter struct
 * @str: the string alias to be unset
 * Return: 0 on success, 1 on error (if the alias doesn't exist or other issues)
 */
int unsetAlias(infoTable *info, char *str)
{
    char *p, c;
    int ret;
    p = charInStr(str, '=');
    if (!p)
        return 1;
    c = *p; 
    *p = 0;
    ret = deleteIndexNode(&(info->alias),
                          indexNode(info->alias, nodeStarter(info->alias, str, -1)));
    *p = c; 
    return ret; 
}

/**
 * setAlias - Sets an alias for a specified string.
 * @info: parameter struct
 * @str: the string alias to be set
 * Return: 0 on success, 1 on error
 */
int setAlias(infoTable *info, char *str)
{
    char *p;
    p = charInStr(str, '=');
    if (!p) 
        return 1;

    if (!*++p)
        return unsetAlias(info, str);
    unsetAlias(info, str);
    return (addNodeEnd(&(info->alias), str, 0) == NULL);
}

/**
 * printAlias - Prints an alias string.
 * @node: Pointer to the alias node
 * Return: 0 on success, 1 on error (if the node is NULL)
 */
int printAlias(listType *node)
{
    char *p = NULL, *a = NULL;
    if (node)
    {
        p = charInStr(node->str, '=');
        for (a = node->str; a <= p; a++)
            charCToStdout(*a);
        charCToStdout('\'');
        printStr(p + 1);
        charCToStdout('\'');
        printStr("\n");
        return 0; 
    }
    return 1;
}

/**
 * historyList - Display the history list of commands.
 * @info: Pointer to the structure containing the history list.
 *  Return: Always 0
 */
int historyList(infoTable *info)
{

    listPrinter(info->history);
    return 0;

/**
 * mimicAliasBuiltin - Mimics the behavior of the alias built-in command.
 * @info: Pointer to the structure containing information and aliases.
 * Return: Always 0 (indicating successful completion).
 */
int mimicAliasBuiltin(infoTable *info)
{
    int i = 0;
    char *p = NULL;
    listType *node = NULL;
    if (info->argc == 1)
    {
        node = info->alias;
        while (node)
        {
            printAlias(node);
            node = node->next;
        }
        return 0;
    }
    for (i = 1; info->argv[i]; i++)
    {
        p = charInStr(info->argv[i], '=');
        if (p)
            setAlias(info, info->argv[i]);
        else
            printAlias(nodeStarter(info->alias, info->argv[i], '='));
    }
    return 0;

