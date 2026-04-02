// Opera uma busca sequêncial em um vetor não ordenado com N filhos procurando de maneira paralela o número informado.

#include <iostream>   // cout
#include <unistd.h>   // fork()
#include <sys/wait.h> // wait()
#include <vector>     // vector<T>
#include <cstdlib>    // rand()

// Gera um vetor aleatório com valores inteiros entre 0 e um máximo informado.
std::vector<int> gera_vetor_aleatorio(int tamanho, int max)
{
    std::vector<int> v;

    for (int i = 0; i < tamanho; i++)
    {
        v.push_back(rand() % (max + 1));
    }

    return v;
}

// Faz uma busca sequencial em um vetor, se encontado visa o PID de quem encontrou e onde encontrou.
void procura_elemento(std::vector<int> v, int ini, int fim, int elem)
{

    int status = 1;

    for (int i = 0; i < (fim - ini); i++)
    {
        if (v[ini + i] == elem)
        {
            std::cout << "PID: " << getpid() << " -> Vetor[" << ini + i << "] == " << elem << std::endl;
            status = 0;
        }
    }

    exit(status);
}

// Calcula o pedaço do vetor que cada filho ficará responsável, cria os filhos e os atribui sua tarefa. Espera os filhos terminarem a execução e a partir do exit code checa se o número foi encontrado.
void busca_paralela(std::vector<int> v, int tamanho_vetor, int valor_procurado, int n_filhos)
{
    int step = (int)tamanho_vetor / n_filhos;
    int resto = tamanho_vetor % n_filhos;

    for (int i = 0; i < n_filhos; i++)
    {
        if (!fork())
        {
            if (i == n_filhos - 1) procura_elemento(v, i * step, (i + 1) * step + resto, valor_procurado);
            procura_elemento(v, i * step, (i + 1) * step, valor_procurado);
            break;
        }
    }

    int nao_encontrado = false;
    for (int i = 0; i < n_filhos; i++)
    {
        int status;
        wait(&status);

        if (WEXITSTATUS(status) == 1)
        {
            nao_encontrado = true;
        }
    }

    if (nao_encontrado)
        std::cout << "PROCESSO PAI -> VALOR " << valor_procurado << " NÃO ENCONTRADO" << std::endl;

    // for (int e : v) std::cout << e << " ";
    // std::cout << std::endl;
}

// Função principal: valida argumentos de entrada, gera o vetor aleatório e inicia a busca paralela pelo valor informado.
int main(int argc, char const *argv[])
{

    if (argc < 5)
    {
        std::cout << "Uso: ./vector <TAMANHO_VETOR> <MAX_VETOR> <VALOR_PROCURADO> <#_FILHOS>" << std::endl;
        return 1;
    }

    int tamanho_vetor = std::atoi(argv[1]);
    int max_vetor = std::atoi(argv[2]);
    int valor_procurado = std::atoi(argv[3]);
    int n_filhos = std::atoi(argv[4]);

    if (tamanho_vetor <= 0 || max_vetor <= 0 || n_filhos <= 0)
    {
        std::cout << "Erro: TAMANHO_VETOR, MAX_VETOR e #_FILHOS devem ser maiores que 0" << std::endl;
        return 1;
    }

    std::vector<int> v = gera_vetor_aleatorio(tamanho_vetor, max_vetor);

    // std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    busca_paralela(v, tamanho_vetor, valor_procurado, n_filhos);

    return 0;
}
