#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura de dados: Sala (nó da árvore binária)
// ---------------------------------------------------------
typedef struct Sala {
    char nome[50];            // Nome da sala 
    struct Sala *esquerda;    // Ponteiro para o caminho da esquerda
    struct Sala *direita;     // Ponteiro para o caminho da direita
} Sala;


// Função: criarSala 
// Cria uma nova sala de forma dinâmica e retorna o ponteiro.
// ---------------------------------------------------------
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
   // Copia o nome para a estrutura da sala
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garante terminação nula
    
    // Inicializa os ponteiros dos filhos como NULL (sem caminhos inicialmente)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// ---------------------------------------------------------
// Função: explorarSalas
// Permite ao jogador explorar a mansão a partir da sala atual.
// O jogador escolhe 'e' (esquerda), 'd' (direita) ou 's' (sair).
// ---------------------------------------------------------
void explorarSalas(Sala *atual) {
    char opcao;

    if (atual == NULL) {
        printf("A sala não existe.\n");
        return;
    }

    printf("\nVocê está no(a): %s\n", atual->nome);

    // Se não há caminhos, o jogador chegou ao final
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("Você chegou a um cômodo sem saídas. Fim da exploração!\n");
        return;
    }

    while (1) {
        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL) printf(" (e) Ir para a esquerda\n");
        if (atual->direita != NULL) printf(" (d) Ir para a direita\n");
        printf(" (s) Sair da exploração\n> ");

        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            explorarSalas(atual->esquerda);
            break;
        } else if (opcao == 'd' && atual->direita != NULL) {
            explorarSalas(atual->direita);
            break;
        } else if (opcao == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ---------------------------------------------------------
// Função: liberarMemoria
// Libera toda a memória alocada para as salas.
// ---------------------------------------------------------
void liberarMemoria(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMemoria(raiz->esquerda);
    liberarMemoria(raiz->direita);
    free(raiz);
}

// ---------------------------------------------------------
// Função principal: main
// Monta manualmente a árvore binária (mapa da mansão)
// e inicia a exploração a partir do Hall de entrada.
// ---------------------------------------------------------
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");

    // Estrutura do mapa (árvore)
    //
    //                Hall
    //              /      \
    //        Sala Estar   Biblioteca
    //        /      \          \
    //   Cozinha    Jardim      Escritório


    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->direita = escritorio;
 

    printf("===========================================\n");
    printf("========= **Detective Quest================\n");
    printf("  Mapa da Mansão - Início da Exploração\n");
    printf("===========================================\n");

    explorarSalas(hall);

    liberarMemoria(hall);

    printf("\nExploração encerrada. Obrigado por jogar!\n");
    return 0;
}
