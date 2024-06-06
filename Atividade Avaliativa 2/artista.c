#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"

// Estrutura para um nó da lista
typedef struct Musica {
    char artista[100];
    char musica[100];
    struct Musica* anterior;
    struct Musica* prox;
} Musica;

// Estrutura para a lista circular duplamente encadeada
typedef struct ListaDuplamenteEncadiada{
    Musica* inicio;
} ListaDuplamenteEncadiada;

// Função para criar um novo nó
Musica* criarNo(const char* artista, const char* musica) {
    Musica* novoNo = (Musica*)malloc(sizeof(Musica));
    strcpy(novoNo->artista, artista);
    strcpy(novoNo->musica, musica);
    novoNo->anterior = novoNo;
    novoNo->prox = novoNo;
    return novoNo;
}

// Função para adicionar um nó à lista
void adicionarMusica(ListaDuplamenteEncadiada* lista, const char* artista, const char* musica) {
    Musica* novoNo = criarNo(artista, musica);
    if (lista->inicio == NULL) {
        lista->inicio = novoNo;
    } else {
        Musica* final = lista->inicio->anterior;
        final->prox = novoNo;
        novoNo->anterior = final;
        novoNo->prox = lista->inicio;
        lista->inicio->anterior = novoNo;
    }
}

// Função para remover um nó da lista
int removerNo(ListaDuplamenteEncadiada* lista, const char* musica) {
    if (lista->inicio == NULL) return 0;
    Musica* corrente = lista->inicio;
    do {
        if (strcmp(corrente->musica, musica) == 0) {
            if (corrente->prox == corrente) {
                lista->inicio = NULL;
            } else {
                corrente->anterior->prox = corrente->prox;
                corrente->prox->anterior = corrente->anterior;
                if (lista->inicio == corrente) lista->inicio = corrente->prox;
            }
            free(corrente);
            return 1;
        }
        corrente = corrente->prox;
    } while (corrente != lista->inicio);
    return 0;
}

// Função para exibir a playlist na ordem de cadastro
void mostarMusica(ListaDuplamenteEncadiada* lista) {
    if (lista->inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    Musica* corrente = lista->inicio;
    do {
        printf("%s - %s\n", corrente->artista, corrente->musica);
        corrente = corrente->prox;
    } while (corrente != lista->inicio);
}

// Função para exibir a playlist ordenada pelo nome das músicas
void mostrarMusicaOrdenada(ListaDuplamenteEncadiada* lista) {
    if (lista->inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    // Contando o número de nós
    int contador = 0;
    Musica* corrente = lista->inicio;
    do {
        contador++;
        corrente = corrente->prox;
    } while (corrente != lista->inicio);

    // Criando um array para armazenar os nós
    Musica* nos[contador];
    corrente = lista->inicio;
    for (int i = 0; i < contador; i++) {
        nos[i] = corrente;
        corrente = corrente->prox;
    }

    // Ordenando os nós pelo nome da música
    for (int i = 0; i < contador - 1; i++) {
        for (int j = i + 1; j < contador; j++) {
            if (strcmp(nos[i]->musica, nos[j]->musica) > 0) {
                Musica* temp = nos[i];
                nos[i] = nos[j];
                nos[j] = temp;
            }
        }
    }

    // Exibindo os nós ordenados
    for (int i = 0; i < contador; i++) {
        printf("%s - %s\n", nos[i]->artista, nos[i]->musica);
    }
}

// Função para buscar uma música na playlist
Musica* buscar(ListaDuplamenteEncadiada* lista, const char* musica) {
    if (lista->inicio == NULL) return NULL;
    Musica* corrente = lista->inicio;
    do {
        if (strcmp(corrente->musica, musica) == 0) {
            return corrente;
        }
        corrente = corrente->prox;
    } while (corrente != lista->inicio);
    return NULL;
}

// Função para carregar a playlist do arquivo
void atualizarMusica(ListaDuplamenteEncadiada* list, const char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char* artist = strtok(line, ";");
        char* musica = strtok(NULL, "\n");
        if (artist && musica) {
            adicionarMusica(list, artist, musica);
        }
    }
    fclose(file);
}

// Função para salvar a playlist no arquivo
void salvarMusica(ListaDuplamenteEncadiada* lista, const char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }
    if (lista->inicio != NULL) {
        Musica* corrente = lista->inicio;
        do {
            fprintf(file, "%s;%s\n", corrente->artista, corrente->musica);
            corrente = corrente->prox;
        } while (corrente != lista->inicio);
    }
    fclose(file);
}

// Função para inserir uma nova música na playlist e no arquivo
void inserindoMusica(ListaDuplamenteEncadiada* lista, const char* nomeArquivo) {
    char artista[100];
    char musica[100];
    printf("Digite o nome do artista: ");
    fgets(artista, sizeof(artista), stdin);
    artista[strcspn(artista, "\n")] = 0;
    printf("Digite o nome da música: ");
    fgets(musica, sizeof(musica), stdin);
    musica[strcspn(musica, "\n")] = 0;

    adicionarMusica(lista, artista, musica);
    salvarMusica(lista, nomeArquivo);
    printf("Música inserida com sucesso.\n");
}

// Função para remover uma música da playlist e do arquivo
void apagarMusica(ListaDuplamenteEncadiada* lista, const char* nomeArquivo) {
    char musica[100];
    printf("Digite o nome da música a ser removida: ");
    fgets(musica, sizeof(musica), stdin);
    musica[strcspn(musica, "\n")] = 0;

    if (removerNo(lista, musica)) {
        salvarMusica(lista, nomeArquivo);
        printf("Música removida com sucesso.\n");
    } else {
        printf("Música não encontrada.\n");
    }
}

// Função para exibir o menu e processar as opções do usuário
void menu(ListaDuplamenteEncadiada* lista, const char* nomeArquivo) {
    int opcao;
    Musica* corrente = lista->inicio;

    do {
        printf("\nMenu:\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir nova música\n");
        printf("4. Remover música\n");
        printf("5. Buscar música\n");
        printf("6. Avançar para próxima música\n");
        printf("7. Retornar à música anterior\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                mostarMusica(lista);
                break;
            case 2:
                mostrarMusicaOrdenada(lista);
                break;
            case 3:
                inserindoMusica(lista, nomeArquivo);
                break;
            case 4:
                apagarMusica(lista, nomeArquivo);
                break;
            case 5: {
                char musica[100];
                printf("Digite o nome da música a ser buscada: ");
                fgets(musica, sizeof(musica), stdin);
                musica[strcspn(musica, "\n")] = 0;

                Musica* encontrar = buscar(lista, musica);
                if (encontrar) {
                    printf("Música encontrada: %s - %s\n", encontrar->artista, encontrar->musica);
                } else {
                    printf("Música não encontrada.\n");
                }
                break;
            }
            case 6:
                if (corrente) {
                    corrente = corrente->prox;
                    printf("Música atual: %s - %s\n", corrente->artista, corrente->musica);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 7:
                if (corrente) {
                    corrente = corrente->anterior;
                    printf("Música atual: %s - %s\n", corrente->artista, corrente->musica);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 8:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 8);
}

int main() {
    ListaDuplamenteEncadiada musica = { NULL };
    const char* nomeArquivo = "musicas.txt";
    atualizarMusica(&musica, nomeArquivo);
    menu(&musica, nomeArquivo);
    return 0;
}
