#!/bin/sh
echo "[BUILD] Executando o server ------------------------------"
echo "./server $WEBSPACE $1 $2"
./server $1 $2 $3

echo "[BUILD] Parâmetros utilizados:"
echo $WEBSPACE
echo $1
echo $2
