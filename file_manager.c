
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
char original_resource[1075];

void print_file_to_string(int result, char * fname, char * response) {
	
	printf("PRINTANDO ARQUIVO %s NA RESPONSE!!\n", fname);
	
	switch (result){
		case 200:
			break;
		case 403:
			memset(fname, 0, sizeof(fname));
			strcpy(fname, "error_pages/403.html");		
			break;
		case 404:
			memset(fname, 0, sizeof(fname));
			strcpy(fname, "error_pages/404.html");
			break;
		case 501:
			memset(fname, 0, sizeof(fname));
			strcpy(fname, "error_pages/501.html");
			break;
	}
	printf("Printing %s to string (status %d)\n", fname, result);
	
	int fh;
        char buffer[64];
        int gotten;
	int total = 0;
	
	char * mimimi = malloc(sizeof(char) * 5000000);
	mimimi[0] = '\0';
	printf(">>>>>>>>>>>>>>MIMI\n%s))))))))))\n", mimimi);
	printf(">>>>>>>>>>>>>>RESP\n%s))))))))))\n", response);

        fh = open(fname,O_RDONLY);
        printf ("!!!!!!!!!!!!!!!File handle %d\n",fh);
	
	
        while (gotten = read(fh,buffer,64)) {
		total = total + gotten;
		//buffer[gotten] = '\0';
		//printf("%d|%d|%s\n", total, gotten, buffer);
		strcat(mimimi, buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	
	char * new_mimimi = malloc(sizeof(char)*strlen(mimimi)+sizeof(char)*strlen(response));
	strcpy(new_mimimi, mimimi);
	
	printf("\n\nTERMINOU DE IMPRIMIR ESSA MERDA\n\n");
	printf("RESPONSE %s\n\nTAMANHO DA CARALHA:%d\n", response, strlen(response));
	new_mimimi[strlen(new_mimimi)]='\0';
	strcat(new_mimimi, response);
	strcat(new_mimimi, mimimi);
	printf("\n\nTERMINOU DE CONCATENAR ESSA MERDA\n\n");

	printf("%s\nTAMANHO DAs KRLAs!!! %d; %d; %d\n", mimimi, strlen(response), strlen(mimimi), total);
}

void write_header(char * response, char * header, char * info, int suppress_last_character) {
	
	if (suppress_last_character) {
		info[strlen(info) - 1] = '\0';
	}
	
	strcat(response, header);
	strcat(response, info);
	strcat(response, "\r\n");
}

char * trataGET(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type){
		
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
		
		char content_length[1024];
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
	
		write_header(response, "Content-Lenght: ", content_length, 0);
		write_header(response, "Content-Type: ", content_type, 0);
	}
	
	strcat(response, "\r\n");

	fprintf(reg_file, "%s", response);
	
	print_file_to_string(result, path, response);

	char * mimimi = malloc(sizeof(char) * 50000000);
	
	memset(mimimi, 0, sizeof(mimimi));
	strcpy(mimimi, response);
	return mimimi;
}

char * trataHEAD(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type) {
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
	char * mimimi = malloc(sizeof(char) * 50000000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataTRACE(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type) {
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

		strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS, PUT, POST, DELETE\n");
		
		char content_length[1024];
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);		
		write_header(response, "Content-Type: ", content_type, 0);

		strcat(response, "\r\n");

	}
	fprintf(reg_file, "%s", response);
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	strcpy(mimimi, response);
	return mimimi;
  
}

char * trataOPTIONS(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type) {
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
		strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS, PUT, POST, DELETE\n");
		
		char content_length[1024];
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);
		write_header(response, "Content-Type: ", content_type, 0);
		strcat(response, "\r\n");

	}
	
	fprintf(reg_file, "%s", response);
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	strcpy(mimimi, response);

	return mimimi;
}

