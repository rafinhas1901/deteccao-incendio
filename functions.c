#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAMANHO 30

extern char floresta[TAMANHO][TAMANHO];
extern pthread_mutex_t mutex_matriz;

typedef struct {
    int x;
    int y;
} posicao;

void inicializa_matriz() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            floresta[i][j] = '-';
        }
    }
}

void* sensor(void* args) {
    posicao* p = (posicao*)args;
    while (1) {
        pthread_mutex_lock(&mutex_matriz);
        if (floresta[p->x][p->y] == '@') {
            if (p->x == 0 || p->x == TAMANHO - 1 || p->y == 0 || p->y == TAMANHO - 1) {
                comunica_central(p->x, p->y);
            }
            combate_fogo(p->x, p->y);
        }
        pthread_mutex_unlock(&mutex_matriz);
        usleep(100000);
    }
    free(p);
    return NULL;
}

void* gera_incendios(void* args) {
    while (1) {
        int x = rand() % TAMANHO;
        int y = rand() % TAMANHO;
        pthread_mutex_lock(&mutex_matriz);
        if (floresta[x][y] == '-') {
            floresta[x][y] = '@';
        }
        pthread_mutex_unlock(&mutex_matriz);
        sleep(3);
    }
    return NULL;
}

void* central(void* args) {
    while (1) {
        coleta_mensagens();
        usleep(100000);
    }
    return NULL;
}

void comunica_central(int x, int y) {
    printf("Incêndio detectado na posição (%d, %d)\n", x, y);
}

void combate_fogo(int x, int y) {
    floresta[x][y] = '/';
    printf("Incêndio combatido na posição (%d, %d)\n", x, y);
}

void coleta_mensagens() {
    // Simula a coleta de mensagens dos nós da borda
    // Aqui pode incluir a lógica de combate automático
}
