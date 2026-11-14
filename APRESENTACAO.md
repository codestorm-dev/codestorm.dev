# Roteiro da apresentação

Duração: uns 15 a 20 minutos. Cada integrante apresenta os seus slides e faz uma parte da demonstração.

## Antes de começar

- Apagar `produtos.dat` e `produtos.txt` da pasta, para a demonstração começar do zero.
- Deixar o Dev-C++ (ou o terminal) e o site já abertos.
- Testar a internet: a cena 3D do site carrega o Three.js pela rede.
- Cada um ler a sua parte e as 3 perguntas, e treinar a fala em voz alta pelo menos uma vez. A nota de domínio é individual.

## Quem explica o quê

### Lais Aparecida: abertura (slides 1 a 8)

Capa · Quem apresenta cada parte · Por que criamos · O que é · Como desenvolvemos · O que o trabalho pedia · Onde cada critério aparece · Tecnologias usadas

- O problema: produto que acaba sem ninguém ver, compra repetida, validade vencida e controle no caderno.
- As duas partes: o programa em C (o centro do trabalho) e o site com depósito 3D (o extra).
- As 4 etapas: programa em C, site com tabela, galpão 3D e testes.
- As 5 funções pedidas e onde cada critério da avaliação aparece.
- As tecnologias: C (gcc / Dev-C++), arquivo binário, HTML, CSS, JavaScript, Three.js e localStorage.

**Demo:** dar dois cliques no `compilar.bat` (ou F11 no Dev-C++) e mostrar o menu com **0 produtos ativos**.

### André Luiz Ribeiro Resende: o programa em C (slides 9 a 15)

Como guardamos cada produto · Como funciona o produtos.dat · As funções · Comandos de C · Exclusão lógica · Como exportamos para TXT · O programa rodando

- A `struct Produto`: código, nome, categoria, quantidade, preço e status. Sempre 96 bytes.
- O `produtos.dat`: registros um atrás do outro; o produto da posição 2 começa no byte 2 × 96 = 192.
- `fopen` com `"rb+"` (ler e gravar) e `"wb+"` (criar na primeira vez).
- As funções: `cadastrarProduto`, `consultarProduto`, `exportarTxt`, `excluirProduto`, `contarAtivos` e as de leitura (`lerTexto`, `lerInteiro`, `lerReal`).
- Exclusão lógica: acha a posição, troca o status de `'A'` para `'E'`, `fseek` e `fwrite` regravam só aquele registro.
- Exportação: `"w"` cria o TXT com **todos** os registros e uma coluna STATUS (ATIVO ou EXCLUIDO); `%-8d` e `%-30.30s` alinham as colunas; o valor do estoque soma só os ativos.

**Demo:** opção 1, cadastrar dois produtos, errando de propósito:

| O que digitar | O que acontece |
|---|---|
| Código `abc` | "Valor invalido", pede de novo |
| Código `101`, nome `Arroz Tipo 1 5kg`, categoria `Alimentos` | Aceita |
| Quantidade `-5` e depois `40` | Recusa o negativo, aceita o 40 |
| Preço `25,90` (com vírgula) | Aceita |
| Segundo produto: código `102`, `Detergente 500ml`, `Limpeza`, `15`, `3.49` | Aceita (preço com ponto) |
| Opção 1 de novo com código `101` | "Ja existe um produto ativo com o codigo 101" |

Depois, opção 2 e consultar o `101`: mostra os dados e o valor total (R$ 1036.00).

### João Gabriel da Silva Seixas: testes e demonstração (slides 16 a 19)

Testes que fizemos · Demonstração ao vivo · O que o site tem a mais · Como o site guarda os dados

- Os testes da tabela abaixo, todos aprovados.
- Por que `fgets` + `strtol` e não `scanf`.
- O site: entrada e saída, estoque mínimo, validade, pesquisa, histórico, excluídos e backup.
- Os dados do site: `save()` grava com `JSON.stringify` no `localStorage` e `render()` redesenha a tela.

**Demo:**
1. Opção 4, excluir o `102` e responder `N`: "Exclusao cancelada".
2. Opção 4 de novo, excluir o `102` e responder `S`. O menu mostra **1 ativo (2 gravados)**: o registro continua no arquivo.
3. Opção 2, consultar o `102`: "nao encontrado".
4. Opção 3, exportar, e abrir o `produtos.txt` no Bloco de Notas: aparecem os dois, o 102 marcado como EXCLUIDO e fora do valor do estoque.
5. Opção 0 para sair, abrir o programa de novo e mostrar que o menu continua com 1 ativo e 2 gravados.

### Victor Hugo de Sousa Ananias: o site e o 3D (slides 20 a 27)

Como montamos a cena 3D · Como o estoque vira o galpão · Como o clique no 3D funciona · Como as animações funcionam · Uma entrega passo a passo · O site: tela principal · depósito 3D · produtos e histórico

