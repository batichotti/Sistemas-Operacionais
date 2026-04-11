# Lab3 - Guia de Uso

Este diretório contém dois programas com pthreads:

- busca paralela em vetor: `1/vetor_com_threads.cpp`
- processamento de matriz (médias por linha e coluna): `2/matrix.cpp`

## 1) vetor_com_threads.cpp

### O que o programa faz

Cria um vetor aleatório e procura um valor usando `N` threads, dividindo o espaço de busca entre elas.

### Compilação

No diretório `Lab3/1`:

```bash
g++ vetor_com_threads.cpp -o vector -pthread
```

### Execução

```bash
./vector <TAMANHO_VETOR> <MAX_VETOR> <VALOR_PROCURADO> <NUM_THREADS>
```

Parâmetros:

- `TAMANHO_VETOR`: quantidade de elementos do vetor
- `MAX_VETOR`: valor máximo gerado no vetor (mínimo é 0)
- `VALOR_PROCURADO`: número que será buscado
- `NUM_THREADS`: quantidade de threads usadas na busca

Exemplo:

```bash
./vector 1000 500 42 4
```

## 2) matrix.cpp

### O que o programa faz

- lê uma matriz `MxN` de arquivo
- calcula média aritmética de cada linha
- calcula média geométrica de cada coluna
- salva os resultados em arquivo texto
- permite escolher quantas threads serão usadas
- pode gerar matriz aleatória e salvar em arquivo

### Compilação

No diretório `Lab3/2`:

```bash
make matrix
```

ou manualmente:

```bash
g++ matrix.cpp matriz.c -o matrix -pthread
```

### Formato do arquivo de matriz

Primeira linha: `MxN` (exemplo: `6x8`)

Linhas seguintes: valores inteiros da matriz, separados por espaço.

Exemplo:

```text
3x4
1 2 3 4
5 6 7 8
9 10 11 12
```

### Execução

```bash
./matrix -i <arquivo_entrada> [-o <arquivo_saida>] [-n <threads>] [-g] [-L <linhas>] [-C <colunas>] [-h]
```

Parâmetros:

- `-i`: arquivo da matriz (entrada ou destino da geração)
- `-o`: arquivo de saída com resultados (padrão: `resultados.txt`)
- `-n`: quantidade de threads (padrão: `2`)
- `-g`: gera matriz aleatória antes de processar
- `-L`: número de linhas para geração (padrão: `10`)
- `-C`: número de colunas para geração (padrão: `10`)
- `-h`: mostra ajuda

Exemplos:

1. Processar uma matriz já existente com 4 threads:

```bash
./matrix -i matriz_6_8.in -o resultados.txt -n 4
```

2. Gerar uma matriz 20x15 e já processar com 6 threads:

```bash
./matrix -i minha_matriz.in -g -L 20 -C 15 -o resultados.txt -n 6
```

## Limpeza dos binários

No diretório `Lab3/2`:

```bash
make clean
```
