// Implemente um programa multithread com pthreads que calcule:
// a) a média aritmética de cada linha de uma matriz MxN e devolva o resultado em um vetor de tamanho M.
// b) a média geométrica de cada coluna de uma matriz MxN e devolva o resultado em um vetor de tamanho N.
// O programa deve gerar matrizes MxN com elementos aleatórios para arquivos; usar técnicas de paralelização de funções e de dados; ler matrizes MxN de arquivos no formato em anexo; gravar os resultados em um arquivo texto.

#include <fstream> // ler arquivo .in
#include <iostream> // cin & cout
#include <stdexcept> // erros e tratamento de excessões
#include <string> // manipulação de strings
#include <vector> // matriz
#include <cmath> // pow
#include <iomanip> // setprecision
#include <pthread.h> // threads

struct DataStruct {
    const std::vector<std::vector<int>>* matriz;
    int linha;
    int coluna;
    double* resultado;
};

void* media_aritmetica_linha(void* arg) {
    DataStruct* dados = static_cast<DataStruct*>(arg);

    long long int soma = 0;
    int colunas = static_cast<int>((*dados->matriz)[dados->linha].size());
    for (int j = 0; j < colunas; ++j) {
        soma += (*dados->matriz)[dados->linha][j];
    }

    *dados->resultado = static_cast<double>(soma) / colunas;

    return nullptr;
}

void* media_geometrica_coluna(void* arg) {
    DataStruct* dados = static_cast<DataStruct*>(arg);

    double soma_log = 0.0;
    int linhas = static_cast<int>(dados->matriz->size());
    for (int i = 0; i < linhas; i++) {
        int valor = (*dados->matriz)[i][dados->coluna];
        if (valor <= 0) {
            *dados->resultado = 0.0;
            return nullptr;
        }
        soma_log += std::log(static_cast<double>(valor));
    }

    *dados->resultado = std::exp(soma_log / linhas);

    return nullptr;
}

static std::vector<std::vector<int>> ler_matriz_arquivo(const std::string& caminho, int& linhas, int& colunas) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);
    }

    char separador = 'x';
    if (!(arquivo >> linhas >> separador >> colunas) || separador != 'x' || linhas <= 0 || colunas <= 0) {
        throw std::runtime_error("Cabecalho invalido. Formato esperado: MxN");
    }

    std::vector<std::vector<int>> matriz(linhas, std::vector<int>(colunas));
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (!(arquivo >> matriz[i][j])) {
                throw std::runtime_error("Quantidade de valores insuficiente para a dimensao informada");
            }
        }
    }

    return matriz;
}

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada.in>\n";
        return 1;
    }

    int linhas = 0;
    int colunas = 0;

    try {
        auto matriz = ler_matriz_arquivo(argv[1], linhas, colunas);
        std::cout << "Matriz lida com sucesso: " << linhas << "x" << colunas << "\n";
        std::cout << "Primeiro elemento: " << matriz[0][0] << "\n";

        std::vector<double> medias_linha(linhas, 0.0);
        std::vector<double> medias_coluna(colunas, 0.0);
        std::vector<pthread_t> threads_linha(linhas);
        std::vector<pthread_t> threads_coluna(colunas);
        std::vector<DataStruct> args_linha(linhas);
        std::vector<DataStruct> args_coluna(colunas);

        for (int i = 0; i < linhas; ++i) {
            args_linha[i] = {&matriz, i, 0, &medias_linha[i]};
            int status = pthread_create(&threads_linha[i], nullptr, media_aritmetica_linha, &args_linha[i]);
            if (status != 0) {
                throw std::runtime_error("Falha ao criar thread de linha");
            }
        }
        
        for (int i = 0; i < colunas; ++i) {
            args_coluna[i] = {&matriz, 0, i, &medias_coluna[i]};
            int status = pthread_create(&threads_coluna[i], nullptr, media_geometrica_coluna, &args_coluna[i]);
            if (status != 0) {
                throw std::runtime_error("Falha ao criar thread de coluna");
            }
        }

        for (int i = 0; i < linhas; ++i) {
            pthread_join(threads_linha[i], nullptr);
        }

        for (int i = 0; i < colunas; ++i) {
            pthread_join(threads_coluna[i], nullptr);
        }

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "\nMedias aritmeticas por linha:\n";
        for (int i = 0; i < linhas; ++i) {
            std::cout << "Linha " << i << ": " << medias_linha[i] << "\n";
        }

        std::cout << "\nMedias geometricas por coluna:\n";
        for (int i = 0; i < colunas; ++i) {
            std::cout << "Coluna " << i << ": " << medias_coluna[i] << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler arquivo: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
