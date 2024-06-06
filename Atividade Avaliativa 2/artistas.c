#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct "Musica"
typedef struct Musica {
  char nomeArtista[50];
  char nomeMusica[50];
  struct Musica *anterior;
  struct Musica *proximo;
} Musica;

// Função para ler uma linha do arquivo
char *lerLinha(FILE *arquivo, char *linha) {
  if (fgets(linha, 256, arquivo) == NULL) {
    return NULL;
  }
  
  linha[strlen(linha) - 1] = '\0';

  return linha;
}

// Função para separar o nome do artista e a música da linha
void separarDados(char *linha, Musica *musica) {
  char *pontoVirgula = strchr(linha, ';');
  if (pontoVirgula == NULL) {
    return;
  }

  strncpy(musica->nomeArtista, linha, pontoVirgula - linha);
  musica->nomeArtista[pontoVirgula - linha] = '\0';

  pontoVirgula++;
  while (*pontoVirgula == ' ') {
    pontoVirgula++;
  }

  strcpy(musica->nomeMusica, pontoVirgula);
}

// Função para adicionar uma música à lista circular
void adicionarMusica(Musica **lista, Musica *musica) {
  if (*lista == NULL) {
    *lista = musica;
    musica->anterior = musica;
    musica->proximo = musica;
    return;
  }

  Musica *aux = *lista;
  musica->anterior = aux->anterior;
  musica->proximo = aux;
  aux->anterior->proximo = musica;
  aux->anterior = musica;
}

// Função para remover uma música da lista circular
void removerMusica(Musica **lista, Musica *musica) {
  if (*lista == NULL) {
    return;
  }

  if (*lista == musica && (*lista)->proximo == musica) {
    free(musica);
    *lista = NULL;
    return;
  }

  if (*lista == musica) {
    *lista = musica->proximo;
    musica->proximo->anterior = musica->anterior;
    musica->anterior->proximo = musica->proximo;
  } else {
    musica->anterior->proximo = musica->proximo;
    musica->proximo->anterior = musica->anterior;
  }

  free(musica);
}

// Função para atualizar o arquivo de músicas
void atualizarArquivo(Musica *lista) {
  FILE *arquivo = fopen("musicas.txt", "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para escrita!\n");
    return;
  }

  Musica *aux = lista;
  do {
    fprintf(arquivo, "%s;%s\n", aux->nomeArtista, aux->nomeMusica);
    aux = aux->proximo;
  } while (aux != lista);

  fclose(arquivo);
}

// Função para imprimir a lista de músicas
void imprimirLista(Musica *lista) {
  if (lista == NULL) {
    printf("A lista está vazia.\n");
    return;
  }

  Musica *aux = lista;
  do {
    printf("Artista: %s - Música: %s\n", aux->nomeArtista, aux->nomeMusica);
    aux = aux->proximo;
  } while (aux != lista);
}

int main() {
  FILE *arquivo = fopen("musicas.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return 1;
  }
  
  Musica *lista = NULL;

  char linha[256];
  while (lerLinha(arquivo, linha) != NULL) {
    Musica *musica = malloc(sizeof(Musica));
    separarDados(linha, musica);

    adicionarMusica(&lista, musica);
  }

  fclose(arquivo);

  imprimirLista(lista);

  // Libera a memória alocada dinamicamente
  Musica *aux = lista;
  while (aux != NULL) {
    Musica *temp = aux;
    aux = aux->proximo;
    free(temp);
  }

  return 0;
}
