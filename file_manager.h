
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

int acesso(char *local, char *recurso, char *metodo);

void trataMetodo(char *metodo, int result, int fd);

void trataGET(int result, int fd);

void trataHEAD(int result, int fd);

void imprime404();

void imprime403();