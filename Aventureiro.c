#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50

// ------------------------------
// Estruturas de dados
// ------------------------------
typedef struct Sala {
    char nome[TAM];
    char pista[TAM];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[TAM];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ------------------------------
// Funções relacionadas à mansão
// ------------------------------

// Cria dinamicamente uma sala (com ou sem pista)
Sala* criarSala(char nome[], char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ------------------------------
// Funções relacionadas às pistas (BST)
// ------------------------------

// Insere uma nova pista em ordem alfabética na árvore BST
PistaNode* inserirPista(PistaNode *raiz, char pista[]) {
    if (strlen(pista) == 0) return raiz; // ignora cômodos sem pista

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// Exibe as pistas coletadas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("🔎 %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ------------------------------
// Exploração da mansão
// ------------------------------

// Controla a navegação entre as salas e coleta de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    char opcao;

    while (1) {
        printf("\nVocê está no cômodo: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Há uma pista aqui: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nEscolha o caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita) printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("\n🕵️ Você decidiu encerrar a exploração.\n");
            return;
        } else if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho inválido! Tente novamente.\n");
        }
    }
}

// ------------------------------
// Função principal
// ------------------------------
int main() {
    // Criação do mapa fixo da mansão (árvore binária)
    Sala *hall = criarSala("Hall de Entrada", "Pegada de lama");
    Sala *biblioteca = criarSala("Biblioteca", "Livro com páginas rasgadas");
    Sala *salaJantar = criarSala("Sala de Jantar", "");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *escritorio = criarSala("Escritório", "Bilhete misterioso");

    // Conexões do mapa (sem o Jardim)
    hall->esquerda = biblioteca;
    hall->direita = salaJantar;
    biblioteca->esquerda = cozinha;  // só um caminho à esquerda agora
    salaJantar->direita = escritorio;

    // Raiz da BST de pistas
    PistaNode *pistas = NULL;

    printf("=== 🏰 Detective Quest: Exploração da Mansão ===\n");
    explorarSalasComPistas(hall, &pistas);

    printf("\n=== 🔍 Pistas Coletadas (em ordem alfabética) ===\n");
    if (pistas)
        exibirPistas(pistas);
    else
        printf("Nenhuma pista foi coletada!\n");

    printf("\nFim da exploração.\n");
    return 0;
}
