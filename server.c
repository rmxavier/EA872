#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include "y.tab.h"
#include "server.h"
#include "file_manager.h"

/**
 * Para compilar o codigo
 * flex parser.l
 * yacc -d parser.y
 * gcc -o saida server.c y.tab.c lex.yy.c -ly -lfl
 */

char value[1024];
char resource[1024];
int varindex = 0;
int have_host = 0;
int InfoViaPostRecived = 0;

FILE * req_file;
FILE * resp_file;
FILE * reg_file;

struct command_list * list = NULL;

struct command_list *result = NULL;

int error;
char *webspace;

command_list * get_parsed_request() {
	return list;
}

command_list * get_command_by_name(char command[]) {

	command_list * current = list;
	
	// Iterates over the command list looking for a command that matches the parameter
	while (current != NULL) {
		if (strcmp(current->command, command) == 0) {
			break;
		}

		current = current->next;
	}
	return current;
}

const char *get_content_type(char *filename_param) {
	char * filename = malloc(sizeof(char)*1024);;
	strcpy(filename, filename_param);
	
	char *content_type = malloc(sizeof(char)*1024);
	const char *dot = strrchr(filename, '.');
	
	if(!dot || dot == filename || sizeof(filename) == sizeof(char)) {
		strcpy(content_type,"text/html");
	} else {
		content_type = dot + 1;
		
		if (strcmp(content_type, "html") == 0 || strcmp(content_type, "htm") == 0) {
			strcpy(content_type, "text/html");
		} else if (strcmp(content_type, "jpg") == 0 || strcmp(content_type, "jpeg") == 0) {
			strcpy(content_type, "image/jpeg");
		} else if (strcmp(content_type, "png") == 0 || strcmp(content_type, "png") == 0) {
			strcpy(content_type, "image/png");
		} else if (strcmp(content_type, "ico") == 0 || strcmp(content_type, "ico") == 0) {
			strcpy(content_type, "image/ico");
		} else if (strcmp(content_type, "gif") == 0 || strcmp(content_type, "gif") == 0) {
			strcpy(content_type, "image/gif");
		} else if (strcmp(content_type, "pdf") == 0) {
			strcpy(content_type, "application/pdf");
		} else if (strcmp(content_type, "txt") == 0) {
			strcpy(content_type, "text/plain");
		} else {
			strcpy(content_type, "text/html");
		}
	}
		
	return content_type;
}

