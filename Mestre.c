#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower

// Constantes
#define TAMANHO_MAX_STRING 50
#define TAMANHO_TABELA_HASH 10 // Um tamanho pequeno para simplificar

// --- 1. ESTRUTURAS DE DADOS ---

// Estrutura para o Nó da Árvore Binária (Mansão)
typedef struct NoSala {
    char nome[TAMANHO_MAX_STRING];
    char pista[TAMANHO_MAX_STRING]; // Pista estática associada à sala
    struct NoSala *esquerda;
    struct NoSala *direita;
} NoSala;

// Estrutura para o Nó da Árvore de Busca Binária (Pistas Coletadas)
typedef struct NoPista {
    char pista[TAMANHO_MAX_STRING];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para um Item da Tabela Hash (Associação Pista -> Suspeito)
typedef struct HashItem {
    char pista[TAMANHO_MAX_STRING];
    char suspeito[TAMANHO_MAX_STRING];
    struct HashItem *proximo; // Para tratamento de colisão (encadeamento)
} HashItem;

// Estrutura para a Tabela Hash
typedef struct TabelaHash {
    HashItem *itens[TAMANHO_TABELA_HASH];
} TabelaHash;


// --- 2. FUNÇÕES DE SUPORTE ---

/**
 *  Criar dinamicamente um novo cômodo (nó de sala) na mansão.
 *  O nome exclusivo do cômodo.
 *  A pista estática associada a este cômodo.
 *  Um ponteiro para a nova estrutura NoSala alocada.
 */
NoSala* criarSala(const char *nome, const char *pista) {
    NoSala *novaSala = (NoSala*)malloc(sizeof(NoSala));
    if (novaSala == NULL) {
        perror("Erro de alocação de memória para NoSala");
        exit(EXIT_FAILURE);
    }
    strncpy(novaSala->nome, nome, TAMANHO_MAX_STRING - 1);
    novaSala->nome[TAMANHO_MAX_STRING - 1] = '\0';
    strncpy(novaSala->pista, pista, TAMANHO_MAX_STRING - 1);
    novaSala->pista[TAMANHO_MAX_STRING - 1] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    printf("> Cômodo '%s' (Pista: '%s') criado.\n", nome, pista);
    return novaSala;
}

/**
 * Inserir a pista coletada na Árvore de Busca Binária (BST) de forma ordenada.
 * O nó raiz da BST de pistas.
 * A string da pista a ser inserida.
 * O novo nó raiz (ou o nó existente) da BST de pistas.
 */
NoPista* inserirPista(NoPista *raiz, const char *novaPista) {
    if (raiz == NULL) {
        NoPista *novoNo = (NoPista*)malloc(sizeof(NoPista));
        if (novoNo == NULL) {
            perror("Erro de alocação de memória para NoPista");
            exit(EXIT_FAILURE);
        }
        strncpy(novoNo->pista, novaPista, TAMANHO_MAX_STRING - 1);
        novoNo->pista[TAMANHO_MAX_STRING - 1] = '\0';
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }

    int comparacao = strcmp(novaPista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, novaPista);
    }
    // Ignora se a pista já existe (comparacao == 0)

    return raiz;
}

// Função de hash simples para strings
unsigned int hash(const char *chave) {
    unsigned int hashVal = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hashVal = hashVal * 31 + chave[i];
    }
    return hashVal % TAMANHO_TABELA_HASH;
}

/**
 * Insere a associação pista/suspeito na Tabela Hash.
 * Resolve colisões usando encadeamento.
 *  Tabela, O ponteiro para a TabelaHash.
 *  A pista (chave) a ser associada.
 *  O suspeito (valor) correspondente à pista.
 */
void inserirNaHash(TabelaHash *tabela, const char *pista, const char *suspeito) {
    unsigned int indice = hash(pista);

    // Cria o novo item
    HashItem *novoItem = (HashItem*)malloc(sizeof(HashItem));
    if (novoItem == NULL) {
        perror("Erro de alocação de memória para HashItem");
        exit(EXIT_FAILURE);
    }
    strncpy(novoItem->pista, pista, TAMANHO_MAX_STRING - 1);
    novoItem->pista[TAMANHO_MAX_STRING - 1] = '\0';
    strncpy(novoItem->suspeito, suspeito, TAMANHO_MAX_STRING - 1);
    novoItem->suspeito[TAMANHO_MAX_STRING - 1] = '\0';
    novoItem->proximo = NULL;

    // Inserção no início da lista encadeada (ou substitui se já existir)
    HashItem *atual = tabela->itens[indice];
    HashItem *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            // Se a chave já existe, apenas atualiza o valor
            strncpy(atual->suspeito, suspeito, TAMANHO_MAX_STRING - 1);
            atual->suspeito[TAMANHO_MAX_STRING - 1] = '\0';
            free(novoItem); // Libera o item recém-criado, pois não será usado
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    
    // Insere o novo item no início da lista no índice
    novoItem->proximo = tabela->itens[indice];
    tabela->itens[indice] = novoItem;

    printf("> Associação na Hash: Pista '%s' -> Suspeito '%s' inserida no índice %u.\n", pista, suspeito, indice);
}