// Erro 501 - Not Implemented
char * trataNotImplemented(int fd, char * response, FILE * reg_file, char * connection, char * content_type) {
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
	fprintf(reg_file, "Connection: %s\r\n", connection);

	timeinfo = localtime(&lastmod);
	write_header(response, "Last-Modified: ", asctime(timeinfo), 1);
	fprintf(reg_file, "Last-Modified: %s", asctime(timeinfo));

	strcat(response, "Allow: GET, HEAD, TRACE, OPTIONS\n");
	
	char content_length[1024];
	snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
	
	write_header(response, "Content-Lenght: ", content_length, 0);
	fprintf(reg_file, "Content-Lenght: %zd\n", fileStat.st_size, 1);
	
	write_header(response, "Content-Type: ", content_type, 0);
	fprintf(reg_file, "Content-Type: %s\r\n", content_type);

	strcat(response, "\r\n");
	
	char * error_page = malloc(sizeof(char) * 50000000);
	strcpy(error_page, original_local);
	strcat(error_page, "/501.html"); 
	
	fprintf(reg_file, "%s", response);
	print_file_to_string(501, error_page, response);
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataPOST(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type, char * request_body){
	
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
		
		char content_length[1024];
		snprintf(content_length, sizeof(content_length), "%zd", fileStat.st_size);
		
		write_header(response, "Content-Lenght: ", content_length, 0);
		write_header(response, "Content-Type: ", content_type, 0);
	}

	strcat(response, "\r\n");
	
	fprintf(reg_file, "%s", response);
	print_file_to_string(result, path, response);
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	strcpy(mimimi, response);

	return mimimi;
}

char * trataDELETE(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type){
		
	strcpy(response, "HTTP/1.1 ");
	time_t rawtime;
	time_t lastmod = fileStat.st_mtime; 
	struct tm * timeinfo;

	fstat(fd, &fileStat);	

	strcat(response, "204 NO CONTENT\r\n");
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	write_header(response, "Date: ", asctime (timeinfo), 1);
	
	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");
	strcat(response, "Connection: ");
	strcat(response, connection);
	strcat(response, "\r\n\r\n");

	fprintf(reg_file, "%s", response);
		
	if (remove(path) == 0) {
		printf("[FILE_MANAGER] Removido com sucesso\n");
	} else {
		printf("[FILE_MANAGER] Removido sem sucesso\n");
	}
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	
	memset(mimimi, 0, sizeof(mimimi));
	strcpy(mimimi, response);
	return mimimi;
}

char * trataPUT(int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type, char * request_body){
	
	FILE * new_file;
	
	strcpy(response, "HTTP/1.1 ");
	strcat(response, "HTTP/1.1 201 Created\r\n");
	write_header(response, "Content-Location: ", original_resource, 0);
	
	strcat(response, "Server: Servidor HTTP ver 0.1 dos Descolados\r\n");
	strcat(response, "\r\n");

	fprintf(reg_file, "%s", response);
		
	if (remove(path) == 0) {
		printf("[FILE_MANAGER] Removido com sucesso\n");
	} else {
		printf("[FILE_MANAGER] Removido sem sucesso\n");
	}
	
	if((new_file = fopen(path, "w")) == NULL){
		printf("[SERVER] Error to open file %s.\n", path);
		exit (0);
	}
	
	// Escreve o conteudo do arquivo
	fprintf(new_file, request_body);
	
	fclose(new_file);
	
	char * mimimi = malloc(sizeof(char) * 50000000);
	
	memset(mimimi, 0, sizeof(mimimi));
	strcpy(mimimi, response);
	return mimimi;
}


char * trataMetodo(char *metodo, int result, int fd, char * response, FILE * reg_file, char * connection, char * content_type, char * request_body){
	char * resp;
		
	if(strcmp(metodo, "GET")==0){
		resp = trataGET(result, fd, response, reg_file, connection, content_type);
	}
	else if(strcmp(metodo, "HEAD")==0){
		resp = trataHEAD(result, fd, response, reg_file, connection, content_type);
	}
	else if(strcmp(metodo, "TRACE")==0){
		resp = trataTRACE(result, fd, response, reg_file, connection, content_type);
	}
	else if(strcmp(metodo, "OPTIONS")==0){
		resp = trataOPTIONS(result, fd, response, reg_file, connection, content_type);
	}
	else if (strcmp(metodo, "POST")==0) {
		resp = trataPOST(result, fd, response, reg_file, connection, content_type, request_body);
	}
	else if (strcmp(metodo, "DELETE")==0) {
		resp = trataDELETE(result, fd, response, reg_file, connection, content_type);
	}
	else if (strcmp(metodo, "PUT")==0) {
		resp = trataPUT(result, fd, response, reg_file, connection, content_type, request_body);
	}
	else {
		resp = trataNotImplemented(fd, response, reg_file, connection, content_type);
	}
	
	return resp;
}

