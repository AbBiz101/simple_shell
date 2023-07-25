#ifndef _allHeader_H_
#define _allHeader_H_
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define OR_CMD 1
#define AND_CMD 2
#define NORM_CMD 0
#define CHAIN_CMD 3
#define MAX_HIST 4096
#define IF_UNSIGNED	2
#define TO_LOWERCASE 1
#define FLASH_BUFFER -1
#define ERROR_FILE_OPEN 126
#define USE_GETLINE_STRTOK 0
#define ERROR_FILE_NOT_FOUND 127
#define READ_WRITE_BUF_SIZE 1024
#define FILE_HIST ".shell_project"
extern char **environ;
/**
 * struct listNode - Singly linked list node
 * @num: Number field
 * @str: String field
 * @next: Points to the next node
 */
typedef struct listNode
{
int num;
char *str;
struct listNode *next;
} listType;
/**
 * struct infoPass - Contains pseudo-arguments to pass into a function,
 *                   allowing uniform prototypes for function pointers.
 * @arg: A string generated from getline containing arguments.
 * @argv: An array of strings generated from arg.
 * @path: A string path for the current command.
 * @argc: The argument count.
 * @lineCount: The error count.
 * @errNum: The error code for exit()s.
 * @lineCountFlag: If on, count this line of input.
 * @fname: The program filename.
 * @env: Linked list local copy of environ.
 * @environ: Custom modified copy of environ from LL env.
 * @history: The history node.
 * @alias: The alias node.
 * @envChanged: If on, environ was changed.
 * @status: The return status of the last exec'd command.
 * @cmdBuffer: Address of pointer to cmdBuffer, on if chaining.
 * @cmdBufferType: CMD_type ||, &&, ;
 * @readfd: The fd from which to read line input.
 * @histCount: The history line number count.
 */
typedef struct infoPass
{
char *arg;
char **argv;
char *path;
int argc;
unsigned int lineCount;
int errNum;
int lineCountFlag;
char *fname;
listType *env;
listType *history;
listType *alias;
char **environ;
int envChanged;
int status;
char **cmdBuffer;
int cmdBufferType;
int readfd;
int histCount;
} infoTable;
#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}
/**
 * struct builtin - Contains a builtin command string and related function.
 * @command: The builtin command string.
 * @func: Pointer to the function that handles the execution of
 * the builtin command.
 */
typedef struct builtin
{
char *type;
int (*func)(infoTable *);
} builtinTable;
/***************** helper1.c ****************/
char *valueOfEnvVar(infoTable *, const char *);
char *numToStr(long int, int, int);
char *getHistoryFiles(infoTable *info);
int envListPopulate(infoTable *);
int historyWriter(infoTable *info);
/***************** helper2.c ****************/
int deleteEnvVar(infoTable *);
int historyReader(infoTable *info);
int newEnvVer(infoTable *);
int strLength(char *);
char *copyStr(char *, char *);
/***************** helper3.c ****************/
char *pathFind(infoTable *, char *, char *);
int shellLoop(infoTable *, char **);
int strCompar(char *, char *);
int strToInt(char *);
int shellExit(infoTable *);
/***************** helper4.c ****************/
void infoStructClear(infoTable *);
void errorMessage(infoTable *, char *);
void infoStructInitialize(infoTable *, char **);
int changeCD(infoTable *);
char **splitStringToWords(char *, char);
/***************** helper5.c ****************/
void printInputStr_0(char *);
int isInteractive(infoTable *);
char **splitString(char *, char *);
int charCToStderr(char);
int isDelimeter(char, char *);
/***************** helper6.c ****************/
int charCToFD(char c, int fd);
char *charInStr(char *, char);
void forkCMD(infoTable *);
char *strDuplicate(const char *);
char *needleStartWithHaystack(const char *, const char *);
/***************** helper7.c ****************/
char *concatTowStr(char *, char *);
void printStr(char *);
int isAlpha(int);
int printInputStr(char *str, int fd);
char *concatTwoStr(char *, char *, int);
/***************** helper8.c ****************/
int ifStrToInt(const char *);
int isExecutable(const char *);
char *strCopy(char *, const char *, int);
char *setMemory(char *, char, unsigned int);
void freeString(char **);
/***************** helper9.c ****************/
void *reallocMemory(void *, unsigned int, unsigned int);
int shellHelp(infoTable *);
int intPrinter(int, int);
void freeInfoStructFields(infoTable *, int);
int removeEnvVar(infoTable *, char *);
/***************** helper10.c ****************/
char **getStrEnvVar(infoTable *);
void findCMDPath(infoTable *);
int charCToStdout(char);
char *duplicateChar(char *, int, int);
int findBuiltinCMD(infoTable *);
/***************** helper11.c ****************/
int freePointer(void **);
void commentToNull(char *);
int envVarInit(infoTable *, char *, char *);
int historyListBuilder(infoTable *info, char *buf, int lineCount);
int currentEnvVar(infoTable *);
/***************** helper12.c ****************/
int historyRenumber(infoTable *info);
int isChain(infoTable *, char *, size_t *);
void chainCheck(infoTable *, char *, size_t *, size_t, size_t);
size_t listLength(const listType *);
char **listStringTypeO(listType *);
/***************** helper13.c ****************/
size_t listPrinter(const listType *);
listType *nodeStarter(listType *, char *, char);
ssize_t indexNode(listType *, listType *);
int aliasReplace(infoTable *);
int varsReplace(infoTable *);
/***************** helper14.c ****************/
listType *addNodeStart(listType **, const char *, int);
listType *addNodeEnd(listType **, const char *, int);
size_t strListPrinter(const listType *);
int deleteIndexNode(listType **, unsigned int);
void freeListNodes(listType **);
/***************** helper15.c ****************/
int stringReplace(char **, char *);
/***************** helperFunction1.c ****************/
ssize_t lineMinusNewline(infoTable *);
int nextInputLine(infoTable *, char **, size_t *);
void blockCtrlC(int);
/***************** helperFunction2.c ****************/
int historyList(infoTable *);
int mimicAliasBuiltin(infoTable *);
#endif

