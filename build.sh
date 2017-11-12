#!/bin/sh
echo "[BUILD] Building server..."
echo "[BUILD] Iniciando lex ------------------------------------"
lex parser.l
echo "[BUILD] Iniciando yacc -----------------------------------"
yacc -d parser.y
echo "[BUILD] Compilando o server ------------------------------"
gcc -w -o server server.c y.tab.c file_manager.c lex.yy.c -ly -lfl
echo "[BUILD] Executando o server ------------------------------"
echo "./server $WEBSPACE $1 $2 $3 $4"
./server $WEBSPACE $1 $2 $3 $4

echo "[BUILD] Parâmetros utilizados:"
echo $WEBSPACE
echo $1
echo $2
echo $3
echo $4
