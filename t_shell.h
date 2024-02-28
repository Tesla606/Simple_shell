#ifndef _T_SHELL_
#define _T_SHELL_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

#define COMMAND_NORMAL 0
#define COMMAND_OR 1
#define COMMAND_AND 2
#define COMMAND_CHAIN 3

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: string
 * @next: points to the next node
 */
typedef struct StringList
{
    int num;
    char *str;
    struct StringList *next;
} str_list_t;

/**
 *struct passinfo - contains arguements to pass into a function
 *
 *@arg: string from getline containing arguments
 *@arguments: array of strings
 *@path: string path for current command
 *@fname: the program filename
 *@environment: linkedlist local copy of environ
 *@environ: custom modified copy of environ from env
 *@history: history node
 *@alias: alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmdBuffer: pointer
 *@commandBufferType: CMD_type
 *@readFd: fd from which to read line input
 *@histcount: history line number count
 *@argumentCount: argument count
 *@lineCount: error count
 *@errNum: error code for exit
 *@lineCountFlag: on count this line of input
 */
typedef struct passinfo
{
	char *arg;
	char **arguments;
	char *path;
	char *fname;
	str_list_t *environment;
	str_list_t *history;
	str_list_t *alias;
	char **environ;
	int environmentChanged;
	int status;

	char **cmdBuffer;
	int commandBufferType;
	int readFd;
	int histcount;
	int argumentCount;
	unsigned int lineCount;
	int errNum;
	int lineCountFlag;
} info_t;

#define INIT_INFO \
{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0, 0, 0, 0, 0, }

extern char **environ;

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} _builtin_tbl;

/* str_func1 */
int _strlen(char *str);
int _strcmp(char *str1, char *str2);
char *_starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);
int _putchar(char character);

/* str_func2 */
char *_strcpy(char *dest, char *src);
char *_strncpy(char *dest, char *src, int n);
char *_strdup(const char *str);
char *_strncat(char *dest, char *src, int n);
void _puts(char *str);


/* str_func3 */
char *_strchr(char *str, char c);
char **_strtok(char *str, char *delimiters);
char **_strtok2(char *str, char delimiter);
int is_delimiter(char c, char *dlm);

/* mem_func */
int freePointer(void **ptr);
char *memSet(char *memory, char byte, unsigned int size);
void freeStringArray(char **stringArray);
void *reallocateMemory(void *prevPtr, unsigned int oldSize, unsigned int newSize);

/* linked_lis */
str_list_t *addNode(str_list_t **head, const char *data, int num);
str_list_t *addNodeEnd(str_list_t **head, const char *data, int num);
size_t printStr(const str_list_t *h);
int deleteNodeAtIndex(str_list_t **head, unsigned int index);
void freeList(str_list_t **head_ptr);

/* likend_lis2 */
size_t listLen(const str_list_t *head);
char **listToString(str_list_t *head);
size_t printList(const str_list_t *head);
str_list_t *nodePrefix(str_list_t *node, char *pre, char c);
ssize_t getNodeIndex(str_list_t *head, str_list_t *node);

/* func_helpers1 */
int check_interactive_mode(info_t *info);
int is_character_delimiter(char c, char *delimiter);
int is_alpha_character(int c);
int _atoi(char *str);
void erase_comments(char *buffer);

/* func_helpers2 */
int safe_atoi(char *str);
void print_custom_err(info_t *info, char *err_string);
int print_int(int input, int fd);
char *convert_to_string(long int num, int base, int flags);

/* builtin_func */
int exit_shell(info_t *info);
int change_directory(info_t *info);
int show_help(info_t *info);
int display_history(info_t *info);
int manage_alias(info_t *info);

/* builtin_sub_func */
int remove_alias(info_t *info, char *str);
int assign_alias(info_t *info, char *str);
int display_alias(str_list_t *node);

/* getline */
ssize_t bufferInput(info_t *info, char **buffer, size_t *length);
ssize_t getInput(info_t *info);
ssize_t readBuffer(info_t *info, char *buf, size_t *i);
int _getline(info_t *info, char **ptr, size_t *length);
void interruptHandler(__attribute__((unused))int signalNumber);

/* chain_func */
int isCommandChain(info_t *info, char *buffer, size_t *position);
void checkChain(info_t *info, char *buf, size_t *pos, size_t i, size_t l);
int replaceAliases(info_t *info);
int replaceVariables(info_t *info);
int replaceString(char **oldString, char *newString);

/* enviroment_func1 */
int displayEnvironment(info_t *info);
char *getEnvironmentVariable(info_t *info, const char *name);
int _mysetenv(info_t *info);
int unsetEnvironmentVariable(info_t *info);
int populateEnvironmentList(info_t *info);

/* enviroment_func2 */
char **copyEnvironToStringArray(info_t *info);
int removeEnvironmentVariable(info_t *info, char *variable);
int setEnvironmentVariable(info_t *info, char *variable, char *value);

/* shell_loop */
int shellLoop(info_t *info, char **argv);
int findBuiltin(info_t *info);
void findCommand(info_t *info);
void forkCommand(info_t *info);

/* info_func */
void clearInfo(info_t *info);
void setInfo(info_t *info, char **argv);
void freeInfo(info_t *info, int all);

/* path_func */
int isCmd(info_t *info, char *path);
char *dupChars(char *pathstr, int start, int stop);
char *findPath(info_t *info, char *spath, char *cmd);

/* error_func */
void _eputs(char *str);
int _eputchar(char c);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* fileio_func */
char *getHistoryFile(info_t *info);
int writeHistory(info_t *info);
int readHistory(info_t *info);
int buildHistoryList(info_t *info, char *buffer, int linecount);
int renumberHistory(info_t *info);


#endif /* _S_SHELL_ */