/**
 * Consulta o suspeito correspondente a uma pista na Tabela Hash.
 * Tabela, O ponteiro para a TabelaHash.
 * A pista (chave) a ser procurada.
 * O ponteiro para a string do nome do suspeito, ou NULL se não for encontrada.
 */
const char* encontrarSuspeito(TabelaHash *tabela, const char *pista) {
    unsigned int indice = hash(pista);
    HashItem *atual = tabela->itens[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Pista não encontrada na hash
}

// --- 3. FUNÇÕES DO JOGO ---

/**
 * Função recursiva principal para navegação e interação do jogador na mansão.
 * O nó da sala atual na Árvore Binária da mansão.
 * A raiz da BST onde as pistas encontradas serão armazenadas.
 * O ponteiro para a Tabela Hash de associações Pista/Suspeito.
 * A raiz da BST de pistas atualizada.
 */
NoPista* explorarSalas(NoSala *atual, NoPista *pistasColetadas, TabelaHash *tabelaHash) {
    char acao[10];

    while (atual != NULL) {
        printf("\n--- Você está no cômodo: **%s** ---\n", atual->nome);

        // 1. Coleta da Pista
        if (atual->pista[0] != '\0') {
            printf(" Você encontrou uma pista: **%s**\n", atual->pista);
            
            // Insere na BST (Árvore de Pistas)
            NoPista* raizAnterior = pistasColetadas;
            pistasColetadas = inserirPista(pistasColetadas, atual->pista);
            
            if (raizAnterior != pistasColetadas && raizAnterior != NULL) {
                // A inserção real (não duplicada) ocorreu
                printf(" Pista coletada e registrada.\n");
            } else if (raizAnterior == pistasColetadas && raizAnterior != NULL && strcmp(atual->pista, atual->nome) != 0) {
                // Se a BST não mudou e não é a raiz, a pista já havia sido coletada
                printf(" Pista já havia sido coletada.\n");
            }
        } else {
            printf("O cômodo parece estar limpo. Nenhuma pista visível aqui.\n");
        }

        // 2. Escolha de Navegação
        printf("\nPara onde deseja ir? **(e)**squerda, **(d)**ireita ou **(s)**air da exploração: ");
        if (scanf("%s", acao) != 1) {
             // Limpa o buffer de entrada em caso de erro
             while (getchar() != '\n'); 
             continue; // Tenta novamente
        }

        // Limpa o buffer de entrada
        while (getchar() != '\n'); 

        char escolha = tolower(acao[0]);

        if (escolha == 's') {
            printf("\nFim da exploração. Preparando para a fase de julgamento...\n");
            return pistasColetadas;
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf(" Não há saída para a esquerda neste cômodo. Tente outra direção.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf(" Não há saída para a direita neste cômodo. Tente outra direção.\n");
            }
        } else {
            printf("Opção inválida. Digite 'e', 'd' ou 's'.\n");
        }
    }
    return pistasColetadas; // Retorna a BST de pistas
}

// Função auxiliar para listar as pistas (In-Order Traversal da BST)
void listarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        listarPistas(raiz->direita);
    }
}

/**
 * Conduz à fase de julgamento final.
 * Conta quantas pistas coletadas apontam para o suspeito acusado.
 * A raiz da BST com todas as pistas que o jogador encontrou.
 * O ponteiro para a Tabela Hash de associações Pista/Suspeito.
 */
void verificarSuspeitoFinal(NoPista *pistasColetadas, TabelaHash *tabelaHash) {
    char acusado[TAMANHO_MAX_STRING];
    int contagemPistas = 0;

    printf("\n\n=============== FASE DE JULGAMENTO ==============\n");

    if (pistasColetadas == NULL) {
        printf(" Você não coletou nenhuma pista. A acusação será apenas um palpite!\n");
    } else {
        printf(" Pistas coletadas (em ordem alfabética):\n");
        listarPistas(pistasColetadas);
    }
    
    printf("\nCom base nas evidências, quem você acusa? (Digite o nome do suspeito): ");
    scanf("%s", acusado);
    // Limpa o buffer de entrada
    while (getchar() != '\n'); 

    printf("\nAnalisando as evidências coletadas contra **%s**...\n", acusado);

    // Função recursiva interna para contar as pistas
    void contarPistas(NoPista *no) {
        if (no != NULL) {
            contarPistas(no->esquerda);
            
            const char *suspeitoDaPista = encontrarSuspeito(tabelaHash, no->pista);
            
            if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, acusado) == 0) {
                contagemPistas++;
                printf("   [+] Pista '%s' aponta para %s.\n", no->pista, acusado);
            }
            
            contarPistas(no->direita);
        }
    }

    contarPistas(pistasColetadas);

    printf("\n--- RESULTADO DA ANÁLISE ---\n");
    printf("Número total de pistas contra %s: **%d**\n", acusado, contagemPistas);

    if (contagemPistas >= 2) {
        printf("\n SUCESSO! **%s** foi formalmente acusado!\n", acusado);
        printf("Pelo menos duas pistas substanciais (%d) sustentam a sua conclusão. Caso resolvido!\n", contagemPistas);
    } else {
        printf("\n FRACASSO! A acusação contra **%s** não pode ser sustentada.\n", acusado);
        printf("Você precisa de, pelo menos, 2 pistas. A falta de evidências substanciais leva à absolvição.\n");
    }
}

