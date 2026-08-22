# Algoritmo de Compressão Lempel-Ziv (LZ78) em C

Projeto desenvolvido para a disciplina de **Sistemas de Comunicação 2** no centro universitário **FAESA**.

## Autor
- **Arthur Carlos Calvi de Bone**

## Descrição
Este projeto implementa o algoritmo de compressão e descompressão de dados **Lempel-Ziv (LZ78)** em linguagem C. O algoritmo constrói dinamicamente um dicionário de dicionários/símbolos durante a leitura do arquivo para realizar a compressão sem perda de dados.

## Estrutura do Projeto
- `main.c`: Código fonte contendo a implementação do dicionário encadeado, funções de compactação e descompactação.

## Como Compilar e Executar

### Pré-requisitos
- Compilador C (`gcc` ou `clang`)

### Compilação
No terminal, execute:
```bash
gcc main.c -o lempel_ziv
