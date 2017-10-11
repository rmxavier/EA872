#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <pthread.h>
#define _XOPEN_SOURCE
#include <crypt.h>

void add_command_list(char *command);
void add_param_list(char *param);
void print_request_to_file();

extern FILE * req_file;
extern FILE * resp_file;
extern FILE * reg_file;

typedef struct command_list{
	char command[30];
	struct command_list *next;
	struct param_list *params;
} command_list;

typedef struct param_list{
	char param[250];
	struct param_list *next;
} param_list;