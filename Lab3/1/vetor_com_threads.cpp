// Opera uma busca sequêncial em um vetor não ordenado com N threads procurando de maneira paralela o número informado.

#include <iostream>   // cout
#include <vector>     // vector<T>
#include <cstdlib>    // rand()
#include <pthread.h>  // pthreads

// Struct para empacotar os argumentos de cada thread
struct ThreadArgs {
    std::vector<int>* v;
    int ini;
    int fim;
    int elem;
    bool encontrado;
};

// Gera um vetor aleatório com valores inteiros entre 0 e um máximo informado.
std::vector<int> gera_vetor_aleatorio(int tamanho, int max){
    std::vector<int> v;

    for (int i = 0; i < tamanho; i++){
        v.push_back(rand() % (max + 1));
    }

    return v;
}

// Faz uma busca sequencial no intervalo atribuído à thread.
// Assinatura obrigatória para pthread: void* func(void*)
void* procura_elemento(void* arg){
    ThreadArgs* args = (ThreadArgs*) arg;
    args->encontrado = false;

    for (int i = args->ini; i < args->fim; i++){
        if ((*args->v)[i] == args->elem){
            std::cout << " -> Vetor[" << i << "] == " << args->elem << std::endl;
            args->encontrado = true;
        }
    }

    return nullptr;
}

// Divide o vetor entre as threads, cria e aguarda cada uma, depois verifica se o valor foi encontrado.
void busca_paralela(std::vector<int>& v, int tamanho_vetor, int valor_procurado, int n_filhos){
    int step = tamanho_vetor / n_filhos;
    int resto = tamanho_vetor % n_filhos;

    pthread_t threads[n_filhos];
    ThreadArgs args[n_filhos]; // cada thread tem seus próprios argumentos

    for (int i = 0; i < n_filhos; i++){
        args[i].v    = &v;
        args[i].ini  = i * step;
        args[i].fim  = (i == n_filhos - 1) ? (i + 1) * step + resto : (i + 1) * step;
        args[i].elem = valor_procurado;

        // Passa ponteiro para a struct como argumento da thread
        pthread_create(&threads[i], NULL, procura_elemento, &args[i]);
    }

    bool encontrado = false;
    for (int i = 0; i < n_filhos; i++){
        pthread_join(threads[i], NULL);

        if (args[i].encontrado)
            encontrado = true;
    }

    if (!encontrado)
        std::cout << "THREAD PRINCIPAL -> VALOR " << valor_procurado << " NÃO ENCONTRADO" << std::endl;
}

// Função principal: valida argumentos de entrada, gera o vetor aleatório e inicia a busca paralela.
int main(int argc, char const *argv[]){

    if (argc < 5){
        std::cout << "Uso: ./vector <TAMANHO_VETOR> <MAX_VETOR> <VALOR_PROCURADO> <#_FILHOS>" << std::endl;
        return 1;
    }

    int tamanho_vetor = std::atoi(argv[1]);
    int max_vetor = std::atoi(argv[2]);
    int valor_procurado = std::atoi(argv[3]);
    int n_filhos = std::atoi(argv[4]);

    if (tamanho_vetor <= 0 || max_vetor <= 0 || n_filhos <= 0){
        std::cout << "Erro: TAMANHO_VETOR, MAX_VETOR e #_FILHOS devem ser maiores que 0" << std::endl;
        return 1;
    }

    std::vector<int> v = gera_vetor_aleatorio(tamanho_vetor, max_vetor);

    busca_paralela(v, tamanho_vetor, valor_procurado, n_filhos);

    return 0;
}