#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// --------------------
// Definição das structs
// --------------------
typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// --------------------
// Variável global de controle de IDs
// --------------------
int contadorID = 0;

// --------------------
// Protótipos das funções
// --------------------
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void enfileirar(Fila *fila, Peca pec);
Peca desenfileirar(Fila *fila);
void empilhar(Pilha *pilha, Peca pec);
Peca desempilhar(Pilha *pilha);
Peca gerarPeca();
void exibirEstado(Fila *fila, Pilha *pilha);
void trocarTopoComFrente(Fila *fila, Pilha *pilha);
void trocaMultipla(Fila *fila, Pilha *pilha);

// --------------------
// Função principal
// --------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for(int i=0; i<TAM_FILA; i++)
        enfileirar(&fila, gerarPeca());

    do {
        exibirEstado(&fila, &pilha);
        printf("\n1-Jogar 2-Reservar 3-Usar 4-Troca Topo/Frente 5-Troca Multipla 0-Sair\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                if(!filaVazia(&fila)) {
                    Peca p = desenfileirar(&fila);
                    printf("Jogou [%c %d]\n", p.nome, p.id);
                    enfileirar(&fila, gerarPeca());
                } else printf("Fila vazia!\n");
                break;
            case 2:
                if(!filaVazia(&fila)) {
                    if(!pilhaCheia(&pilha)) {
                        Peca p = desenfileirar(&fila);
                        empilhar(&pilha, p);
                        printf("Reservou [%c %d]\n", p.nome, p.id);
                        enfileirar(&fila, gerarPeca());
                    } else printf("Pilha cheia!\n");
                } else printf("Fila vazia!\n");
                break;
            case 3:
                if(!pilhaVazia(&pilha)) {
                    Peca p = desempilhar(&pilha);
                    printf("Usou [%c %d]\n", p.nome, p.id);
                } else printf("Pilha vazia!\n");
                break;
            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    return 0;
}

// --------------------
// Funções auxiliares
// --------------------
void inicializarFila(Fila *fila) { fila->frente = 0; fila->tras = -1; fila->qtd = 0; }
void inicializarPilha(Pilha *pilha) { pilha->topo = -1; }

int filaVazia(Fila *fila) { return fila->qtd == 0; }
int filaCheia(Fila *fila) { return fila->qtd == TAM_FILA; }
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == TAM_PILHA-1; }

void enfileirar(Fila *fila, Peca pec) {
    if(filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = pec;
    fila->qtd++;
}

Peca desenfileirar(Fila *fila) {
    Peca rem = {'?', -1};
    if(filaVazia(fila)) return rem;
    rem = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->qtd--;
    return rem;
}

void empilhar(Pilha *pilha, Peca pec) {
    if(pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->pecas[pilha->topo] = pec;
}

Peca desempilhar(Pilha *pilha) {
    Peca rem = {'?', -1};
    if(pilhaVazia(pilha)) return rem;
    rem = pilha->pecas[pilha->topo];
    pilha->topo--;
    return rem;
}

Peca gerarPeca() {
    char tipos[] = {'I','O','T','L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n----- ESTADO -----\nFila: ");
    int i = fila->frente;
    for(int c = 0; c < fila->qtd; c++) {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\nPilha (Topo->Base): ");
    for(int j = pilha->topo; j >= 0; j--)
        printf("[%c %d] ", pilha->pecas[j].nome, pilha->pecas[j].id);
    printf("\n------------------\n");
}

void trocarTopoComFrente(Fila *fila, Pilha *pilha) {
    if(filaVazia(fila) || pilhaVazia(pilha)) return;
    Peca temp = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
}

void trocaMultipla(Fila *fila, Pilha *pilha) {
    if(fila->qtd < 3 || pilha->topo < 2) return;
    Peca temp[3];
    for(int i = 0; i < 3; i++)
        temp[i] = fila->pecas[(fila->frente + i) % TAM_FILA];
    for(int i = 0; i < 3; i++) {
        fila->pecas[(fila->frente + i) % TAM_FILA] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp[i];
    }
}
