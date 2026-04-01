# Laboratório 2

## Como compilar

Na pasta do projeto (```/Lab2/```), execute o comando no terminal:

```bash
make
```

Ele criará versões compiladas dos códigos nas suas respectivas pastas, por exemplo, o código ```/Lab2/1/hierarquia.cpp``` será compilado para ```/Lab2/1/hierarquia```.

## Como executar

### 1

Para executar o programa da questão 1, basta passar o número de níveis da hierarquia como argumento no terminal durante a chamando do programa:

```bash
./1/hierarquia <NÍVEIS>
```

### 2

Para executar o programa da questão 2, basta passar o comando desejado (e seus argumentos) como argumento no terminal durante a chamando do programa:

```bash
./2/cmd <COMANDO>
```

### 3

Para executar o programa da questão 3, basta passar o tamanho do vetor, o valor máximo que o vetor pode conter, o valor procurado e o número de filhos como argumentos no terminal durante a chamando do programa:

```bash
./3/vector <TAMANHO_VETOR> <MAXIMO_VETOR> <VALOR_PROCURADO> <N_FILHOS>
```

### 4

Para executar o programa da questão 4, basta chamar o programa no terminal, ele irá solicitar a entrada de comandos, e para sair do programa, basta digitar "exit":

```bash
./4/shell
```

## Bibliotecas utilizadas

```c
#include <iostream> // cout, cin, endl
#include <unistd.h> // fork()
#include <sys/wait.h> // wait()
#include <vector> // vector<T>
#include <cstdlib> // rand()
#include <sstream> //  stringstream() - Manipulação de strings
#include <cstring> // strcmp() - Manipulação de strings
```

## Exemplos de uso

### 1

```bash
./1/hierarquia 3
```

### 2

```bash
./2/cmd ls -l
```

### 3

```bash
./3/vector 1000 100 13 7
```

### 4

```bash
./4/shell
> ls
> ls -l
> ping 8.8.8.8 -c 4 &
> clear
> exit
```