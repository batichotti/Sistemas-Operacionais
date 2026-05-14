// Leitores/Escritores com Monitor

#include <mutex>
#include <condition_variable>
#include <cstdlib>

#define VALORMAXIMO 10

int gera_numero_aleatorio(){
    return (std::rand() % VALORMAXIMO);
}

class Buffer {
    private:
        int buffer = 0;
        int leitores = 0;
        bool escrevendo = false;

        std::mutex mutex;
        std::condition_variable pode_ler;
        std::condition_variable pode_escrever;

    public:
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