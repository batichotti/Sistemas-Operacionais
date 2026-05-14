/**
 * @file ex1.cpp
 * @brief Implementação do padrão Leitores/Escritores com Monitor
 * 
 * Este programa implementa um buffer compartilhado com sincronização usando
 * variáveis de condição (condition_variable) e mutex para coordenar múltiplos
 * leitores e escritores de forma segura.
 */

#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

#define VALORMAXIMO 10

/**
 * @brief Gera um número aleatório entre 0 e VALORMAXIMO-1
 * @return int - Número aleatório gerado
 */
int gera_numero_aleatorio(){
    return (std::rand() % VALORMAXIMO);
}

/**
 * @class Buffer
 * @brief Monitor para gerenciar leitura e escrita compartilhada de dados
 * 
 * Implementa o padrão Leitores/Escritores onde:
 * - Múltiplos leitores podem ler simultaneamente
 * - Um escritor tem acesso exclusivo
 * - Leitores bloqueiam enquanto há escritor ativo
 */
class Buffer {
    private:
        int buffer;                             ///< Dado compartilhado
        int leitores;                           ///< Contador de leitores ativos
        bool escrevendo;                        ///< Flag de escrita em progresso

        std::mutex mutex;                       ///< Mutex para proteção crítica
        std::condition_variable pode_ler;       ///< CV para sincronizar leitores
        std::condition_variable pode_escrever;  ///< CV para sincronizar escritores

    public:
        Buffer() : buffer(0), leitores(0), escrevendo(false) {}

        /**
         * @brief Lê o valor do buffer de forma segura
         * 
         * Múltiplos leitores podem executar simultaneamente enquanto não há
         * escritor. Bloqueia se algum escritor está ativo.
         * 
         * @return int - Valor lido do buffer
         */
        int ler(){
            std::unique_lock<std::mutex> lock(mutex, std::defer_lock);

            lock.lock();
            pode_ler.wait(lock, [this]{ return !escrevendo; });
            leitores++;
            lock.unlock();

            int item = buffer;

            lock.lock();
            leitores--;
            if (leitores == 0)
                pode_escrever.notify_one();
            lock.unlock();

            return item;
        }

        /**
         * @brief Escreve um novo valor aleatório no buffer de forma exclusiva
         * 
         * Bloqueia até que não haja leitores nem outros escritores ativos.
         * Garante acesso exclusivo durante a escrita.
         */
        void escrever(){
            std::unique_lock<std::mutex> lock(mutex, std::defer_lock);

            lock.lock();
            pode_escrever.wait(lock, [this]{
                return leitores == 0 && !escrevendo;
            });
            escrevendo = true;
            lock.unlock();

            buffer = gera_numero_aleatorio();

            lock.lock();
            escrevendo = false;
            pode_ler.notify_all();
            pode_escrever.notify_one();
            lock.unlock();
        }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Buffer b;
    std::vector<std::thread> leitores;
    std::vector<std::thread> escritores;

    for (int i = 0; i < 2; ++i) {
        escritores.emplace_back([&b]() {
            for (int j = 0; j < 5; ++j) {
                b.escrever();
            }
        });
    }

    for (int i = 0; i < 3; ++i) {
        leitores.emplace_back([&b]() {
            for (int j = 0; j < 5; ++j) {
                int valor = b.ler();
                std::cout << "Leitura: " << valor << '\n';
            }
        });
    }

    for (std::thread &t : escritores) {
        t.join();
    }

    for (std::thread &t : leitores) {
        t.join();
    }

    return 0;
}