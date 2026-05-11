# Lab 5 - Exercício 2

Programa em C++ para leitura e escrita de arquivos com tratamento de sinais e gravação em buffer.

## Arquivos

- `ex2.cpp`: implementação principal do programa.
- `Makefile`: compilação, limpeza e execução.

## Funcionalidades

- **Modo `write`**: lê linhas da entrada padrão e grava no arquivo informado.
- **Modo `read`**: exibe o conteúdo do arquivo informado.
- Tratamento de `SIGINT` e `SIGTERM`.
- Buffer de escrita com tamanho máximo de 5 linhas.
- Flush periódico a cada 5 segundos.

## Compilação

```bash
make
```

## Execução

### Gravar no arquivo

```bash
./ex2 write dados.txt
```

### Ler o arquivo

```bash
./ex2 read dados.txt
```

## Limpeza

```bash
make clean
```

## Observações

- O modo `write` adiciona ao arquivo existente.
- Ao encerrar com `Ctrl+C`, o programa tenta salvar as linhas pendentes antes de finalizar.
