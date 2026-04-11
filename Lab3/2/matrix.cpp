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
#include <cstdlib> // rand()
#include <ctime> // time()

struct DataStruct {
    const std::vector<std::vector<int>>* matriz;
    std::vector<double>* resultado;
    int comeco;
    int fim;
    int linhas;
    int colunas;
};

/**
 * Calcula a média aritmética das linhas atribuídas para esta thread.
 * arg Ponteiro para DataStruct com intervalo de linhas.
 * return nullptr
 */
void* worker_media_aritmetica(void* arg) {
    DataStruct* dados = static_cast<DataStruct*>(arg);
    
    for (int i = dados->comeco; i < dados->fim; ++i) {
        long long int soma = 0;
        for (int j = 0; j < dados->colunas; ++j) {
            soma += (*dados->matriz)[i][j];
        }
        (*dados->resultado)[i] = static_cast<double>(soma) / dados->colunas;
    }
    
    return nullptr;
}

/**
 * Calcula a média geométrica das colunas atribuídas para esta thread.
 * arg Ponteiro para DataStruct com intervalo de colunas.
 * return nullptr
 */
void* worker_media_geometrica(void* arg) {
    DataStruct* dados = static_cast<DataStruct*>(arg);
    
    for (int j = dados->comeco; j < dados->fim; ++j) {
        double soma_log = 0.0;
        bool valido = true;
        for (int i = 0; i < dados->linhas; ++i) {
            int valor = (*dados->matriz)[i][j];
            if (valor <= 0) {
                (*dados->resultado)[j] = 0.0;
                valido = false;
                break;
            }
            soma_log += std::log(static_cast<double>(valor));
        }
        if (valido) {
            (*dados->resultado)[j] = std::exp(soma_log / dados->linhas);
        }
    }
    
    return nullptr;
}

/**
 * Lê uma matriz de um arquivo no formato MxN.
 * caminho Caminho do arquivo de entrada.
 * linhas Referência para retornar quantidade de linhas.
 * colunas Referência para retornar quantidade de colunas.
 * return Matriz lida do arquivo.
 */
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

/**
 * Salva uma matriz em arquivo no formato MxN.
 * caminho Caminho do arquivo de saída.
 * matriz Matriz que será salva.
 */
static void salva_matriz_arquivo(const std::string& caminho, const std::vector<std::vector<int>>& matriz) {
    std::ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo para escrita: " + caminho);
    }
    
    int linhas = matriz.size();
    int colunas = matriz[0].size();
    
    arquivo << linhas << "x" << colunas << "\n";
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            arquivo << matriz[i][j];
            if (j < colunas - 1) arquivo << " ";
        }
        arquivo << "\n";
    }
}

/**
 * Gera uma matriz com valores aleatórios no intervalo [1, limite].
 * linhas Quantidade de linhas da matriz.
 * colunas Quantidade de colunas da matriz.
 * limite Limite superior para os valores gerados.
 * return Matriz gerada.
 */
static std::vector<std::vector<int>> gera_matriz_aleatoria(int linhas, int colunas, int limite) {
    std::vector<std::vector<int>> matriz(linhas, std::vector<int>(colunas));
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            matriz[i][j] = (rand() % limite) + 1;
        }
    }
    return matriz;
}

/**
 * Salva em arquivo as médias calculadas por linha e por coluna.
 * caminho Caminho do arquivo de saída.
 * medias_linhas Vetor com as médias aritméticas por linha.
 * medias_colunas Vetor com as médias geométricas por coluna.
 */
static void salva_resultados_arquivo(const std::string& caminho, 
                                     const std::vector<double>& medias_linhas,
                                     const std::vector<double>& medias_colunas) {
    std::ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de resultados: " + caminho);
    }
    
    arquivo << std::fixed << std::setprecision(6);
    
    arquivo << "=== RESULTADOS DO PROCESSAMENTO DA MATRIZ ===\n\n";
    
    arquivo << "MEDIAS ARITMETICAS POR LINHA:\n";
    arquivo << "------------------------------\n";
    for (size_t i = 0; i < medias_linhas.size(); ++i) {
        arquivo << "Linha " << i << ": " << medias_linhas[i] << "\n";
    }
    
    arquivo << "\nMEDIAS GEOMETRICAS POR COLUNA:\n";
    arquivo << "-------------------------------\n";
    for (size_t j = 0; j < medias_colunas.size(); ++j) {
        arquivo << "Coluna " << j << ": " << medias_colunas[j] << "\n";
    }
}

/**
 * Exibe instruções de uso do programa.
 * programa Nome do executável.
 */
static void mostrar_uso(const char* programa) {
    std::cout << "Uso: " << programa << " -i <arquivo_entrada> [-o <arquivo_saida>] [opcoes]\n";
    std::cout << "Opcoes:\n";
    std::cout << "  -i <arquivo>      Arquivo da matriz (entrada ou destino da geracao)\n";
    std::cout << "  -o <arquivo>      Arquivo de resultados (padrao: resultados.txt)\n";
    std::cout << "  -n <threads>      Quantidade de threads (padrao: 2)\n";
    std::cout << "  -g                Gera matriz aleatoria antes de processar\n";
    std::cout << "  -L <linhas>       Linhas para geracao (padrao: 10)\n";
    std::cout << "  -C <colunas>      Colunas para geracao (padrao: 10)\n";
    std::cout << "  -h                Mostra esta ajuda\n";
}

