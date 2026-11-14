# Controle de Estoque de Produtos

Trabalho da faculdade, orientado pela Professora Flávia.

**Grupo:** Lais Aparecida, André Luiz Ribeiro Resende, Victor Hugo de Sousa Ananias e João Gabriel da Silva Seixas.

O projeto tem duas partes:

1. **`estoque.c`** – o programa em C pedido no trabalho, que roda no terminal.
2. **`estoque.html`** – uma versão visual do mesmo controle de estoque, com depósito em 3D.

## Programa em C (`estoque.c`)

Funcionalidades:

- cadastrar produto (código, nome, categoria, quantidade e preço);
- consultar produto pelo código;
- exportar os produtos ativos para `produtos.txt`;
- excluir produto por **exclusão lógica**: o registro continua no arquivo, só muda o campo `status` de `'A'` (ativo) para `'E'` (excluído);
- mostrar no menu quantos produtos ativos existem.

Os dados ficam no arquivo binário `produtos.dat`, criado na primeira execução. Cada produto é gravado como uma `struct Produto` de tamanho fixo. Para excluir um produto, o programa usa `fseek` para voltar à posição do registro e regravá-lo com o novo status.

### Como compilar e rodar

No Windows, é só dar dois cliques no **`compilar.bat`**: ele compila e já abre o programa (precisa do Dev-C++ ou do gcc instalado).

Ou pelo terminal, com o GCC (MinGW, Dev-C++ ou Linux):

```bash
gcc -Wall -Wextra -std=c99 -o estoque estoque.c
./estoque
```

No Dev-C++, também dá para abrir o `estoque.c` e apertar **F11** (compilar e executar).

## Versão visual (`estoque.html`)

**Versão online:** https://codestorm-dev.github.io/codestorm.dev/

Também dá para baixar e abrir o arquivo direto no navegador, sem instalar nada. Além do que o programa em C faz, ela tem:

- entrada e saída de mercadoria, com histórico de movimentações;
- estoque mínimo e alertas de reposição;
- busca e filtros na lista de produtos;
- fornecedor e data de validade;
- um galpão em 3D, feito com [Three.js](https://threejs.org/), onde as caixas nas prateleiras mostram a quantidade em estoque e um caminhão com empilhadeira faz as entregas;
- exportação em TXT e backup em JSON.

Os dados ficam salvos no próprio navegador (`localStorage`). A cena 3D carrega o Three.js pela internet, então precisa de conexão para aparecer.

## Apresentação

O roteiro de quem apresenta o quê, a demonstração passo a passo, os testes e as perguntas prováveis estão em [APRESENTACAO.md](APRESENTACAO.md).

## Relatório

A pasta `relatorio/` tem o relatório do trabalho em PDF e as imagens usadas nele.
