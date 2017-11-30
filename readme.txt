# Instruções de uso
# ===========================================================

# 1) Primeiramente, é necessário que haja uma variável de ambiente denotando o endereço do seu web space, isto é, onde ficam os arquivos que serão acessados via HTTP quando o servidor estiver funcionando
#	Para tal, execute o comando a seguir, substituindo <endereco-diretorio> pelo endereço completo de onde estão hospedadas suas páginas
export WEBSPACE=<endereco-diretorio>

# 2) Criamos um shell script que encapsula todos os processos necessários para a execução, incluindo compilação dos arquivos
#	Os parâmetros entre "<>" devem ser substituidos. O script encontra-se na pasta principal do servidor, junto aos demais arquivos. Abaixo um exemplo:
# >> ./build.sh / reg.txt
./build.sh <path> <arquivo de logs>
