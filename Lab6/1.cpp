// Faça um aplicativo com threads que processa uma matriz e realiza a soma dos elementos. Cada thread
// deve realizar a leitura de uma linha por vez e a armazenar a soma em uma variável compartilhada. Por
// exemplo, considere 3 threads (t1, t2, t3) e uma matriz de 10 linhas. As threads não sabem qual linha
// farão a leitura, até o momento que precisarem de mais dados para processar. Assim, a t1 pode ler linhas
// 0,2,8,9, a t2 pode ler as linhas 1,6, a thread 3 pode ler as linhas 3,4,5,7. É importante observar que após
// leitura de cada linha, a soma é armazenada em uma variável compartilhada antes de continuar a
// próxima leitura. Ao final, exibe a soma total.

#include <pthread.h>
#include <iostream>
#include <mutex>

std::mutex mutex;

int proxima_linha = 0;
int soma = 0;

constexpr int TOTAL_LINHAS = 10;
constexpr int TOTAL_COLUNAS = 5;
constexpr int TOTAL_THREADS = 3;

int matriz[TOTAL_LINHAS][TOTAL_COLUNAS] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20},
    {21, 22, 23, 24, 25},
    {26, 27, 28, 29, 30},
    {31, 32, 33, 34, 35},
    {36, 37, 38, 39, 40},
    {41, 42, 43, 44, 45},
    {46, 47, 48, 49, 50}
};

// Estrutura para passar informações da linha para a thread
struct LinhaInfo {
    int linha;
    int count;
    int *data;
};

// Rotina para ser executada por threads
void* somaLinha(void *arg) {
    (void)arg;

    while (true) {
        int local_pos;

        mutex.lock();
        if (proxima_linha >= TOTAL_LINHAS) {
            mutex.unlock();
            break;
        }
        local_pos = proxima_linha++;
        mutex.unlock();

        LinhaInfo info{local_pos, TOTAL_COLUNAS, matriz[local_pos]};

        int local_sum = 0;
        for (int i = 0; i < info.count; ++i) {
            local_sum += info.data[i];
        }

        mutex.lock();
        soma += local_sum;
        mutex.unlock();

        std::cout << "Thread " << (unsigned long)pthread_self()
                  << " => Linha " << info.linha
                  << ": count=" << info.count
                  << " sum=" << local_sum << std::endl;
    }

    pthread_exit(nullptr);
}

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    pthread_t threads[TOTAL_THREADS];

    for (int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_create(&threads[i], nullptr, somaLinha, nullptr);
    }

    for (int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "Soma total: " << soma << std::endl;

    return 0;
}
