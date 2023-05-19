#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct filme{
    int ano;
    char genero[20];
    char titulo[50];
} Filme;

// Estrutura para armazenar um n� da �rvore bin�ria
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

// Fun��o para inserir n�

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

// Fun��o para buscar filme por ano
//No *buscar_filme_ano(No *raiz, int ano){
//    if (raiz == NULL){
//        return NULL;
//    }
//    else if (raiz->dado.ano == ano){
//        return raiz;
//    }
//    else if (raiz->dado.ano > ano){
//        return buscar_filme_ano(raiz->esq, ano);
//    }
//    else{
//        return buscar_filme_ano(raiz->dir, ano);
//    }
//}

// Fun��o para buscar filme por ano com mais de um resultado
No **buscar_filme_ano(No *raiz, int ano, No **resultados, int *n){
    if (raiz == NULL){
        return NULL;
    }
    else if (raiz->dado.ano == ano){
        resultados[*n] = raiz; // adiciona o n� ao vetor de resultados
        (*n)++; // incrementa o contador de resultados
    }
    buscar_filme_ano(raiz->esq, ano, resultados, n); // busca na sub�rvore esquerda
    buscar_filme_ano(raiz->dir, ano, resultados, n); // busca na sub�rvore direita
    return NULL;
}



// Fun��o para buscar um filme pelo g�nero
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

// Fun��o para buscar um filme pelo t�tulo
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

// Fun��o para imprimir os dados de um filme
void imprimir_filme(Filme f){
    printf("Ano: %d\n", f.ano);
    printf("Genero: %s\n", f.genero);
    printf("Titulo: %s\n", f.titulo);
}

// Fun��o para imprimir a �rvore bin�ria em ordem crescente de ano
void imprimir_arvore(No *raiz){
    if (raiz != NULL){
        imprimir_arvore(raiz->esq);
        imprimir_filme(raiz->dado);
        printf("\n");
        imprimir_arvore(raiz->dir);
    }
}

// Fun��o para imprimir uma lista de filmes
void imprimir_lista(No *lista){
    No *aux = lista;
    while (aux != NULL){
        imprimir_filme(aux->dado);
        printf("\n");
        aux = aux->esq;
    }
}

// Fun��o para liberar a mem�ria da lista de filmes
void liberar_lista(No **lista){
     No *aux = *lista;
     while (aux != NULL){
         No *prox = aux->esq;
         free(aux);
         aux = prox;
     }
     *lista = NULL;
}

// Fun��o para liberar a mem�ria da �rvore bin�ria
void liberar_arvore(No **raiz){
     if (*raiz != NULL){
         liberar_arvore(&(*raiz)->esq);
        liberar_arvore(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}

// Fun��o para ler o arquivo de testes
Filme ler_filme(FILE *arq){
    Filme f;
    f.ano = -1;
    if (arq != NULL){
        fscanf(arq, "%d", &f.ano);
        fscanf(arq, "%s", f.genero);
        fscanf(arq, "%[^\n]s", f.titulo);
    }
    return f;
}

// Fun��o para carregar uma �rvore bin�ria a partir de um arquivo de casos de teste
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

// Fun��o de menu
void exibir_menu(){
    printf("Escolha uma opcao:\n");
    printf("1 - Buscar filme por ano\n");
    printf("2 - Buscar filme por genero\n");
    printf("3 - Buscar filme por titulo\n");
    printf("4 - Carregar arvore binaria a partir de um arquivo de casos de teste\n");
    printf("5 - Imprimir arvore binaria em ordem crescente de ano\n");
    printf("6 - Sair do programa\n");
}

// Fun��o main
int main(){

    setlocale(LC_ALL, "Portuguese_Brazil");

    No *raiz = NULL;

    int opcao;
    do{
        exibir_menu();
        scanf("%d", &opcao);
        switch (opcao){
            case 1:{ // buscar filme por ano
                int ano;
                printf("Digite o ano do filme que deseja buscar: ");
                scanf("%d", &ano);
                No *resultados[100];
                int n = 0;
                buscar_filme_ano(raiz, ano, resultados, &n);
                if (n > 0){
                    printf("Filmes encontrados:\n");
                    for (int i = 0; i < n; i++) {
                        imprimir_filme(resultados[i]->dado);
                    }
                }
                else
                    printf("Filme nao encontrado.\n");
                break;
            }
            case 2:{ // buscar filme por g�nero
                char genero[20];
                printf("Digite o genero do filme que deseja buscar: ");
                scanf("%s", genero);
                No *lista = buscar_filme_genero(raiz, genero);
                if (lista != NULL){
                    printf("Filmes encontrados:\n");
                    imprimir_lista(lista);
                    liberar_lista(&lista);
                }
                else{
                    printf("Nenhum filme encontrado.\n");
                }
                break;
            }
            case 3:{ // buscar filme por t�tulo
                char titulo[50];
                printf("Digite o titulo do filme que deseja buscar: ");
                scanf("%[^\n]s", titulo);
                No *busca = buscar_filme_titulo(raiz, titulo);
                if (busca != NULL){
                    printf("Filme encontrado:\n");
                    imprimir_filme(busca->dado);
                }
                else{
                    printf("Filme n�o encontrado.\n");
                }
                break;
            }
            case 4:{  // carregar casos de teste
                char nome_arquivo[50];
                printf("Digite o nome do arquivo de casos de teste: ");
                scanf("%s", nome_arquivo);
                carregar_arvore(&raiz, nome_arquivo);
                break;
            }
            case 5:{  // imprimir a �rvore bin�ria em ordem de ano
                imprimir_arvore(raiz);
                break;
            }
            case 6:{  // sair
                liberar_arvore(&raiz);  // libera a mem�ria da �rvore bin�ria
                break;
            }
            default:{
                printf("Opcao invalida.\n");
                break;
            }
        }
    } while (opcao != 6);

}