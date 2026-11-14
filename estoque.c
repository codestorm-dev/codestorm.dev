/*
 * Controle de Estoque de Produtos
 * --------------------------------
 * Funcionalidades:
 *   1 - Cadastrar produto
 *   2 - Consultar produto por codigo
 *   3 - Exportar produtos para arquivo texto (TXT)
 *   4 - Excluir produto (exclusao logica via campo status)
 *   Contagem de produtos ativos exibida no menu.
 *
 * Os registros ficam gravados no arquivo binario "produtos.dat".
 * Compilar: gcc estoque.c -o estoque
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ_DADOS   "produtos.dat"
#define ARQ_TEXTO   "produtos.txt"

#define STATUS_ATIVO     'A'
#define STATUS_EXCLUIDO  'E'

typedef struct {
    int   codigo;
    char  nome[50];
    char  categoria[30];
    int   quantidade;
    float preco;
    char  status;      /* 'A' = ativo, 'E' = excluido */
} Produto;

/* ---------- Funcoes auxiliares de entrada ---------- */

/* Le uma linha do teclado, removendo o '\n' final. */
void lerTexto(const char *rotulo, char *destino, int tamanho) {
    for (;;) {
        printf("%s", rotulo);
        if (fgets(destino, tamanho, stdin) == NULL) {
            /* Fim da entrada (Ctrl+Z / Ctrl+D): encerra para nao entrar em laco. */
            printf("\nEntrada encerrada.\n");
            exit(0);
        }
        if (strchr(destino, '\n') == NULL) {
            /* Linha maior que o buffer: descarta o restante. */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        } else {
            destino[strcspn(destino, "\r\n")] = '\0';
        }
        if (destino[0] != '\0') return;
        printf("  Valor obrigatorio.\n");
    }
}

/* Le um inteiro validado (>= minimo). */
int lerInteiro(const char *rotulo, int minimo) {
    char buf[64];
    char *fim;
    long valor;
    for (;;) {
        lerTexto(rotulo, buf, sizeof buf);
        valor = strtol(buf, &fim, 10);
        if (*fim == '\0' && valor >= minimo) return (int)valor;
        printf("  Valor invalido. Informe um numero inteiro >= %d.\n", minimo);
    }
}

/* Le um numero real validado (>= 0). Aceita virgula ou ponto. */
float lerReal(const char *rotulo) {
    char buf[64];
    char *fim, *p;
    double valor;
    for (;;) {
        lerTexto(rotulo, buf, sizeof buf);
        for (p = buf; *p; p++) if (*p == ',') *p = '.';
        valor = strtod(buf, &fim);
        if (*fim == '\0' && valor >= 0) return (float)valor;
        printf("  Valor invalido. Informe um numero >= 0 (ex.: 12.50).\n");
    }
}

void pausar(void) {
    int c;
    printf("\nPressione ENTER para continuar...");
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ---------- Acesso ao arquivo binario ---------- */

/* Abre o arquivo de dados para leitura/escrita, criando-o se nao existir. */
FILE *abrirArquivo(void) {
    FILE *fp = fopen(ARQ_DADOS, "rb+");
    if (fp == NULL) fp = fopen(ARQ_DADOS, "wb+");
    if (fp == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQ_DADOS);
    }
    return fp;
}

/*
 * Procura um produto ATIVO pelo codigo.
 * Retorna a posicao (indice do registro) ou -1 se nao encontrado.
 * Se 'saida' nao for NULL, copia o registro encontrado.
 */
long buscarPorCodigo(FILE *fp, int codigo, Produto *saida) {
    Produto p;
    long pos = 0;
    rewind(fp);
    while (fread(&p, sizeof(Produto), 1, fp) == 1) {
        if (p.status == STATUS_ATIVO && p.codigo == codigo) {
            if (saida != NULL) *saida = p;
            return pos;
        }
        pos++;
    }
    return -1;
}

/* Conta os registros com status ativo. */
int contarAtivos(void) {
    FILE *fp = fopen(ARQ_DADOS, "rb");
    Produto p;
    int total = 0;
    if (fp == NULL) return 0;
    while (fread(&p, sizeof(Produto), 1, fp) == 1) {
        if (p.status == STATUS_ATIVO) total++;
    }
    fclose(fp);
    return total;
}

/* Conta todos os registros gravados (ativos + excluidos). */
int contarTotalGravados(void) {
    FILE *fp = fopen(ARQ_DADOS, "rb");
    long tamanho;
    if (fp == NULL) return 0;
    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);
    fclose(fp);
    return (int)(tamanho / (long)sizeof(Produto));
}

void exibirProduto(const Produto *p) {
    printf("----------------------------------------\n");
    printf("Codigo     : %d\n", p->codigo);
    printf("Nome       : %s\n", p->nome);
    printf("Categoria  : %s\n", p->categoria);
    printf("Quantidade : %d\n", p->quantidade);
    printf("Preco unit.: R$ %.2f\n", p->preco);
    printf("Valor total: R$ %.2f\n", p->quantidade * p->preco);
    printf("----------------------------------------\n");
}

/* ---------- Funcionalidades ---------- */

