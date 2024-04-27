#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banda.h"

#define MAX_ARTISTS 100
#define MAX_NAME_LENGTH 50
#define MAX_GENRE_LENGTH 50
#define MAX_LOCATION_LENGTH 50
#define MAX_ALBUMS 50
#define MAX_LINE_LENGTH 256

typedef struct Artista {
    char nome[MAX_NAME_LENGTH];
    char genero[MAX_GENRE_LENGTH];
    char localizacao[MAX_LOCATION_LENGTH];
    char albums[MAX_ALBUMS][MAX_NAME_LENGTH];
    int num_albums;
} Artista;

int num_lista = 0;

void leitorArtistas(Artista artistas[], int *num_artistas) {
    FILE *arq = fopen("artistas.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    *num_artistas=0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), arq) != NULL) {
        if (strlen(line) == 10 && strncmp(line, "==========", 10) == 0) {
            (*num_artistas)++;
            continue;
        }

        char *token = strtok(line, "\n");
        strcpy(artistas[*num_artistas].nome, token);
        fgets(line, sizeof(line), arq);
        strcpy(artistas[*num_artistas].genero, strtok(line, "\n"));
        fgets(line, sizeof(line), arq);
        strcpy(artistas[*num_artistas].localizacao, strtok(line, "\n"));
        artistas[*num_artistas].num_albums = 0;
        while (fgets(line, sizeof(line), arq) != NULL && strncmp(line, "==========", 10) != 0) {
            strcpy(artistas[*num_artistas].albums[artistas[*num_artistas].num_albums], strtok(line, "\n"));
            artistas[*num_artistas].num_albums++;
        }
        num_lista ++;
    }
    fclose(arq);
}

void mostrarLista(Artista artistas[], int num_lista) {
    for (int i = 0; i < num_lista; i++) {
        printf("Nome: %s\n", artistas[i].nome);
        printf("Gênero: %s\n", artistas[i].genero);
        printf("Local: %s\n", artistas[i].localizacao);
        printf("Álbuns:\n");
        for (int j = 0; j < artistas[i].num_albums; j++) {
            printf("- %s\n", artistas[i].albums[j]);
        }
        printf("==========\n");
    }
}

void inserirOrdenado(Artista artistas[], int *num_artistas, Artista novo) {
    int i = *num_artistas - 1;
    while (i >= 0 && strcmp(artistas[i].nome, novo.nome) > 0) {
        artistas[i + 1] = artistas[i];
        i--;
    }
    artistas[i + 1] = novo;
    (*num_artistas)++;
}

void removerArtista(Artista artistas[], int *num_artistas, const char *remove_name) {
    int index = -1;
    for (int i = 0; i < *num_artistas; i++) {
        if (strcmp(artistas[i].nome, remove_name) == 0) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < *num_artistas - 1; i++) {
            artistas[i] = artistas[i + 1];
        }
        (*num_artistas)--;
        printf("Artista removido com sucesso.\n");
    } else {
        printf("Artista não encontrado.\n");
    }
}

void editarArtista(Artista artistas[], int num_artistas, const char *edit_name) {
    int index = -1;
    for (int i = 0; i < num_artistas; i++) {
        if (strcmp(artistas[i].nome, edit_name) == 0) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        printf("Novo nome: ");
        scanf("%s", artistas[index].nome);
        printf("Novo gênero: ");
        scanf("%s", artistas[index].genero);
        printf("Novo local: ");
        scanf("%s", artistas[index].localizacao);
        printf("Número de álbuns: ");
        scanf("%d", &artistas[index].num_albums);
        for (int i = 0; i < artistas[index].num_albums; i++) {
            printf("Álbum %d: ", i + 1);
            scanf("%s", artistas[index].albums[i]);
        }
        printf("Artista editado com sucesso.\n");
    } else {
        printf("Artista não encontrado.\n");
    }
}

void buscaSequencial(Artista artistas[], int num_artistas) {
    int i, j;
    Artista key;
    for (i = 1; i < num_artistas; i++) {
        key = artistas[i];
        j = i - 1;
        while (j >= 0 && strcmp(artistas[j].nome, key.nome) > 0) {
            artistas[j + 1] = artistas[j];
            j = j - 1;
        }
        artistas[j + 1] = key;
    }
}

