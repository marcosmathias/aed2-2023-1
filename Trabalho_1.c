#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct filme{
    int ano;
    char genero[20];
    char titulo[50];
} Filme;

// Estrutura para armazenar um no da arvore binaria
typedef struct no{
    Filme dado;
    struct no *esq;
    struct no *dir;
} No;

No *criar_no(Filme f){
    No *novo = (No*) malloc(sizeof(No));
    novo->dado = f;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Funcao para inserir no

void inserir_no(No **raiz, No *novo){
    if (*raiz == NULL){
        *raiz = novo;
    }
    else{
        if (novo->dado.ano < (*raiz)->dado.ano){
            inserir_no(&(*raiz)->esq, novo);
        }
        else if (novo->dado.ano > (*raiz)->dado.ano){
            inserir_no(&(*raiz)->dir, novo);
        }
        else{
            inserir_no(&(*raiz)->dir, novo);
        }
    }
}

// Funcao para calcular a altura de um no
int altura_no(No *no){
    if (no == NULL)
        return 0;
    int altura_esq = altura_no(no->esq);
    int altura_dir = altura_no(no->dir);

    if (altura_esq > altura_dir) {
        return 1 + altura_esq;
    } else {
    return 1 + altura_dir;
    };
}

// Funcao para calcular o fator de balanceamento de um no
int fator_balanceamento(No *no){
    if (no == NULL)
        return 0;
    int altura_esq = altura_no(no->esq);
    int altura_dir = altura_no(no->dir);
    return altura_esq - altura_dir;
}

// Funcao para fazer uma rotacao simples ˆ direita
No *rotacao_simples_direita(No *raiz){
    No *nova_raiz = raiz->esq;
    raiz->esq = nova_raiz->dir;
    nova_raiz->dir = raiz;
    return nova_raiz;
}

// Funcao para fazer uma rotacao simples ˆ esquerda
No *rotacao_simples_esquerda(No *raiz){
    No *nova_raiz = raiz->dir;
    raiz->dir = nova_raiz->esq;
    nova_raiz->esq = raiz;
    return nova_raiz;
}

// Funcao para fazer uma rotacao dupla ˆ direita
No *rotacao_dupla_direita(No *raiz){
    raiz->esq = rotacao_simples_esquerda(raiz->esq);
    return rotacao_simples_direita(raiz);
}

// Funcao para fazer uma rotacao dupla ˆ esquerda
No *rotacao_dupla_esquerda(No *raiz){
    raiz->dir = rotacao_simples_direita(raiz->dir);
    return rotacao_simples_esquerda(raiz);
}

// Funcao para balancear um no da arvore
No *balancear_no(No *no){
    int fb = fator_balanceamento(no);
    if (fb > 1){ // Sub‡rvore esquerda pesada
        if (fator_balanceamento(no->esq) > 0) // Caso LL
            no = rotacao_simples_direita(no);
        else // Caso LR
            no = rotacao_dupla_direita(no);
    }
    else if (fb < -1){ // Sub‡rvore direita pesada
        if (fator_balanceamento(no->dir) < 0) // Caso RR
            no = rotacao_simples_esquerda(no);
        else // Caso RL
            no = rotacao_dupla_esquerda(no);
    }
    return no;
}

// Funcao para balancear a arvore binaria utilizando o metodo AVL
No *balancear_arvore(No *raiz){
    if (raiz == NULL)
        return NULL;
    raiz->esq = balancear_arvore(raiz->esq);
    raiz->dir = balancear_arvore(raiz->dir);
    return balancear_no(raiz);
}

// Funcao para buscar filme por ano com mais de um resultado
No **buscar_filme_ano(No *raiz, int ano, No **resultados, int *n){
    if (raiz == NULL){
        return NULL;
    }
    else if (raiz->dado.ano == ano){
        resultados[*n] = raiz; // adiciona o no ao vetor de resultados
        (*n)++; // incrementa o contador de resultados
    }
    buscar_filme_ano(raiz->esq, ano, resultados, n); // busca na sub‡rvore esquerda
    buscar_filme_ano(raiz->dir, ano, resultados, n); // busca na sub‡rvore direita
    return NULL;
}



// Funcao para buscar um filme pelo genero
No *buscar_filme_genero(No *raiz, char *genero){
    No *lista = NULL;
    if (raiz != NULL){
        lista = buscar_filme_genero(raiz->esq, genero);
        if (strcmp(raiz->dado.genero, genero) == 0){
            raiz->esq = lista;
            lista = raiz;
        }
        lista = buscar_filme_genero(raiz->dir, genero);
    }
    return lista;
}

// Funcao para buscar um filme pelo titulo
No *buscar_filme_titulo(No *raiz, char *titulo){
    if (raiz == NULL){
        return NULL;
    }
    else if (strcmp(raiz->dado.titulo, titulo) == 0){
        return raiz;
    }
    else{
        No *busca_esq = buscar_filme_titulo(raiz->esq, titulo);
        No *busca_dir = buscar_filme_titulo(raiz->dir, titulo);
        if (busca_esq != NULL){
            return busca_esq;
        }
        else if (busca_dir != NULL){
            return busca_dir;
        }
        else{
            return NULL;
        }
    }
}

// Funcao para imprimir os dados de um filme
void imprimir_filme(Filme f){
    printf("Ano: %d\n", f.ano);
    printf("Genero: %s\n", f.genero);
    printf("Titulo: %s\n", f.titulo);
}

// Funcao para imprimir a arvore binaria em ordem crescente de ano
void imprimir_arvore(No *raiz){
    if (raiz != NULL){
        imprimir_arvore(raiz->esq);
        imprimir_filme(raiz->dado);
        printf("\n");
        imprimir_arvore(raiz->dir);
    }
}

// Funcao para imprimir uma lista de filmes
void imprimir_lista(No *lista){
    No *aux = lista;
    while (aux != NULL){
        imprimir_filme(aux->dado);
        printf("\n");
        aux = aux->esq;
    }
}

// Funcao para liberar a mem—ria da lista de filmes
void liberar_lista(No **lista){
     No *aux = *lista;
     while (aux != NULL){
         No *prox = aux->esq;
         free(aux);
         aux = prox;
     }
     *lista = NULL;
}

// Funcao para liberar a mem—ria da ‡rvore bin‡ria
void liberar_arvore(No **raiz){
     if (*raiz != NULL){
         liberar_arvore(&(*raiz)->esq);
        liberar_arvore(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}

// Funcao para ler o arquivo de testes
Filme ler_filme(FILE *arq){
    Filme f;
    f.ano = -1;
    if (arq != NULL){
        fscanf(arq, "%d_%[^_]_%[^\n]s", &f.ano, f.genero, f.titulo);
    }
    return f;
}

// Funcao para carregar uma ‡rvore bin‡ria a partir de um arquivo de casos de teste
// formato: ano genero titulo\n
void carregar_arvore(No **raiz, char *nome_arquivo){
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL){
        printf("Erro ao abrir o arquivo %s.\n", nome_arquivo);
    }
    else{
        Filme f = ler_filme(arq);
        while (f.ano != -1){
            inserir_no(raiz, criar_no(f));
            f = ler_filme(arq);
        }
        fclose(arq);
    }
}



// Funcao de menu
void exibir_menu(){
    printf("Escolha uma opcao:\n");
    printf("1 - Buscar filme por ano\n");
    printf("2 - Buscar filme por genero\n");
    printf("3 - Buscar filme por titulo\n");
    printf("4 - Carregar arvore binaria a partir de um arquivo de casos de teste\n");
    printf("5 - Imprimir arvore binaria em ordem crescente de ano\n");
    printf("6 - Balancear a arvore binaria carregada\n");
    printf("7 - Sair do programa\n");
}

// Funcao main
int main(){

    No *raiz = NULL;

    int opcao;
    do{
        printf("\n");
        exibir_menu();
        scanf("%d", &opcao);
        switch (opcao){
            case 1:{ // buscar filme por ano
                int ano;
                printf("\nDigite o ano do filme que deseja buscar: ");
                scanf("%d", &ano);
                No *resultados[100];
                int n = 0;
                buscar_filme_ano(raiz, ano, resultados, &n);
                if (n > 0){
                    printf("\nFilmes encontrados:\n");
                    for (int i = 0; i < n; i++) {
                        imprimir_filme(resultados[i]->dado);
                    }
                }
                else
                    printf("\nFilme nao encontrado.\n");
                break;
            }
            case 2:{ // buscar filme por genero
                char genero[20];
                printf("\nDigite o genero do filme que deseja buscar: ");
                scanf("%s", genero);
                No *lista = buscar_filme_genero(raiz, genero);
                if (lista != NULL){
                    printf("\nFilmes encontrados:\n");
                    imprimir_lista(lista);
                    liberar_lista(&lista);
                }
                else{
                    printf("\nNenhum filme encontrado.\n");
                }
                break;
            }
            case 3:{ // buscar filme por titulo
                char titulo[50];
                printf("\nDigite o titulo do filme que deseja buscar: ");
                scanf("%[^\n]s", titulo);
                No *busca = buscar_filme_titulo(raiz, titulo);
                if (busca != NULL){
                    printf("\nFilme encontrado:\n");
                    imprimir_filme(busca->dado);
                }
                else{
                    printf("\nFilme nao encontrado.\n");
                }
                break;
            }
            case 4:{  // carregar casos de teste
                char nome_arquivo[50];
                printf("\nDigite o nome do arquivo de casos de teste: ");
                scanf("%s", nome_arquivo);
                carregar_arvore(&raiz, nome_arquivo);
            printf("\nArvore carregada com sucesso.\n");
            printf("Altura da arvore: %d\n", altura_no(raiz));
                break;
            }
            case 5:{  // imprimir a arvore bin‡ria em ordem de ano
                imprimir_arvore(raiz);
                break;
            }
            case 6:{  // balancear arvore utilizando o metodo AVL
            printf("\nAltura da arvore antes do balanceamento: %d\n", altura_no(raiz));
            raiz = balancear_arvore(raiz);
            printf("\nArvore balanceada com sucesso.\n");
            printf("\nAltura da arvore apos o balanceamento: %d\n", altura_no(raiz));
            break;
            }
            case 7:{  // sair
                liberar_arvore(&raiz);  // libera a memoria da ‡rvore bin‡ria
                break;
            }
        }
    } while (opcao != 7);

}
