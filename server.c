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
#include <pthread.h>
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
//int max_processos = 3;
//int processo;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int tot_thread, num_thread;
int max_thread = 3;
pthread_t thread[3]; //ric

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

void * req_to_server(){
	
	//A porta que utilizaremos é 8585
	unsigned short porta = 8585;
	char request[1024];
	char * response;
	struct sockaddr_in cliente;	
	struct sockaddr_in servidor;	
	int soquete;			
	int novo_soquete;		
	int nome_compr;			
	int mensagem_compr;		
	int i, j;			//j vai controlar o numero de requições no arquivo. nesse caso, apenas uma.
	//int n;
	//struct timeval timeout;
	//fd_set fds;
	//FD_ZERO(&fds);
	//timeout.tv_sec = 3; /* máximo de tempo para a requisição */
	//timeout.tv_usec = 0; /* */
	
	printf("[SERVER] Iniciando requisição ao server...");
	
	if ((soquete = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Erro em socket()");
		exit(2);
	}

	pthread_mutex_lock(&lock);
	servidor.sin_family 	= AF_INET;
	servidor.sin_port   	= htons(porta);
	servidor.sin_addr.s_addr	= INADDR_ANY;
//pthread_t thread[3]; //
	while (bind(soquete, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
	{
	perror(".");
	//exit(3);
	}
	printf("[SERVER] Voa\n");

	
	if (listen(soquete, 5) != 0)
	{
		perror("Erro em listen().\n");
		exit(4);
	}
	printf("NÚMERO DA THREAD: %d\n", num_thread);
	printf("total DA THREAD: %d\n", tot_thread);
	printf("SOCKET: %d", soquete);
	//FD_SET(soquete, &fds);
	j=0;
	//n = select(1, &fds, (fd_set *)0, (fd_set *)0, &timeout);
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

		printf("%s", request);
		yy_scan_string(request);
		yyparse();

		// Printa a lista
		print_request_to_file(reg_file);

		/* Funcao para obter resultado do parser */
		result = get_parsed_request();

		
		response = malloc(sizeof(char) * 500000);
		char * mimimi = malloc(sizeof(char) * 500000);
		
		
		command_list * method = result;
		command_list * connection = get_command_by_name("Connection");
		printf("[REQ2SERVER] CHEGOU AQUI!!!!!!!!!!\n");
		printf("[REQ2SERVER] webspace = %s\n", webspace);
		printf("[REQ2SERVER] recurso = %s\n", method->params->param);
		printf("[REQ2SERVER] method = %s\n", method->command);
		printf("[REQ2SERVER] response = %s\n", response);

		mimimi = acesso(webspace, method->params->param, method->command, &response, reg_file, connection->params->param);
		printf("[REQ2SERVER] PASSOU DO ACESSO\n");


		write(novo_soquete, mimimi, strlen(mimimi));
		//sleep(10);
		j++;
	}
	tot_thread--;
	pthread_mutex_unlock(&lock);
	close(soquete);
	//printf("O servidor terminou com erro.\n");
	//exit(5);
	
	//printf("P %d RETURN KKKKKKK ===========\n", processo);
	//sleep(10);
	//exit(0);
	pthread_exit(NULL);
	return;
}


void print_request_to_file(FILE * reg_file) {
	int first_command = 0;
	char separator[] = " ";
	
	fprintf(reg_file, "\n");
	command_list * current = list;
	
	while (current != NULL) {//pthread_t thread[3]; //
		
		//printf("[SERVER] Comando: %s\n", current->command);
		fprintf(reg_file, "%s: ", current->command);
		
		if ((++first_command) == 1) {
			strcpy(separator, " ");
		} else {
			strcpy(separator, ", ");
		}
		
		param_list *current_param = current->params;
		while(current_param != NULL){
			
			//printf("[SERVER]     |--> Parâmetro: %s\n", current_param->param);
			if (current_param->next == NULL) {
				fprintf(reg_file, "%s\n", current_param->param);
			} else {
				fprintf(reg_file, "%s%s", current_param->param, separator);
			}
			current_param = current_param->next;
		}

		current = current->next;
	}
	fprintf(reg_file, "\n");
	
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

void cria_thread(){
	int pid;
	int bla = 0;
	/*char *area;
	int pid = fork();
	if(pid==0){
		
		printf("====================================================vai chamar REQ2SERVER PARA O PROCESSO %d\n", processo);
		area = req_to_server();
		printf("====================================================FOI CHAMADO REQ2SERVER PARA O PROCESSO %d\n", processo);
		processo++;
		execlp("cal","cal",processo,NULL);
		//exit(pid);
	}else{
		printf("Não criou outro processo pois o fork não funcionou\n");
	}*/
	pthread_mutex_lock(&lock); //mutex varivael global compartilhada (numberOfReqs) ric
	num_thread = tot_thread;//ric
	pthread_mutex_unlock(&lock);//ric
	if(num_thread>=max_thread){
		printf("cria um jeito. tem que levar pro socket tbm mas responder outra coisa\n");
	}
	else{
		pid = pthread_create(&thread[tot_thread], NULL, &req_to_server, NULL); //cria uma nova THREAD
		if (pid) {
			printf("\n ERRO: codigo de retorno de pthread_create eh %d \n", pid);
			exit(1);
		}
		pthread_mutex_lock(&lock); 
		tot_thread++; //incrementa o contador de TREADS'
		pthread_mutex_unlock(&lock);
	}
	
	
	
	return;
}

int main(int argc, char** argv)
{
	char *path = argv[2];
	char *arq_req = argv[3];
	char *arq_res = argv[4];
	char *arq_reg = argv[5];
	char *area;
	int processo = 0;
	//pthread_t thread[3]; //ric
	int r , i , j , sz ;
	char *req;
	pthread_mutex_init(&lock, NULL);//ric
	tot_thread=0;
	num_thread=0;
	
	webspace = argv[1];


	printf("\n[SERVER] Starting program to call the parser and process a request...\n\n");
		
	printf("[SERVER] Argumentos:\n[SERVER]  (ENV) webspace: %s\n[SERVER]  (1) path: %s\n[SERVER]  (2) arq_res: %s\n[SERVER]  (3) arq_reg: %s\n\n", webspace, path, arq_res, arq_reg);
	
	if((resp_file = fopen(arq_res, "w")) == NULL){
		printf("[SERVER] Error to open file %s.\n", arq_res);
		exit (0);
	}
	
	if((reg_file = fopen(arq_reg, "a")) == NULL){
		printf("[SERVER] Error to open file %s.\n", arq_reg);
		exit (0);
	}
	
	/*while(processo<10){
		processo++;
		cria_thread();
		printf("CADE O RESTO PORRAAAAAAAAAAA\n");
		sleep(10);
	}*/
	req_to_server();
	
	printf("[SERVER] Requisição ao server feita.");

	/* Fechando o arquivo... */
	close(resp_file);
	close(reg_file);

	printf("\n[SERVER] Finished!\n");
	
	return 0;
}