# threadcomparing
# Aluno: Samuel Natã de França Borges

Para compilar utilize a seguinte linha de comando:
g++ -std=c++11 main.cpp -o exe -lpthread

Ou atravez do makefile utilizando o comando:
make compile

Para executar, utilize o cmando:
./exe


Altere o valor do define AUTORUN para true caso queira rodar todos os testes automaticamente, imprimindo seus respectivos resultados nas pastas:

'printingMatrixTime' - contem o tempo de impressão da matriz resultado em arquivo.
'readingFileTime' - contem o tempo de leitura das duas matrizes a serem multiplicadas em arquivo.
'resultMatrix' - contem a matriz resultado da multiplicação
'SolutionTimes' - contem o tempo levado para calcular a multiplicação entra as duas matrizes

OBS1: arquivos com o nome allTimes.dat são apenas um compilado de todos os tempos de leitura, ou escrita, ou calculo da multiplicação, em um só arquivo.

OBS2: ao rodar no modo manual, os tempos não serão impressos em arquivo, mas apenas na tela.

OBS3: os casos de teste são lidos de uma pasta chamada Matrizes, portanto devem estar dentro desta pasta para que o programa possa encontralos. Já no modo manual, é necessário especificar o caminho até a pasta do arquivo, ou apenas o nome do arquivo caso ele esteja na mesma pasta do executável.