# Instruções de uso
# ===========================================================

# A) CONFIGURAÇÕES PADRÃO: COMPILAÇÃO E EXECUÇÃO
# ##

# 1) Primeiramente, é necessário que haja uma variável de ambiente denotando o endereço do seu web space, isto é, onde ficam os arquivos que serão acessados via HTTP quando o servidor estiver funcionando
#	Para tal, execute o comando a seguir, substituindo <endereco-diretorio> pelo endereço completo de onde estão hospedadas suas páginas
export WEBSPACE=<endereco-diretorio>

# 2) Criamos um shell script que encapsula todos os processos necessários para a execução, incluindo compilação dos arquivos
#	O webspace será dado pela variável de sessão WEBSPACE, o arquivo de log será reg.txt e a porta 8585
# >> ./build.sh
./build.sh 

# ===========

# B) CONFIGURAÇÕES ESPECÍFICAS: EXECUÇÃO SOMENTE

# 1) Considerando que o server já está compilado, executar o comando abaixo, substituindo as informações entre "<>"
./startup.sh <webspace> <port> <arquivo de log>