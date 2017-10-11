
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include "server.h"


struct stat fileStat;
FILE *file;
int c;
char path[1075];

FILE * req_file;
FILE * resp_file;
FILE * reg_file;

/*void main( int argc, char *argv[]){
	char local[]= "/home/EC11/ra116006/meu-webspace";
	char recurso[50];
	char metodo[20];
	
	printf("Escreva o método:");
	scanf("%s", metodo);
  
	//printf("Escreva o local:");
	//scanf("%s", local);
	
	
	printf("Escreva o rescurso:");
	scanf("%s", recurso);

	int result = acesso(local, recurso, metodo);
	printf("\n%d", result);
	return;

}*/


int acesso(char *local, char *recurso, char *metodo){
	struct stat statarq;
	int fd;
	DIR *dir;
	
	/*****Trecho para pegar o path do arquivo e incluí-lo em um só*****/

	strcpy (path, local); //copia o local passado ao path
	if (strncmp("/", recurso, 1) == -1) {
		strcat(path, "/");//adicona '/' ao path
	}
	strcat(path, recurso);  //cooloca o nome do recurso depois do path
	
	printf("[FILE_MANAGER] Recurso a ser localizado: %s\n", path);
	//printf("\no path ta aqui: %s\n", path);

	/*****Trecho para buscar o recurso dentro do path passado*****/
	//caso não ache o recurso
	
	if (stat(path, &statarq) == -1){
		trataMetodo(metodo, 404, fd);
		return 404;
	}
	else{
		//printf("access %d",access(path, R_OK));
		//caso o recurso não dê permissão de acesso
		if(access(path, R_OK) != 0){
			trataMetodo(metodo, 403, fd);
			return 403;
		}
		//recurso existe e tem permissão de acesso
		else{
			/*****Trecho para ler e tratar se é um arquivo ou diretório*****/
			
			//caso seja um arquivo
			if((statarq.st_mode & S_IFMT)==S_IFREG){
				//Abre e imprime o arquivo
				fd = open(path, O_RDONLY, 0600);
				trataMetodo(metodo, 200, fd);
				write(fd,path, sizeof(path));
			}
			else{
				//caso seja diretório
				if((statarq.st_mode & S_IFMT)==S_IFDIR){
					//caso o diretório não tenha permissão de acesso
					if(access(path, X_OK) != 0) {
					  trataMetodo(metodo,403, fd);
					  return 403;
					} else {
						struct dirent *newDir;
						//abre o diretório
						dir = opendir(path);
						//le os arquivos até achar um index ou welcome
						while(newDir = readdir(dir)){
							
							printf("%s\n",newDir->d_name);
							
							if(strcmp("index.html",newDir->d_name)==0){
								strcpy (recurso, "index.html");
								strcat(path, "/");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0){
								  trataMetodo(metodo, 403, fd);
								  return 403;
								}
								else{
									fd = open(path, O_RDONLY, 0600);
									//write(fd,"", 50);
									write(fd,path, sizeof(path));
									trataMetodo(metodo, 200, fd);
									return 0;
								}
								break;
							}
							if(strcmp("welcome.html",newDir->d_name)==0){
								strcpy (recurso, "welcome.html");
								strcat(path, "/");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0){
									trataMetodo(metodo, 403,fd);
									return 403;
								}
								else{
									fd = open(path, O_RDONLY, 0600);
									//write(fd,"", 50);
									write(fd,path, sizeof(path));
									trataMetodo(metodo, 200, fd);
									return 0;
								}
								break;
							}
						}
						//caso não tenha achado nenhum arquivo index ou welcome
						trataMetodo(metodo, 404, fd);
						return 404;
					}
				}
			}
			return 0;
		}
	}
}

void trataMetodo(char *metodo, int result, int fd){
 //printf("%d\n", result);
  if(strcmp(metodo, "GET")==0){
      trataGET(result, fd);
  }
  else if(strcmp(metodo, "HEAD")==0){
      trataHEAD(result, fd);
  }
  else if(strcmp(metodo, "TRACE")==0){
      trataTRACE();
  }
  else if(strcmp(metodo, "OPTIONS")==0){
      trataOPTIONS();
    }
}