int buscaBinaria(Artista artistas[], int num_artistas, const char *search_name) {
    int esq = 0, dir = num_artistas - 1;
    while (esq <= dir) {
        int mid = esq + (dir - esq) / 2;
        int cmp = strcmp(artistas[mid].nome, search_name);
        if (cmp == 0)
            return mid;
        if (cmp < 0)
            esq = mid + 1;
        else
            dir = mid - 1;
    }
    return -1; // Elemento não encontrado
}

void menu() {
    printf("\n--- Menu ---\n");
    printf("1. Mostrar lista\n");
    printf("2. Inserir artista\n");
    printf("3. Remover artista\n");
    printf("4. Editar artista\n");
    printf("5. Busca binária por artista\n");
    printf("6. Busca sequencial por álbum\n");
    printf("7. Sair\n");
}

int main() {
    Artista artistas[MAX_ARTISTS];
    int num_artistas = 0;

    leitorArtistas(artistas, &num_artistas);
    buscaSequencial(artistas, num_artistas);

    int opcao;
    do {
        menu();
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Mostrar lista
                mostrarLista(artistas, num_lista);
                break;
            }

            case 2: {
                // Inserir artista
                Artista novo;
                printf("Nome: ");
                scanf("%s", novo.nome);
                printf("Gênero: ");
                scanf("%s", novo.genero);
                printf("Local: ");
                scanf("%s", novo.localizacao);
                printf("Número de álbuns: ");
                scanf("%d", &novo.num_albums);
                for (int i = 0; i < novo.num_albums; i++) {
                    printf("Álbum %d: ", i + 1);
                    scanf("%s", novo.albums[i]);
                }
                inserirOrdenado(artistas, &num_artistas, novo);
                break;
            }

            case 3: {
                // Remover artista
                char remove_name[MAX_NAME_LENGTH];
                printf("Nome do artista a ser removido: ");
                scanf("%s", remove_name);
                removerArtista(artistas, &num_artistas, remove_name);
                break;
            }

            case 4: {
                // Editar artista
                char edit_name[MAX_NAME_LENGTH];
                printf("Nome do artista a ser editado: ");
                scanf("%s", edit_name);
                editarArtista(artistas, num_artistas, edit_name);
                break;
            }

            case 5: {
                // Busca binária por artista
                char search_name[MAX_NAME_LENGTH];
                printf("Nome do artista a ser buscado: ");
                scanf("%s", search_name);
                int index = buscaBinaria(artistas, num_artistas, search_name);
                if (index != -1) {
                    printf("Artista encontrado:\n");
                    mostrarLista(&artistas[index], 1);
                } else {
                    printf("Artista não encontrado.\n");
                }
                break;
            }

            case 6: {
                // Busca sequencial por álbum
                char search_album[MAX_NAME_LENGTH];
                printf("Nome do álbum a ser buscado: ");
                scanf("%s", search_album);
                int found = 0;
                for (int i = 0; i < num_artistas; i++) {
                    for (int j = 0; j < artistas[i].num_albums; j++) {
                        if (strcmp(artistas[i].albums[j], search_album) == 0) {
                            printf("Álbum encontrado:\n");
                            mostrarLista(&artistas[i], 1);
                            found = 1;
                            break;
                        }
                    }
                }
                if (!found) {
                    printf("Álbum não encontrado.\n");
                }
                break;
            }

            case 7:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 7);

    FILE *arq = fopen("artistas.txt", "a"); // Abre o arquivo para escrita
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo 'artistas.txt' para escrita.\n");
        exit(1);
    }

    for (int i = 0; i < num_artistas; i++) {
        fprintf(arq, "\n%s\n%s\n%s\n", artistas[i].nome, artistas[i].genero, artistas[i].localizacao);
        for (int j = 0; j < artistas[i].num_albums; j++) {
            fprintf(arq, "%s\n", artistas[i].albums[j]);
        }
        fprintf(arq, "\n==========");
    }

    fclose(arq);

    return 0;
}
