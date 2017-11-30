%{
	// Includes
	#include "y.tab.h"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	// Variaveis de controle
	extern int lineno;
	char param[250] = "";
	int i_param = 0;
	int has_token_separator = 0;
	char token_separator = '\0';
	int has_query_string = 0;
	int query_string = 0;

%}

%%

^([\r|\n]*[\.][/]http).*[\n|\r]*	{
}

^([\r|\n]*Mensagem).*[\r|\n]*	{
}

"GET "       {
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: GET\n");
	return GET;
}

"TRACE "       {
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: TRACE\n");
	return TRACE;
}

"HEAD "       {
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: HEAD\n");
	return HEAD;
}

"OPTIONS "       {
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: OPTIONS\n");
	return OPTIONS;
}

"PATCH "       {
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: PATCH\n");
	return PATCH;
}

"POST"		{
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 1;
	printf("[LEXX]    -> Command: POST\n");
	return POST;
}

"PUT"		{
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 1;
	printf("[LEXX]    -> Command: PUT\n");
	return PUT;
}

"DELETE"		{
	has_token_separator = 1;
	token_separator = ' ';
	has_query_string = 0;
	printf("[LEXX]    -> Command: DELETE\n");
	return DELETE;
}

"Host: "              {
	has_token_separator = 0;
	token_separator = '\n';
	printf("[LEXX]    -> Command: host\n");
	return HOST;
}
"User-Agent: "        {
	has_token_separator = 0;
	token_separator = '\n';
	printf("[LEXX]    -> Command: user-agent\n");
	return USER_AGENT;
}
"Accept: "            {
	has_token_separator = 1;
	token_separator = ',';
	printf("[LEXX]    -> Command: accept\n");
	return ACCEPT;
}
"Accept-Language: "   {
	has_token_separator = 1;
	token_separator = ',';
	printf("[LEXX]    -> Command: accept-language\n");
	return ACCEPT_LANGUAGE;
}
"Accept-Encoding: "   {
	has_token_separator = 1;
	token_separator = ',';
	printf("[LEXX]    -> Command: accept-encoding\n");
	return ACCEPT_ENCODING;
}
"Content-Length: "   {
	has_token_separator = 1;
	token_separator = ',';
	printf("[LEXX]    -> Command: content-length\n");
	return CONTENT_LENGTH;
}
"Content-Type: "   {
	has_token_separator = 1;
	token_separator = ',';
	printf("[LEXX]    -> Command: content-type\n");
	return CONTENT_TYPE;
}
"Connection: "        {
	has_token_separator = 0;
	token_separator = '\n';
	printf("[LEXX]    -> Command: connection\n");
	return CONNECTION;
}
"Upgrade-Insecure-Requests: " {
	has_token_separator = 0;
	token_separator = '\n';
	printf("[LEXX]    -> Command: upgrade-insecure-requests\n");
	return UPGRADE_INSECURE_REQUESTS;
}

\n\n	{
	if (has_query_string == 0) {
		printf("[LEXX]\n[LEXX]---- [END1] Line number: %d ----\n[LEXX]\n", lineno);
		return END;
	} else {
		query_string = 1;
	}
}
\n\r\n	{
	if (has_query_string == 0) {
		printf("[LEXX]\n[LEXX]---- [END2] Line number: %d ----\n[LEXX]\n", lineno);
		return END;
	} else {
		query_string = 1;
	}
}
<<eof>>	{
	if (query_string == 1) {
		printf("[LEXX]    -> Query string: %s\n", param);
		yylval.token = strdup(param);
		memset(param, 0, strlen(param));
		i_param = 0;

		return QUERY_STRING;
	} else {
		
		printf("[LEXX] EOF found\n");
		return 0;
	}
}

.	{
	
	// Constrói o parâmetro
	if (yytext[0] != '\r' && yytext[0] != '\n' && !(has_token_separator && yytext[0] == token_separator)) {
		param[i_param++] = yytext[0];
	}

	// Imprime o parâmetro
	if (i_param > 0 && (yytext[0] == '\r' || yytext[0] == '\n' || (has_token_separator && yytext[0] == token_separator))) {

		printf("[LEXX]        |--> param: %s\n", param);
		yylval.token = strdup(param);
		memset(param, 0, strlen(param));
		i_param = 0;

		return PARAMETER;
	}
}

%%
