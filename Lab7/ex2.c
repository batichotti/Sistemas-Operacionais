// Problema do Barbeiro Dorminhoco com semáforos POSIX.
//
// Regras deste exemplo:
// - Há 1 barbeiro e uma sala de espera com CADEIRAS assentos.
// - Se não houver lugar, o cliente vai embora.
// - O barbeiro dorme (bloqueia) quando não há clientes.
//
// Semáforos usados:
// - clientes: quantidade de clientes aguardando atendimento.
// - barbeiros: sinaliza que um cliente pode ser atendido.
// - mutex: exclusão mútua para a variável compartilhada "esperando".

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

// Número atual de clientes sentados na sala de espera.
static int esperando = 0;

// Thread do barbeiro: espera clientes e realiza cortes indefinidamente.
void *barbeiro(void *arg) {
	(void)arg;

	for (;;) {
		// Dorme até existir ao menos um cliente aguardando.
		sem_wait(&clientes);
		// Entra na região crítica para atualizar o estado da fila.
		sem_wait(&mutex);
		esperando--;
		// Libera exatamente um cliente para a cadeira do barbeiro.
		sem_post(&barbeiros);
		sem_post(&mutex);

		printf("Barbeiro cortando cabelo...\n");
		// Simula o tempo de corte.
		usleep(200000);
	}

	return NULL;
}

// Thread de cliente: tenta sentar na espera e aguarda atendimento.
void *cliente(void *arg) {
	int id = *(int *)arg;

	// Tenta entrar na sala de espera (região crítica).
	sem_wait(&mutex);
	if (esperando < CADEIRAS) {
		// Há cadeira disponível: cliente aguarda.
		esperando++;
		printf("Cliente %d aguardando. (%d/%d)\n", id, esperando, CADEIRAS);
		// Notifica o barbeiro que existe cliente.
		sem_post(&clientes);
		sem_post(&mutex);

		// Aguarda chamada do barbeiro para ser atendido.
		sem_wait(&barbeiros);
		printf("Cliente %d sendo atendido.\n", id);
	} else {
		// Não há cadeira: cliente desiste e sai.
		printf("Cliente %d foi embora (sem cadeiras).\n", id);
		sem_post(&mutex);
	}

	return NULL;
}

int main(void) {
	// Thread única do barbeiro e um conjunto fixo de clientes.
	pthread_t th_barbeiro;
	pthread_t th_clientes[CLIENTES_TOTAL];
	int ids[CLIENTES_TOTAL];

	// Inicialização dos semáforos.
	// clientes e barbeiros começam em 0; mutex começa liberado (1).
	sem_init(&clientes, 0, 0);
	sem_init(&barbeiros, 0, 0);
	sem_init(&mutex, 0, 1);

	// Inicia o barbeiro.
	pthread_create(&th_barbeiro, NULL, barbeiro, NULL);

	// Cria clientes com pequena defasagem para simular chegadas no tempo.
	for (int i = 0; i < CLIENTES_TOTAL; i++) {
		ids[i] = i + 1;
		pthread_create(&th_clientes[i], NULL, cliente, &ids[i]);
		usleep(100000);
	}

	// Aguarda todos os clientes finalizarem.
	for (int i = 0; i < CLIENTES_TOTAL; i++) {
		pthread_join(th_clientes[i], NULL);
	}

	// Encerra a thread do barbeiro e libera recursos.
	pthread_cancel(th_barbeiro);
	pthread_join(th_barbeiro, NULL);

	sem_destroy(&clientes);
	sem_destroy(&barbeiros);
	sem_destroy(&mutex);

	return 0;
}
    