void trataGET(int result, int fd){
  char resultado[]="                                 ";
  char resposta[] = "HTTP 1/1 ";
  time_t rawtime;
  time_t lastmod = fileStat.st_mtime; 
  struct tm * timeinfo;
  fstat(fd, &fileStat);	
  
  switch (result){
    case 200:
      strcpy(resultado,"200 OK");
      break;
    case 403:
      strcpy(resultado,"403 FORBIDDEN");
      break;
    case 404:
      strcpy(resultado,"404 NOT FOUND");
      break;
  }
  strcat(resposta, resultado);
  printf("%s\n", resposta);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  fprintf (resp_file, "Date: %s", asctime (timeinfo) );
  fprintf(resp_file, "Server: Servidor HTTP ver 0.1 dos Descolados");
  fprintf(resp_file, "Connection: PEGAR DO PARSER!!!!\n");
  timeinfo = localtime(&lastmod);
  if(result==200){
    fprintf(resp_file, "Last-Modified: %s", asctime(timeinfo));
    fprintf(resp_file, "Content-Lenght: %zd\n", fileStat.st_size);
    fprintf(resp_file, "Content-Type: text/html\n");
  
  file = fopen(path, "r");
    if (file) {
	while ((c = getc(file)) != EOF)
	  putchar(c);
	  fclose(file);
    }
  }
  if(result==404)
    imprime404();
  if(result==403)
    imprime403();
}

void trataHEAD(int result, int fd){
  char resultado[]="                                 ";
  char resposta[] = "HTTP 1/1 ";
  time_t rawtime;
  time_t lastmod = fileStat.st_mtime; 
  struct tm * timeinfo;
  fstat(fd, &fileStat);	
  
  
  switch (result){
    case 200:
      strcpy(resultado,"200 OK");
    break;
    case 403:
      strcpy(resultado,"403 FORBIDDEN");
    break;
    case 404:
      strcpy(resultado,"404 NOT FOUND");
    break;
  }
  strcat(resposta, resultado);
  printf("%s\n", resposta);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  fprintf (resp_file, "Date: %s", asctime (timeinfo) );
  fprintf(resp_file, "Server: Servidor HTTP ver 0.1 dos Descolados");
  fprintf(resp_file, "Connection: PEGAR DO PARSER!!!!\n");
  timeinfo = localtime(&lastmod);
  if(result == 200){
    fprintf(resp_file, "Last-Modified: %s", asctime(timeinfo));
    fprintf(resp_file, "Content-Lenght: %zd\n", fileStat.st_size);
    fprintf(resp_file, "Content-Type: text/html\n");
  }
}

void trataTRACE(){
  char resultado[]="                                 ";
  char resposta[] = "HTTP /1.1 ";
  time_t rawtime;
  time_t lastmod = fileStat.st_mtime; 
  struct tm * timeinfo;
  strcpy(resultado,"200 OK");
  strcat(resposta, resultado);
  printf("%s\n", resposta);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ("Date: %s", asctime (timeinfo) );
  printf("Server: Servidor HTTP ver 0.1 dos Descolados\n");
  printf("Connection: PEGAR DO PARSER!!!!\n");
  printf("Content-Type: message/http\n");
}

void trataOPTIONS(){
  char resultado[]="                                 ";
  char resposta[] = "HTTP /1.1 ";
  time_t rawtime;
  time_t lastmod = fileStat.st_mtime; 
  struct tm * timeinfo;
  strcpy(resultado,"200 OK");
  strcat(resposta, resultado);
  printf("%s\n", resposta);
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ("Date: %s", asctime (timeinfo) );
  printf("Server: Servidor HTTP ver 0.1 dos Descolados\n");
  printf("Connection: close\n");
  printf("Allow: GET, HEAD, TRACE, OPTIONS\n");
  printf("Content-Lenght: 0\n", fileStat.st_size);
}

void imprime404(){
  fprintf(resp_file, "<html><<head>    <title>Página não encontrada</title>    </head><body>  <h1>ERROR 404</h1>  <br>Essa página não existe<br>");
}

void imprime403(){
  printf(resp_file, "<html><<head>    <title>Conteúdo Proibido</title>    </head><body>  <h1>ERROR 403</h1>  <br>Você não tem pemissão de acesso a esse conteúdo<br>");
}