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

void req_to_server(FILE *arq_req){
	//A porta que utilizaremos é 8585
	unsigned short porta = 8585;
	char area[1024];			/* area para envio e recebimentede dados	*/
	struct sockaddr_in cliente;		/* estrutura de informações sobre os clientes	*/
	struct sockaddr_in servidor;	/* estrutura de informações sobre o servidor	*/
	int soquete;			/* soquete para aceitação de pedidos de conexão	*/
	int novo_soquete;			/* soquete de conexão aos clientes	*/
	int nome_compr;			/* comprimento do nome de um cliente	*/
	int mensagem_compr;			/* comprimento da mensagem recebida	*/
	int i;
     /* Ligue o soquete ao endereço do servidor.
	/*
	* Crie um soquete para aceitar pedidos de conexão.
	*/
	if ((soquete = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Erro em socket()");
		exit(2);
	}

	/*
	*/
	servidor.sin_family 	= AF_INET;
	servidor.sin_port   	= htons(porta);
	servidor.sin_addr.s_addr	= INADDR_ANY;

	if (bind(soquete, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
	{
	perror("Erro em bind().\n");
	exit(3);
	}

	/*
	* Aguarde por pedidos de conexão com um fila de até 5 pedidos.
	*/
	if (listen(soquete, 5) != 0)
	{
		perror("Erro em listen().\n");
		exit(4);
	}

	
	while (1) {

	/*
	* Aceite um pedido de conexão.
	*/
	nome_compr = sizeof(cliente);
	if ((novo_soquete = accept(soquete, (struct sockaddr *)&cliente, &nome_compr)) == -1)
	{
		perror("Erro em accept().\n");
		break;
	}

	/*
	* Receba a mensagem no novo soquete.
	*/
	if ((mensagem_compr = recv(novo_soquete, area, sizeof(area), 0)) == -1)
	{
		perror("Erro em recv().\n");
		break;
	}

	
	/* 
	* Imprima o que recebeu e feche a conexão.
	*/
	printf("\nMensagem recebida:\n");
	for(i = 0; i < mensagem_compr; i++) printf("%c", area[i]);
	fflush(arq_req);
	close(novo_soquete);

	}  /* Laço termina aqui */

	close(soquete); 
	printf("O servidor terminou com erro.\n");
	exit(5);
}

void print_request_to_file() {
	command_list * current = list;
	
	while (current != NULL) {
		
		printf("[SERVER] Comando: %s\n", current->command);
		fprintf(reg_file, "%s: ", current->command);
		
		param_list *current_param = current->params;
		while(current_param != NULL){
			printf("[SERVER]     |--> Parâmetro: %s\n", current_param->param);
			if (current_param->next == NULL) {
				fprintf(reg_file, "%s\n", current_param->param);
			} else {
				fprintf(reg_file, "%s,", current_param->param);
			}
			current_param = current_param->next;
		}

		current = current->next;
	}
	fprintf(reg_file, "\n");
	
	fclose(reg_file);
}

void add_command_list(char *command)
{
	//se a lista nao tiver sido criada, cria o primeiro elemento
	if(list == NULL){

    		list = malloc(sizeof(command_list));

    		strcpy(list->command, command);
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

command_list * get_parsed_request() {
	return list;
}

int main(int argc, char** argv)
{
	int error;
	struct command_list *result = NULL;
	char *webspace = argv[1];
	char *path = argv[2];
	char *arq_req = argv[3];
	char *arq_res = argv[4];
	char *arq_reg = argv[5];
	
	int r , i , j , sz ;
	char *req;

	printf("\nStarting program to call the parser and process a request...\n\n");
		
	printf("Argumentos:\n  (ENV) webspace: %s\n  (1) path: %s\n  (2) arq_req: %s\n  (3) arq_res: %s\n  (4) arq_reg: %s\n\n", webspace, path, arq_req, arq_res, arq_reg);

	//aqui eu chamo o req_to_server
	/* argv[1] -> Arquivo contendo a requisicao */
	if((req_file = fopen(arq_req, "r")) == NULL){
		printf("Error to open file %s.\n", arq_req);
		exit (0);
	}
	
	if((resp_file = fopen(arq_res, "w")) == NULL){
		printf("Error to open file %s.\n", arq_res);
		exit (0);
	}
	
	if((reg_file = fopen(arq_reg, "a")) == NULL){
		printf("Error to open file %s.\n", arq_reg);
		exit (0);
	}
	

	/* Leitura de dados e escrita no buffer */
	fseek(req_file, 0, SEEK_END);
	sz = (int) ftell(req_file);
	req = ( char *) malloc(sz+1);
	fseek(req_file, 0, SEEK_SET);
	fread (req, 1, sz, req_file);
	req[sz] = '\0';

	/* Processando a requisicao no buffer "req" usando o parser */
	printf("%s", req);
	yy_scan_string(req);
	yyparse();

	// Printa a lista
	print_request_to_file();

	/* Funcao para obter resultado do parser */
	result = get_parsed_request();
	
<<<<<<< Updated upstream
	error = acesso(webspace, result->params->param, result->command, resp_file);
=======
	error = acesso("/home/EC11/ra116006/meu-webspace", result->params->param, result->command, resp_file);
>>>>>>> Stashed changes

	/* Fechando o arquivo... */
	close(req_file);
	close(resp_file);
	close(reg_file);

	printf("\nFinished!\n");
}