- A cena: `WebGLRenderer`, `Scene`, `PerspectiveCamera` + `OrbitControls`, `BoxGeometry`, materiais e luzes.
- Categoria = estante, produto = palete, quantidade = de 0 a 8 caixas; etiqueta amarela (baixo), vermelha (zerado), piscando (vencido).
- O clique: o `Raycaster` lança um raio da câmera pelo mouse e pega o primeiro palete.
- As animações: `requestAnimationFrame` cerca de 60 vezes por segundo, roteiro de passos por veículo, sinais "pronto" e "feito" entre caminhão e empilhadeira, ré em curva de Bézier.

**Demo (no final, slide 29):** abrir o site, clicar em **Carregar exemplo**, clicar num palete, **Adicionar** 10 unidades e mostrar o caminhão chegando pela doca. Depois tentar **Retirar** mais do que tem: aparece "Estoque insuficiente".

### Todos: conclusão e perguntas (slides 28 e 29)

Cada um fala uma frase sobre o que mais aprendeu na sua parte.

## Testes que fizemos

Programa compilado com `gcc -Wall -Wextra -std=c99`: **nenhum erro e nenhum aviso**.

| # | Teste | Resultado |
|---|---|---|
| 1 | Compilar com todos os avisos ligados | Nenhum aviso |
| 2 | Cadastrar produto válido | "Produto cadastrado com sucesso!" |
| 3 | Preço com vírgula (`25,90`) | Aceito |
| 4 | Preço com ponto (`3.49`) | Aceito |
| 5 | Código repetido | Recusado |
| 6 | Letra no código (`abc`) | Pede de novo, sem travar |
| 7 | Nome vazio | "Valor obrigatorio" |
| 8 | Quantidade negativa (`-5`) | Pede de novo |
| 9 | Consultar código existente | Mostra dados e valor total |
| 10 | Excluir e responder `N` | "Exclusao cancelada" |
| 11 | Excluir e responder `S` | Ativos caem, gravados continuam |
| 12 | Consultar produto excluído | "nao encontrado" |
| 13 | Exportar TXT | Todos os registros, com status; valor do estoque só dos ativos |
| 14 | Tamanho do `produtos.dat` com 3 registros | 288 bytes (3 × 96) |
| 15 | Opção inválida no menu (`9`) | "Opcao invalida" |
| 16 | Fechar e abrir o programa de novo | Os dados continuam |
| 17 | Site: retirar mais do que tem | Bloqueado com "Estoque insuficiente" |
| 18 | Site: adicionar pelo 3D | Caminhão entrega e as caixas aparecem |

## Perguntas que a professora pode fazer

### Lais

**Por que fazer o programa em C e também o site?**
O trabalho pede o programa em C, e ele faz as cinco funções. O site usa as mesmas regras e mostra o estoque de um jeito mais fácil de entender.

**O que é exclusão lógica, em uma frase?**
É marcar o produto como excluído (status `'E'`) em vez de apagar ele do arquivo.

**Quais são as cinco funções pedidas?**
Cadastrar, consultar por código, exportar para TXT, excluir com exclusão lógica e mostrar quantos produtos ativos existem.

### André

**Por que a struct tem 96 bytes e não 93?**
Os campos somam 93 bytes, e o compilador completa até 96 para alinhar a memória. O importante é que todos os registros têm o mesmo tamanho.

**Como o programa regrava só um produto?**
Com `fseek(fp, pos * sizeof(Produto), SEEK_SET)`: multiplica a posição pelo tamanho do registro, vai direto para aquele byte e o `fwrite` regrava só ele.

**Qual a diferença entre `"rb+"` e `"wb+"`?**
O `"rb+"` abre um arquivo que já existe para ler e gravar. O `"wb+"` cria o arquivo do zero e apaga o que tinha. Por isso usamos `"wb+"` só quando o arquivo ainda não existe.

### João

**Por que `fgets` + `strtol` e não `scanf`?**
O `fgets` lê a linha inteira com limite de tamanho, então não estoura o vetor nem deixa lixo no buffer. O `strtol` diz se sobrou alguma letra, e aí pedimos de novo.

**Como vocês provaram que a exclusão lógica funciona?**
Depois de excluir, o menu mostra 1 ativo e 2 gravados: o registro continua no arquivo. A consulta responde "não encontrado" e, no TXT, ele aparece marcado como EXCLUIDO.

**Como o programa conta os produtos ativos?**
A função `contarAtivos` lê o arquivo do começo ao fim com `fread` e soma os registros com status `'A'`. O total gravado vem do tamanho do arquivo (`ftell`) dividido por 96.

### Victor

**Como o site sabe em qual palete a pessoa clicou?**
O `Raycaster` lança uma linha da câmera pelo ponto do mouse. O primeiro palete que ela toca é o escolhido, e cada palete guarda o código do produto.

**Onde o site guarda os dados?**
No `localStorage` do navegador. O `save()` transforma tudo em texto com `JSON.stringify` e, ao abrir, o `JSON.parse` lê de volta.

**Como o caminhão anda suave?**
O `requestAnimationFrame` redesenha a cena cerca de 60 vezes por segundo, e a ré até a doca segue uma curva de Bézier.
