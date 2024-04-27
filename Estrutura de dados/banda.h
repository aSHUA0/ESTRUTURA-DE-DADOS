#ifndef BANDA_H
#define BANDA_H

typedef struct Artista Artista;

void leitorArtistas(Artista artistas[], int *num_artistas);
void mostrarLista(Artista artistas[], int num_lista);
void inserirOrdenado(Artista artistas[], int *num_artistas, Artista novo);
void removerArtista(Artista artistas[], int *num_artistas, const char *remove_name);
void editarArtista(Artista artistas[], int num_artistas, const char *edit_name);
void buscaSequencial(Artista artistas[], int num_artistas);
int buscaBinaria(Artista artistas[], int num_artistas, const char *search_name);
void menu();

#endif
