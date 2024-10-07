#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAMANHO 30

typedef struct {
    int x;
    int y;
} posicao;

char floresta[TAMANHO][TAMANHO];
pthread_mutex_t mutex_matriz;

void inicializa_matriz();
void* sensor(void* args);
void* gera_incendios(void* args);
void* central(void* args);
void comunica_central(int x, int y);
void combate_fogo(int x, int y);
void coleta_mensagens();

int main() {
    pthread_t sensores[TAMANHO][TAMANHO];
    pthread_t gerador_incendio, thread_central;

    srand(time(NULL));
    pthread_mutex_init(&mutex_matriz, NULL);

    inicializa_matriz();

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            posicao* p = malloc(sizeof(posicao));
            p->x = i;
            p->y = j;
            pthread_create(&sensores[i][j], NULL, sensor, p);
        }
    }

    pthread_create(&gerador_incendio, NULL, gera_incendios, NULL);
    pthread_create(&thread_central, NULL, central, NULL);

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            pthread_join(sensores[i][j], NULL);
        }
    }

    pthread_join(gerador_incendio, NULL);
    pthread_join(thread_central, NULL);
    pthread_mutex_destroy(&mutex_matriz);

    return 0;
}
