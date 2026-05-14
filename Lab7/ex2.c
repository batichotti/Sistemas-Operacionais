// Barbeiro Dorminhoco com Semáforos

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CADEIRAS 5
#define CLIENTES_TOTAL 12

static sem_t clientes;
static sem_t barbeiros;
static sem_t mutex;

static int esperando = 0;

void *barbeiro(void *arg) {
	(void)arg;

	for (;;) {
		sem_wait(&clientes);
		sem_wait(&mutex);
		esperando--;
		sem_post(&barbeiros);
		sem_post(&mutex);

		printf("Barbeiro cortando cabelo...\n");
		usleep(200000);
	}

	return NULL;
}

void *cliente(void *arg) {
	int id = *(int *)arg;

	sem_wait(&mutex);
	if (esperando < CADEIRAS) {
		esperando++;
		printf("Cliente %d aguardando. (%d/%d)\n", id, esperando, CADEIRAS);
		sem_post(&clientes);
		sem_post(&mutex);

		sem_wait(&barbeiros);
		printf("Cliente %d sendo atendido.\n", id);
	} else {
		printf("Cliente %d foi embora (sem cadeiras).\n", id);
		sem_post(&mutex);
	}

	return NULL;
}

int main(void) {
	pthread_t th_barbeiro;
	pthread_t th_clientes[CLIENTES_TOTAL];
	int ids[CLIENTES_TOTAL];

	sem_init(&clientes, 0, 0);
	sem_init(&barbeiros, 0, 0);
	sem_init(&mutex, 0, 1);

	pthread_create(&th_barbeiro, NULL, barbeiro, NULL);

	for (int i = 0; i < CLIENTES_TOTAL; i++) {
		ids[i] = i + 1;
		pthread_create(&th_clientes[i], NULL, cliente, &ids[i]);
		usleep(100000);
	}

	for (int i = 0; i < CLIENTES_TOTAL; i++) {
		pthread_join(th_clientes[i], NULL);
	}

	pthread_cancel(th_barbeiro);
	pthread_join(th_barbeiro, NULL);

	sem_destroy(&clientes);
	sem_destroy(&barbeiros);
	sem_destroy(&mutex);

	return 0;
}
    