char * req_to_server(char * port, char * arq_reg){
	
	//A porta que utilizaremos é 8585
	unsigned short porta = (unsigned short) strtoul(port, NULL, 0);
	char request[1024];
	char * response;
	struct sockaddr_in cliente;	
	struct sockaddr_in servidor;	
	int soquete;			
	int novo_soquete;		
	int nome_compr;			
	int mensagem_compr;		
	int i, j;			//j vai controlar o numero de requições no arquivo. nesse caso, apenas uma.
	int option = 1;
	int k = 0;
     
	while (1) {
		
		if((reg_file = fopen(arq_reg, "a")) == NULL){
			printf("[SERVER] Error to open file %s.\n", arq_reg);
			exit (0);
		}
		
		printf("[SERVER] Iniciando requisição ao server [%d]...\n", k);
		
		memset(request, 0, sizeof(request));
		list = NULL;
		result = NULL;
		
		if ((soquete = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Erro em socket()");
			exit(2);
		}
		
		// Para parar de dar erro no bind
		setsockopt(soquete, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

		
		servidor.sin_family 	= AF_INET;
		servidor.sin_port   	= htons(porta);
		servidor.sin_addr.s_addr	= INADDR_ANY;

		while (bind(soquete, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
		{
		perror(".///");
		//exit(3);
		}
		
		if (listen(soquete, 5) != 0)
		{
			perror("Erro em listen().\n");
			exit(4);
		}

		j=0;	
		while (j<1) {
		
			nome_compr = sizeof(cliente);
			if ((novo_soquete = accept(soquete, (struct sockaddr *)&cliente, &nome_compr)) == -1)
			{
				perror("Erro em accept().\n");
				break;
			}

			
			//if ((mensagem_compr = recv(novo_soquete, request, sizeof(request), 0)) == -1)
			if ((mensagem_compr = recv(novo_soquete, request, sizeof(request), 0)) == -1)
			{
				perror("Erro em recv().\n");
				break;
			}

			yy_scan_string(request);
			yyparse();

			// Printa a lista
			print_request_to_file(reg_file);

			
			
			/* Funcao para obter resultado do parser */
			result = get_parsed_request();

			response = malloc(sizeof(char) * 50000000);
			//response[0]='\0';
			memset(response, 0, sizeof(response));
			
			char * mimimi = malloc(sizeof(char) * 50000000);
			mimimi[0]='\0';
			
			command_list * method = result;
			
			command_list * connection = get_command_by_name("Connection");
			char * connection_string = malloc(sizeof(char) * 100);
			if (strcmp(connection->command, "Connection") != 0) {
				strcpy(connection_string, "keep-alive");
			} else {
				strcpy(connection_string, connection->params->param);
			}
			
			char * content_type_string = get_content_type(method->params->param);
						
			mimimi = acesso(webspace, method->params->param, method->command, &response, reg_file, connection_string, content_type_string, method->query_string);

			write(novo_soquete, mimimi, strlen(mimimi));
			
			j++;
		}
		
		/* Fechando o arquivo... */
		fflush(reg_file);
		close(reg_file);
		
		close(novo_soquete);
		close(soquete);
		k++;
	}
	//printf("O servidor terminou com erro.\n");
	//exit(5);
	
	return "Terminou";
}


void print_request_to_file(FILE * reg_file) {
	int first_command = 0;
	char separator[] = " ";
	
	fprintf(reg_file, "\n");
	command_list * current = list;
	
	
	while (current != NULL) {
		
		printf("[SERVER] Comando: %s\n", current->command);
		fprintf(reg_file, "%s: ", current->command);
		
		if ((++first_command) == 1) {
			strcpy(separator, " ");
		} else {
			strcpy(separator, ", ");
		}
		
		param_list *current_param = current->params;
		while(current_param != NULL){
			
			printf("[SERVER]     |--> Parâmetro: %s\n", current_param->param);
			if (current_param->next == NULL) {
				fprintf(reg_file, "%s\n", current_param->param);
			} else {
				fprintf(reg_file, "%s%s", current_param->param, separator);
			}

			current_param = current_param->next;
		}

		current = current->next;

	}
	
	if (strcmp(list-> query_string," ") != 0) {
		if (strcmp(list->command, "POST") == 0) {
			printf("[SERVER] Query String: %s\n", list->query_string);
		} else {
			printf("[SERVER] Body: %s\n", list->query_string);
		}
		fprintf(reg_file, "\n");
		fprintf(reg_file, list-> query_string);
		fprintf(reg_file, "\n");
	}
	
	fprintf(reg_file, "\n");
	
}

void add_command_list(char *command)
{
	//se a lista nao tiver sido criada, cria o primeiro elemento
	if(list == NULL){

    		list = malloc(sizeof(command_list));

    		strcpy(list->command, command);
		strcpy(list->query_string, " ");
    		list->next = NULL;
    		list->params = NULL;

    		return;
	}

	command_list * current = list;

	while (current->next != NULL) {
		current = current->next;
	}

	current->next = malloc(sizeof(command_list));
	strcpy(current->next->command, command);
	current->next->next = NULL;
	current->next->params = NULL;	

	return;
}

void add_query_string(char * query_string) {
	command_list * current = list;
	
	strcpy(list->query_string, query_string);

	return;
}


void add_param_list(char *param)
{
	command_list * current = list;
	param_list * current_param = NULL;

	while (current->next != NULL) {
		current = current->next;
	}

	//se a lista de parametros nao tiver sido criada, cria o primeiro elemento
	if(current->params == NULL){

		current->params = malloc(sizeof(param_list));

		strcpy(current->params->param, param);
		current->params->next = NULL;

		return;
	}

	param_list * new_node;
	new_node = malloc(sizeof(param_list));

	strcpy(new_node->param, param);
	
	current_param = current->params;
	while (current_param->next != NULL) {
		current_param = current_param->next;
	}
	
	current_param->next = new_node;
	new_node->next = NULL;

	return;
}

int main(int argc, char** argv)
{
	char *port = argv[2];
	char *arq_reg = argv[3];
	char *area;
	
	int r , i , j , sz ;
	char *req;
	
	webspace = argv[1];

	printf("\n[SERVER] Starting program to call the parser and process a request...\n\n");
		
	printf("[SERVER] Arguments:\n[SERVER]  (ENV) webspace: %s\n[SERVER]  (1) PORT: %s\n[SERVER]  (2) arq_reg: %s\n\n", webspace, port, arq_reg);
	
	if((reg_file = fopen(arq_reg, "a")) == NULL){
		printf("[SERVER] Error to open file %s.\n", arq_reg);
		exit (0);
	} else {
		fprintf(reg_file, "Iniciando log %s...\r\n\r\n", arq_reg);
		fflush(reg_file);
		close(reg_file);
	}
	
	area = req_to_server(port, arq_reg);
	

	printf("\n[SERVER] Finished!\n");
	
	return 0;
}