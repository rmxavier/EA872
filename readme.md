lex parser.l
yacc -d parser.y
gcc -w -o server server.c y.tab.c file_manager.c lex.yy.c -ly -lfl
./server /home/EE09/ra094349/Documentos/EA872/modulo09/ reqTrace.txt resp.txt reg.txt