char * acesso(char *local, char *recurso, char *metodo, char * response, FILE * reg_file, char * connection, char * content_type, char * request_body){
	struct stat statarq;
	int fd;
	DIR *dir;
	
	/*****Trecho para pegar o path do arquivo e incluí-lo em um só*****/

	memset(original_local, 0, sizeof(original_local));
	memset(original_resource, 0, sizeof(original_resource));
	
	strcpy(original_local, local);
	strcpy(original_resource, recurso);
	strcpy(path, local); //copia o local passado ao path
	if (strncmp("/", recurso, 1) == -1) {
		strcat(path, "/");//adicona '/' ao path
	}
	
	strcat(path, recurso);  //cooloca o nome do recurso depois do path
	
	printf("[FILE_MANAGER] Recurso a ser localizado: %s\n", path);
	printf("[FILE_MANAGER] local: %s\n", local);
	printf("[FILE_MANAGER] metodo: %s\n", metodo);
	//printf("\no path ta aqui: %s\n", path);

	// Trecho para buscar o recurso dentro do path passado
	//caso não ache o recurso
	
	printf("METODO --------[%s]\n", metodo);
	
	if (strcmp(metodo, "DELETE") == 0) {
		printf("\n---- Metodo DELETE ---\n");
		
		trataMetodo(metodo, 204, fd, response, reg_file, connection, content_type, request_body);
	}
	else if (stat(path, &statarq) == -1){
		return trataMetodo(metodo, 404, fd, response, reg_file, connection, content_type, request_body);
	}
	else{
		//printf("access %d",access(path, R_OK));
		//caso o recurso não dê permissão de acesso
		if(access(path, R_OK) != 0){
			return trataMetodo(metodo, 403, fd, response, reg_file, connection, content_type, request_body);
		}
		//recurso existe e tem permissão de acesso
		else{
			// Trecho para ler e tratar se é um arquivo ou diretório
			//caso seja um arquivo
			if((statarq.st_mode & S_IFMT)==S_IFREG){
				//Abre e imprime o arquivo
				fd = open(path, O_RDONLY, 0600);
				return trataMetodo(metodo, 200, fd, response, reg_file, connection, content_type, request_body);
			}
			else{
				//caso seja diretório
				if((statarq.st_mode & S_IFMT)==S_IFDIR){
					//caso o diretório não tenha permissão de acesso
					if(access(path, X_OK) != 0) {
				
						return trataMetodo(metodo, 403, fd, response, reg_file, connection, content_type, request_body);
						//return 403;
					} else {
						struct dirent *newDir;
						//abre o diretório
						dir = opendir(path);
						//le os arquivos até achar um index ou welcome
						while(newDir = readdir(dir)){

							if(strcmp("index.html",newDir->d_name)==0){
								strcpy (recurso, "index.html");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0) {
									return trataMetodo(metodo, 403, fd, response, reg_file, connection, content_type, request_body);
								}
								else{									
									fd = open(path, O_RDONLY, 0600);
									return trataMetodo(metodo, 200, fd, response, reg_file, connection, content_type, request_body);
								}
								break;
							}
							if(strcmp("welcome.html",newDir->d_name)==0){
								strcpy (recurso, "welcome.html");
								strcat(path, recurso);
								//o arquivo index foi encontrado, mas não tem permissao
								if(access(path, R_OK) != 0){
									return trataMetodo(metodo, 403, fd, response, reg_file, connection, content_type, request_body);
								}
								else{
									fd = open(path, O_RDONLY, 0600);
									//write(fd,"", 50);
									write(fd,path, sizeof(path));
									return trataMetodo(metodo, 200, fd, response, reg_file, connection, content_type, request_body);
								}
								break;
							}
						}
						//caso não tenha achado nenhum arquivo index ou welcome
						return trataMetodo(metodo, 404, fd, response, reg_file, connection, content_type, request_body);
					}
				}
			}
		}
	}
}