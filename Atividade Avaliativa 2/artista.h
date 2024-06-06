#ifndef MUSICA_H
#define MUSICA_H

typedef struct Musica Musica;
typedef struct ListaDuplamenteEncadiada ListaDuplamenteEncadiada;

Musica* criaNo();
void adicionarMusica();
int removerNo();
void mostarMusica();
void mostrarMusicaOrdenada();
Musica* buscar();
void atualizarMusica();
void salvarMusica();
void inserindoMusica();
void apagarMusica();
void menu();

#endif
