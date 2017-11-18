
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
char c;
char * str;
char path[1075];

char original_local[1075];

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

void print_file_to_string(char * fname, char * response) {
	FILE *fp;
	char str[80];

	if ((fp= fopen(fname, "r")) == NULL) {
		printf("cannot open file");
		exit(1);
	}

	while(!feof(fp)) {
		fgets(str,79,fp);
		printf("%s",str);
		strcat(response, str);
	}

	fclose(fp);
}

void write_header(char * response, char * header, char * info, int suppress_last_character) {
	
	if (suppress_last_character) {
		info[strlen(info) - 1] = '\0';
	}
	
	strcat(response, header);
	strcat(response, info);
	strcat(response, "\r\n");
}

char * trataGET(int result, int fd, char * response, FILE * reg_file, char connection[]){
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);	
	switch (result){
		case 200:
			strcat(response, "200 OK\r\n");
			break;
		case 403:
			strcat(response, "403 FORBIDDEN\r\n");
			break;
		case 404:
			strcat(response, "404 NOT FOUND\r\n");
			break;
	}

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write_header(response, "Date: ", asctime (timeinfo), 1);

	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");

	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n");

	timeinfo = localtime(&lastmod);
	if(result==200) {
		write_header(response, "Last-Modified: ", asctime(timeinfo), 1);
		
		char * content_length;
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);
		strcat(response, "Content-Type: text/html\r\n");
	}
	
	
	strcat(response, "\r\n");
	
	fprintf(reg_file, "%s", response);
	print_file_to_string(path, response);
	
	char * mimimi = malloc(sizeof(char) * 500000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataHEAD(int result, int fd, char * response, FILE * reg_file, char connection[]) {
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);	
	switch (result){
		case 200:
			strcat(response, "200 OK\r\n");
			break;
		case 403:
			strcat(response, "403 FORBIDDEN\r\n");
			break;
		case 404:
			strcat(response, "404 NOT FOUND\r\n");
			break;
	}

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write_header(response, "Date: ", asctime (timeinfo), 1);

	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");

	// Pegar do parser
	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n");

	timeinfo = localtime(&lastmod);
	
	fprintf(reg_file, "%s", response);
	char * mimimi = malloc(sizeof(char) * 500000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataTRACE(int result, int fd, char * response, FILE * reg_file, char connection[]) {
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write_header(response, "Date: ", asctime (timeinfo), 1);

	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");

	// Pegar do parser
	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n");

	timeinfo = localtime(&lastmod);
	if(result==200) {
		write_header(response, "Last-Modified: ", asctime(timeinfo), 1);

		strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS\n");
		
		char * content_length;
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);		
		strcat(response, "Content-Type: text/html\r\n");

		strcat(response, "\r\n");

	}
	fprintf(reg_file, "%s", response);
	
	char * mimimi = malloc(sizeof(char) * 500000);
	strcpy(mimimi, response);
	return mimimi;
  
}

char * trataOPTIONS(int result, int fd, char * response, FILE * reg_file, char connection[]) {
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);	
	switch (result){
		case 200:
			strcat(response, "200 OK\r\n");
			break;
		case 403:
			strcat(response, "403 FORBIDDEN\r\n");
			break;
		case 404:
			strcat(response, "404 NOT FOUND\r\n");
			break;
	}

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write_header(response, "Date: ", asctime (timeinfo), 1);

	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");
	// Pegar do parser
	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n");

	timeinfo = localtime(&lastmod);
	if(result==200) {
		write_header(response, "Last-Modified: ", asctime(timeinfo), 1);
		strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS\n");
		
		char * content_length;
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);
		strcat(response, "Content-Type: text/html\r\n");
		strcat(response, "\r\n");

	}
	
	fprintf(reg_file, "%s", response);
	
	char * mimimi = malloc(sizeof(char) * 500000);
	strcpy(mimimi, response);

	return mimimi;
}