/**
 * Este é o começo do programa.
 *
 * Faz o parse dos argumentos, opcionalmente gera a matriz, executa os cálculos em paralelo e salva o resultado em arquivo.
 *
 * argc Quantidade de argumentos da linha de comando.
 * argv Vetor com os argumentos da linha de comando.
 * return 0 em caso de sucesso, 1 em caso de erro.
 */
int main(int argc, char const* argv[]) {
    std::string arquivo_entrada = "";
    std::string arquivo_saida = "resultados.txt";
    int num_threads = 2;
    int gera_matriz = 0;
    int linhas_gerar = 10, colunas_gerar = 10;
    
    // argumentos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) {
            arquivo_entrada = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            arquivo_saida = argv[++i];
        } else if (arg == "-n" && i + 1 < argc) {
            num_threads = std::atoi(argv[++i]);
        } else if (arg == "-g") {
            gera_matriz = 1;
        } else if (arg == "-L" && i + 1 < argc) {
            linhas_gerar = std::atoi(argv[++i]);
        } else if (arg == "-C" && i + 1 < argc) {
            colunas_gerar = std::atoi(argv[++i]);
        } else if (arg == "-h") {
            mostrar_uso(argv[0]);
            return 0;
        }
    }
    
    // Validação
    if (arquivo_entrada.empty()) {
        std::cerr << "Erro: arquivo de entrada obrigatorio (-i)\n";
        std::cerr << "Use -h para ver os parametros disponiveis\n";
        return 1;
    }
    
    if (num_threads < 1) {
        std::cerr << "Erro: numero de threads deve ser >= 1\n";
        return 1;
    }
    
    try {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        std::vector<std::vector<int>> matriz;
        int linhas = 0, colunas = 0;
        
        // Gerar matriz se solicitado
        if (gera_matriz) {
            std::cout << "Gerando matriz aleatoria " << linhas_gerar << "x" << colunas_gerar << "...\n";
            matriz = gera_matriz_aleatoria(linhas_gerar, colunas_gerar, 100);
            linhas = linhas_gerar;
            colunas = colunas_gerar;
            
            salva_matriz_arquivo(arquivo_entrada, matriz);
            std::cout << "Matriz salva em: " << arquivo_entrada << "\n";
            std::cout << "Processando a matriz gerada...\n";
        } else {
            // Ler matriz de arquivo
            std::cout << "Lendo matriz do arquivo: " << arquivo_entrada << "\n";
            matriz = ler_matriz_arquivo(arquivo_entrada, linhas, colunas);
        }
        
        std::cout << "Matriz lida com sucesso: " << linhas << "x" << colunas << "\n";
        
        std::vector<double> medias_linha(linhas, 0.0);
        std::vector<double> medias_coluna(colunas, 0.0);
        
        std::cout << "\n=== Calculando medias aritmeticas por linha (" << num_threads 
                  << " threads) ===\n";
        
        // threads para média aritmética com divisão de dados
        std::vector<pthread_t> threads_linha(num_threads);
        std::vector<DataStruct> args_linha(num_threads);
        
        for (int t = 0; t < num_threads; ++t) {
            int comeco = (t * linhas) / num_threads;
            int fim = ((t + 1) * linhas) / num_threads;
            
            args_linha[t] = {&matriz, &medias_linha, comeco, fim, linhas, colunas};
            int status = pthread_create(&threads_linha[t], nullptr, worker_media_aritmetica, &args_linha[t]);
            if (status != 0) {
                throw std::runtime_error("Falha ao criar thread de linha");
            }
        }
        
        // Aguardar término das threads
        for (int t = 0; t < num_threads; ++t) {
            pthread_join(threads_linha[t], nullptr);
        }
        
        std::cout << "=== Calculando medias geometricas por coluna (" << num_threads 
                  << " threads) ===\n";
        
        // threads para média geométrica com divisão de dados
        std::vector<pthread_t> threads_coluna(num_threads);
        std::vector<DataStruct> args_coluna(num_threads);
        
        for (int t = 0; t < num_threads; ++t) {
            int comeco = (t * colunas) / num_threads;
            int fim = ((t + 1) * colunas) / num_threads;
            
            args_coluna[t] = {&matriz, &medias_coluna, comeco, fim, linhas, colunas};
            int status = pthread_create(&threads_coluna[t], nullptr, worker_media_geometrica, &args_coluna[t]);
            if (status != 0) {
                throw std::runtime_error("Falha ao criar thread de coluna");
            }
        }
        
        // Aguardar término das threads
        for (int t = 0; t < num_threads; ++t) {
            pthread_join(threads_coluna[t], nullptr);
        }
        
        std::cout << std::fixed << std::setprecision(6);

        std::cout << "\nMedias aritmeticas por linha:\n";
        for (int i = 0; i < linhas; ++i) {
            std::cout << "Linha " << i << ": " << medias_linha[i] << "\n";
        }

        std::cout << "\nMedias geometricas por coluna:\n";
        for (int j = 0; j < colunas; ++j) {
            std::cout << "Coluna " << j << ": " << medias_coluna[j] << "\n";
        }
        
        // Salvar resultados em arquivo
        salva_resultados_arquivo(arquivo_saida, medias_linha, medias_coluna);
        std::cout << "\nResultados salvos em: " << arquivo_saida << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
