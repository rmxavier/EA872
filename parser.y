
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

void yyerror (char *s);

%}

%token NL CR COMMA END SLASH BLANK AND
%token GET TRACE OPTIONS HEAD PATCH POST PUT DELETE 
%token HOST USER_AGENT ACCEPT ACCEPT_LANGUAGE ACCEPT_ENCODING CONTENT_LENGTH CONTENT_TYPE CONNECTION UPGRADE_INSECURE_REQUESTS
%union {
	char * token;
}

%token <token> PARAMETER;
%%

requests: request		{printf("[YACC] Request\n");}		/* Apenas uma requisicao */
	| requests request	{printf("[YACC] MoreRequests\n");}	/* Varias requisicoes */
	;

request: method parameters headers ends	{printf("[YACC] ARequest\n");}	/* Estrutura da requisicao: metodo, parametros do metodo, headers (estrutura) e fim */
	| method parameters ends	{printf("[YACC] ARequest with no headers...\n");}
	| method parameters		{printf("[YACC] ARequest with no headers and no END (weird eh?!)...\n");}
	;

headers: header_structure		{printf("[YACC] HeaderStructure... \n");}		/* Header sem parametros */
	| headers header_structure	{printf("[YACC] MoreHeaders...\n");}				/* Varios headers */
	;

header_structure: 
	header			{printf("[YACC] Header... \n");}		/* Header sem parametros */
	| header parameters	{printf("[YACC] HeaderParams... \n");}		/* Header com parametros */
	;

method: /* Tipos de metodos */
	GET		{add_command_list("GET");printf("[YACC] Method (GET) \n");}
	| TRACE		{add_command_list("TRACE");printf("[YACC] Method (TRACE) \n");}
	| OPTIONS	{add_command_list("OPTIONS");printf("[YACC] Method (OPTIONS) \n");}
	| HEAD		{add_command_list("HEAD");printf("[YACC] Method (HEAD) \n");}
	| PATCH		{add_command_list("PATCH");printf("[YACC] Method (PATCH) \n");}
	| POST		{add_command_list("POST");printf("[YACC] Method (POST) \n");}
	| PUT		{add_command_list("PUT");printf("[YACC] Method (PUT) \n");}
	| DELETE	{add_command_list("DELETE");printf("[YACC] Method (DELETE) \n");}
	;

header: /* Tipos de header  */
	CONNECTION			{add_command_list("Connection"); printf("[YACC] Header (Connection)\n");}
	| USER_AGENT			{add_command_list("User-Agent");printf("[YACC] Header (UserAgent)\n");}
	| ACCEPT			{add_command_list("Accept");printf("[YACC] Header (Accept)\n");}
	| ACCEPT_ENCODING		{add_command_list("Accept-Encoding");printf("[YACC] Header (AcceptEncoding)\n");}
	| CONTENT_LENGTH		{add_command_list("Content-Length");printf("[YACC] Header (ContentLength)\n");}
	| CONTENT_TYPE			{add_command_list("Content-Type");printf("[YACC] Header (ContentType)\n");}
	| ACCEPT_LANGUAGE		{add_command_list("Accept-Language");printf("[YACC] Header (AcceptLanguage)\n");}
	| UPGRADE_INSECURE_REQUESTS	{add_command_list("Upgrade-Insecure-Requests");printf("[YACC] Header (UpgradeLanguage)\n");}
	| HOST				{add_command_list("Host");printf("[YACC] Header (Host)\n");}
	;

parameters: parameter		{printf("[YACC] Parameter... \n");}
	| parameters parameter	{printf("[YACC] MoreParameters...\n");}
      ;


parameter: PARAMETER	{add_param_list((char*)$1);printf("[YACC] #Parameter (%s)#\n",(char*)$1);}
	;

ends: END		{printf("[YACC] ENDFound\n");}
	| ends END	{printf("[YACC] ENDSFound\n");}
	;

%%


int lineno = 0;
extern char *yytext;

void yyerror (char *s)
{
	printf("[YACC] ERRORS:\n");
	printf("[YACC] http parser error: %s\n", s);
	printf("[YACC] line %d\n", lineno);
	printf("[YACC] last token read: |%s|\n", yytext);
}
