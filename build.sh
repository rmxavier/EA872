#!/bin/sh
echo "[BUILD] Building server..."
echo "[BUILD] Iniciando lex ------------------------------------"
lex parser.l
echo "[BUILD] Iniciando yacc -----------------------------------"
yacc -d parser.y
echo "[BUILD] Compilando o server ------------------------------"
gcc -w -o server server.c y.tab.c file_manager.c lex.yy.c -ly -lfl
echo "[BUILD] Executando o server ------------------------------"
echo "./server $WEBSPACE $1"
./server $WEBSPACE 8585 reg.txt

echo "[BUILD] Parâmetros utilizados:"
echo $WEBSPACE
echo $1
echo $2