// Erro 501 - Not Implemented
char * trataNotImplemented(int fd, char * response, FILE * reg_file, char connection[]) {
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);	
	strcat(response, "501 NOT IMPLEMENTED\r\n");

	printf(reg_file, "%s\n", response );
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	write_header(response, "Date: ", asctime (timeinfo), 1);
	fprintf(reg_file, "Date: %s", asctime (timeinfo) );

	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");
	fprintf(reg_file, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");

	// Pegar do parser
	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n");
	fprintf(reg_file, "Connection: PEGAR DO PARSER!!!!\r\n");

	timeinfo = localtime(&lastmod);
	write_header(response, "Last-Modified: ", asctime(timeinfo), 1);
	fprintf(reg_file, "Last-Modified: %s", asctime(timeinfo));

	strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS\n");
	
	char * content_length;
	snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
	
	write_header(response, "Content-Lenght: ", content_length, 0);
	fprintf(reg_file, "Content-Lenght: %zd\n", fileStat.st_size, 1);
	
	strcat(response, "Content-Type: text/html\r\n");
	fprintf(reg_file, "Content-Type: text/html\r\n");

	strcat(response, "\r\n");
	
	char * error_page = malloc(sizeof(char) * 500000);
	strcpy(error_page, original_local);
	strcat(error_page, "/501.html"); 
	
	fprintf(reg_file, "%s", response);
	print_file_to_string(error_page, response);
	
	char * mimimi = malloc(sizeof(char) * 500000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataMetodo(char *metodo, int result, int fd, char * response, FILE * reg_file, char connection[]){
	char * resp;
	if(strcmp(metodo, "GET")==0){
		resp = trataGET(result, fd, response, reg_file, connection);
	}
	else if(strcmp(metodo, "HEAD")==0){
		resp = trataHEAD(result, fd, response, reg_file, connection);
	}
	else if(strcmp(metodo, "TRACE")==0){
		resp = trataTRACE(result, fd, response, reg_file, connection);
	}
	else if(strcmp(metodo, "OPTIONS")==0){
		resp = trataOPTIONS(result, fd, response, reg_file, connection);
	}
	else {
		resp = trataNotImplemented(fd, response, reg_file, connection);
	}
	
	return resp;
}

char * acesso(char *local, char *recurso, char *metodo, char * response, FILE * reg_file, char connection[]){
	struct stat statarq;
	int fd;
	DIR *dir;
	
	/*****Trecho para pegar o path do arquivo e incluí-lo em um só*****/

	strcpy(original_local, local);
	strcpy(path, local); //copia o local passado ao path
	if (strncmp("/", recurso, 1) == -1) {
		strcat(path, "/");//adicona '/' ao path
	}
	
	strcat(path, recurso);  //cooloca o nome do recurso depois do path
	
			printf("[FILE_MANAGER] ADICIONANDO BARRA %s\n%s\n", path, recurso);

	
	printf("[FILE_MANAGER] Recurso a ser localizado: %s\n", path);
	printf("[FILE_MANAGER] local: %s\n", local);
	printf("[FILE_MANAGER] metodo: %s\n", metodo);
	//printf("\no path ta aqui: %s\n", path);

	/*****Trecho para buscar o recurso dentro do path passado*****/
	//caso não ache o recurso
	
	if (stat(path, &statarq) == -1){
		strcpy(path, local);
		strcpy(recurso, "/404.html");
		strcat(path, recurso);

		return trataMetodo(metodo, 404, fd, response, reg_file, connection);
		//return 404;
	}
	else{
		//printf("access %d",access(path, R_OK));
		//caso o recurso não dê permissão de acesso
		if(access(path, R_OK) != 0){
			strcpy(path, local);
			strcpy(recurso, "/403.html");
			strcat(path, recurso);
			
			return trataMetodo(metodo, 403, fd, response, reg_file, connection);
			//return 403;
		}
		//recurso existe e tem permissão de acesso
		else{
			/*****Trecho para ler e tratar se é um arquivo ou diretório*****/
			
			//caso seja um arquivo
			if((statarq.st_mode & S_IFMT)==S_IFREG){
				//Abre e imprime o arquivo
				fd = open(path, O_RDONLY, 0600);
				return trataMetodo(metodo, 200, fd, response, reg_file, connection);
			}
			else{
				//caso seja diretório
				if((statarq.st_mode & S_IFMT)==S_IFDIR){
					//caso o diretório não tenha permissão de acesso
					if(access(path, X_OK) != 0) {
						
						strcpy(path, local);
						strcpy(recurso, "/403.html");
						strcat(path, recurso);
				
						return trataMetodo(metodo, 403, fd, response, reg_file, connection);
						//return 403;
					} else {
						struct dirent *newDir;
						//abre o diretório
						dir = opendir(path);
						//le os arquivos até achar um index ou welcome
						while(newDir = readdir(dir)){
							
							printf("%s\n",newDir->d_name);
							
							if(strcmp("index.html",newDir->d_name)==0){
								strcpy (recurso, "index.html");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0) {
									strcpy(path, local);
									strcpy(recurso, "/403.html");
									strcat(path, recurso);
				
									return trataMetodo(metodo, 403, fd, response, reg_file, connection);
									//return 403;
								}
								else{
									printf("[FILE_MANAGER] FD FD FD %d", fd);
									
									fd = open(path, O_RDONLY, 0600);
									//write(fd,"", 50);
									//write(fd,path, sizeof(path));
									return trataMetodo(metodo, 200, fd, response, reg_file, connection);
									//return 0;
								}
								break;
							}
							if(strcmp("welcome.html",newDir->d_name)==0){
								strcpy (recurso, "welcome.html");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0){
									strcpy(path, local);
									strcpy(recurso, "/403.html");
									strcat(path, recurso);
				
									return trataMetodo(metodo, 403, fd, response, reg_file, connection);
									//return 403;
								}
								else{
									fd = open(path, O_RDONLY, 0600);
									//write(fd,"", 50);
									write(fd,path, sizeof(path));
									return trataMetodo(metodo, 200, fd, response, reg_file, connection);
									//return 0;
								}
								break;
							}
						}
						strcpy(path, local);
						strcpy(recurso, "/404.html");
						strcat(path, recurso);
						printf("404 da massa!!! ================= %s \n", path);
						//caso não tenha achado nenhum arquivo index ou welcome
						return trataMetodo(metodo, 404, fd, response, reg_file, connection);
						//return 404;
					}
				}
			}
		}
	}
}