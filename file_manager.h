
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>


struct stat fileStat;
FILE *file;
int c;
char path[1075];

char * acesso(char *local, char *recurso, char *metodo, char * response, FILE * reg_file);

char * trataMetodo(char *metodo, int result, int fd, char * resp_file, FILE * reg_file);

char * trataGET(int result, int fd, char * response, FILE * reg_file);

char * trataHEAD(int result, int fd, char * resp_file, FILE * reg_file);