// Função para liberar a memória da Árvore Binária da mansão
void liberarMansao(NoSala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

// Função para liberar a memória da BST de Pistas
void liberarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// Função para liberar a memória da Tabela Hash
void liberarHash(TabelaHash *tabela) {
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        HashItem *atual = tabela->itens[i];
        while (atual != NULL) {
            HashItem *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}

// --- 4. FUNÇÃO PRINCIPAL (MAIN) ---

int main() {
    printf("==========================================\n");
    printf("        DETETIVE QUEST - CAPÍTULO FINAL\n");
    printf("==========================================\n");

    // Inicialização da Tabela Hash
    TabelaHash tabelaPistasSuspeitos;
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        tabelaPistasSuspeitos.itens[i] = NULL;
    }

    // Inicialização da BST de Pistas
    NoPista *pistasColetadas = NULL;

    // --- Montagem do Mapa Fixo da Mansão (Árvore Binária) ---
    printf("\n--- Montando a Mansão ---\n");

    // Nível 0 (Raiz)
    NoSala *hall = criarSala("Hall de Entrada", "Estrela Azul");

    // Nível 1
    hall->esquerda = criarSala("Cozinha", " Fogao sujo");
    hall->direita = criarSala("Sala de Jantar", "Pratos sujos na mesa");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Despensa", "Garrafa de azeite Vazia");
    hall->esquerda->direita = criarSala("Biblioteca", "Livros fora de armario");
    hall->direita->esquerda = criarSala("Sala de Estar", "Nenhum"); // Sem pista
    hall->direita->direita = criarSala("Quarto Principal", "Um pequeno alfinete de lapela");

    // Nível 3
    hall->esquerda->direita->esquerda = criarSala("Escritório", "Nenhum"); // Sem pista
    hall->direita->direita->esquerda = criarSala("Banheiro", "papel higienico");
    hall->direita->direita->direita = criarSala("Closet", "cabelo no chão");

    // --- Montagem das Associações Pista -> Suspeito (Tabela Hash) ---
    printf("\n--- Definindo as Associações de Pistas ---\n");
    
    // Suspeitos: Mordomo (Alfred), Jardineiro (Bartolomeu), Esposa (Cecília)
    inserirNaHash(&tabelaPistasSuspeitos, "Anel de Prata", "Bartolomeu");
    inserirNaHash(&tabelaPistasSuspeitos, "Fogão sujo", "Luzia");
    inserirNaHash(&tabelaPistasSuspeitos, "pratos sujos na mesa", "Sebastiao");
    inserirNaHash(&tabelaPistasSuspeitos, "Garrafa de Azeite Vazia", "Rafael");
    inserirNaHash(&tabelaPistasSuspeitos, "Livros fora do armario", "Emilly");
    inserirNaHash(&tabelaPistasSuspeitos, "Papel higienico", "Cecilia");
    inserirNaHash(&tabelaPistasSuspeitos, "Tubo de batom vermelho", "Cecilia");
    inserirNaHash(&tabelaPistasSuspeitos, "Cabelo no chão", "Cecilia");


    // --- Início do Jogo ---
    printf("\n================ INÍCIO DA EXPLORAÇÃO ================\n");
    
    // Inicia a exploração da mansão (navegação na Árvore Binária)
    pistasColetadas = explorarSalas(hall, pistasColetadas, &tabelaPistasSuspeitos);
    
    // Conduz a fase de julgamento (Verificação de Suspeito com BST e Hash)
    verificarSuspeitoFinal(pistasColetadas, &tabelaPistasSuspeitos);

    // --- Fim e Limpeza da Memória ---
    printf("\n--- Fim do Programa. Liberando memória ---\n");
    liberarMansao(hall);
    liberarPistas(pistasColetadas);
    liberarHash(&tabelaPistasSuspeitos);

    return 0;
}