void cadastrarProduto(void) {
    FILE *fp;
    Produto p;

    printf("\n=== CADASTRAR PRODUTO ===\n");
    fp = abrirArquivo();
    if (fp == NULL) return;

    memset(&p, 0, sizeof p);
    p.codigo = lerInteiro("Codigo: ", 1);

    if (buscarPorCodigo(fp, p.codigo, NULL) != -1) {
        printf("Ja existe um produto ativo com o codigo %d.\n", p.codigo);
        fclose(fp);
        return;
    }

    lerTexto("Nome: ", p.nome, sizeof p.nome);
    lerTexto("Categoria: ", p.categoria, sizeof p.categoria);
    p.quantidade = lerInteiro("Quantidade em estoque: ", 0);
    p.preco = lerReal("Preco unitario (R$): ");
    p.status = STATUS_ATIVO;

    fseek(fp, 0, SEEK_END);
    if (fwrite(&p, sizeof(Produto), 1, fp) == 1) {
        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Erro ao gravar o produto.\n");
    }
    fclose(fp);
}

void consultarProduto(void) {
    FILE *fp;
    Produto p;
    int codigo;

    printf("\n=== CONSULTAR PRODUTO POR CODIGO ===\n");
    fp = fopen(ARQ_DADOS, "rb");
    if (fp == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    codigo = lerInteiro("Codigo: ", 1);
    if (buscarPorCodigo(fp, codigo, &p) != -1) {
        exibirProduto(&p);
    } else {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
    }
    fclose(fp);
}

void exportarTxt(void) {
    FILE *fp, *txt;
    Produto p;
    int qtd = 0;
    float valorEstoque = 0;

    printf("\n=== EXPORTAR PARA TXT ===\n");
    fp = fopen(ARQ_DADOS, "rb");
    if (fp == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    txt = fopen(ARQ_TEXTO, "w");
    if (txt == NULL) {
        printf("Erro: nao foi possivel criar %s.\n", ARQ_TEXTO);
        fclose(fp);
        return;
    }

    fprintf(txt, "RELATORIO DE ESTOQUE DE PRODUTOS\n");
    fprintf(txt, "==================================================================================\n");
    fprintf(txt, "%-8s %-30s %-20s %8s %12s\n", "CODIGO", "NOME", "CATEGORIA", "QTD", "PRECO (R$)");
    fprintf(txt, "----------------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Produto), 1, fp) == 1) {
        if (p.status != STATUS_ATIVO) continue;
        fprintf(txt, "%-8d %-30.30s %-20.20s %8d %12.2f\n",
                p.codigo, p.nome, p.categoria, p.quantidade, p.preco);
        valorEstoque += p.quantidade * p.preco;
        qtd++;
    }

    fprintf(txt, "----------------------------------------------------------------------------------\n");
    fprintf(txt, "Total de produtos ativos: %d\n", qtd);
    fprintf(txt, "Valor total em estoque : R$ %.2f\n", valorEstoque);

    fclose(txt);
    fclose(fp);
    printf("%d produto(s) exportado(s) para o arquivo \"%s\".\n", qtd, ARQ_TEXTO);
}

void excluirProduto(void) {
    FILE *fp;
    Produto p;
    long pos;
    int codigo;
    char resp[8];

    printf("\n=== EXCLUIR PRODUTO ===\n");
    fp = fopen(ARQ_DADOS, "rb+");
    if (fp == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    codigo = lerInteiro("Codigo do produto a excluir: ", 1);
    pos = buscarPorCodigo(fp, codigo, &p);
    if (pos == -1) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        fclose(fp);
        return;
    }

    exibirProduto(&p);
    lerTexto("Confirma a exclusao? (S/N): ", resp, sizeof resp);
    if (resp[0] != 'S' && resp[0] != 's') {
        printf("Exclusao cancelada.\n");
        fclose(fp);
        return;
    }

    /* Exclusao logica: apenas marca o status e regrava o registro no mesmo lugar. */
    p.status = STATUS_EXCLUIDO;
    fseek(fp, pos * (long)sizeof(Produto), SEEK_SET);
    if (fwrite(&p, sizeof(Produto), 1, fp) == 1) {
        printf("Produto excluido com sucesso.\n");
    } else {
        printf("Erro ao excluir o produto.\n");
    }
    fclose(fp);
}

/* ---------- Menu principal ---------- */

int main(void) {
    int opcao;

    do {
        printf("\n==========================================\n");
        printf("      CONTROLE DE ESTOQUE DE PRODUTOS\n");
        printf("==========================================\n");
        printf(" Produtos ativos: %d  (gravados no arquivo: %d)\n",
               contarAtivos(), contarTotalGravados());
        printf("------------------------------------------\n");
        printf(" 1 - Cadastrar produto\n");
        printf(" 2 - Consultar produto por codigo\n");
        printf(" 3 - Exportar produtos para TXT\n");
        printf(" 4 - Excluir produto\n");
        printf(" 0 - Sair\n");
        printf("==========================================\n");
        opcao = lerInteiro("Opcao: ", 0);

        switch (opcao) {
            case 1: cadastrarProduto(); pausar(); break;
            case 2: consultarProduto(); pausar(); break;
            case 3: exportarTxt();      pausar(); break;
            case 4: excluirProduto();   pausar(); break;
            case 0: printf("Encerrando o sistema...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
