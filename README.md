# Programação Paralela - Trabalho 1

## Descrição

Este repositório contém os códigos desenvolvidos para o Trabalho 1 da disciplina de Programação Paralela do curso de Ciência da Computação da Universidade Federal de Mato Grosso do Sul (UFMS).

## Alunos

- [Luiz Gustavo Sabadim Spolon Junqueira](github.com/luizgustavojunqueira)
- [Camila Cardoso Cáceres](github.com/camilacaceres1d)

## Objetivo

O objetivo deste trabalho prático é permitir o projeto e implementação de um algoritmo paralelo em uma aquitetura de memória compartilhada convencional (CPU multicore) utilizando o modelo de programação Pthread em C/C++.

## Descrição do Problema

O problema consiste em implementar um algoritmo para contagem de cliques em um grafo não direcionado. Um clique é um subgrafo completo, ou seja, um conjunto de vértices onde cada par de vértices é adjacente. O algoritmo deve receber como entrada um grafo não direcionado e um valor K e retornar o número de cliques de tamanho K no grafo.

## Desenvolvimento

Na proposta do trabalho, é pedido para implementar três versões do algoritmo de contagem de cliques, todas recebendo os mesmos parâmetros de entrada e retornando o mesmo valor de saída. A primeira versão é a serial. A segunda é uma versão paralela que distribui o conjunto inicial de cliques de forma fixa entre as threads. A terceira é uma versão paralela que faz o balanceamento de carga entre as threads.

### Metodologia

Para a implementação do algoritmo, foi utilizado a linguagem de programação C/C++ e a biblioteca Pthread. O algoritmo foi implementado em três versões: serial, paralela com divisão fixa e paralela com balanceamento de carga.

Os algoritmos foram testados com os datasets presentes na pasta `graph_datasets`. Os resultados foram comparados com o seguinte gabarito para cada valor de K:

| Dataset    | K   | Cliques    |
| ---------- | --- | ---------- |
| citeseer   | 3   | 24546      |
| citeseer   | 4   | 1166       |
| citeseer   | 5   | 46         |
| citeseer   | 6   | 6          |
| ca_astroph | 3   | 1351441    |
| ca_astroph | 4   | 9580415    |
| ca_astroph | 5   | 64997961   |
| ca_astroph | 6   | 400401488  |
| ca_astroph | 7   | 2218947958 |
| dblp       | 3   | 2224385    |
| dblp       | 4   | 16713192   |
| dblp       | 5   | 262663639  |
| dblp       | 6   | 422180226  |

## Executando o programa

### Compilação

Para compilar o programa, basta executar o seguinte comando no terminal. Isso irá gerar um arquivo executável chamado `main`.

```bash
make main
```

### Execução

Para executar o programa, basta executar o seguinte comando no terminal. O programa irá receber como entrada um arquivo contendo o grafo e um valor K.

```bash
./main <arquivo_do_grafo> <K>
```
