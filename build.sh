lex parser.l
yacc -d parser.y
gcc -w -o server server.c y.tab.c file_manager.c lex.yy.c -ly -lfl
./server $WEBSPACE $1 $2 $3 $4

