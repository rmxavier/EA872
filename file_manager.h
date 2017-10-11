
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

int acesso(char *local, char *recurso, char *metodo, FILE * resp_file);

void trataMetodo(char *metodo, int result, int fd, FILE * resp_file);

void trataGET(int result, int fd, FILE * resp_file);

void trataHEAD(int result, int fd, FILE * resp_file);

void imprime404(FILE * resp_file);

void imprime403(FILE * resp_file);