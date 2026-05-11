#include <fstream>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>

// ============================
// CONFIGURAÇÕES
// ============================
constexpr size_t BUFFER_MAX = 5;                  // tamanho do buffer
constexpr int FLUSH_INTERVAL_MS = 5000;           // flush periódico

// ============================
// CONTROLE DE SINAIS
// ============================
volatile std::sig_atomic_t g_signal = 0;

/// Manipulador de sinais que captura SIGINT e SIGTERM
/// \param sinal Número do sinal recebido
void signalHandler(int sinal) {
    g_signal = sinal;
}

/// Configura os manipuladores de sinais SIGINT e SIGTERM
/// Define a função signalHandler como callback para ambos os sinais
void set_signals() {
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));

    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
}

// ============================
// ESCRITA SEGURA
// ============================
void flushBuffer(std::ofstream& arquivo,
                 std::vector<std::string>& buffer) {
    for (const auto& linha : buffer) {
        arquivo << linha << '\n';
    }

    arquivo.flush();
    buffer.clear();
}

// ============================
// MODO WRITE
// ============================
void modoWrite(const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo, std::ios::app);

    if (!arquivo) {
        std::cerr << "Erro ao abrir arquivo.\n";
        return;
    }

    std::vector<std::string> buffer;
    std::string linha;

    auto ultimoFlush = std::chrono::steady_clock::now();

    while (!g_signal) {
        std::cout << "Digite (Ctrl+C para sair): ";

        if (!std::getline(std::cin, linha)) {
            if (std::cin.eof()) break;

            std::cin.clear();
            continue;
        }

        buffer.push_back(linha);

        // Critério 1: buffer cheio
        if (buffer.size() >= BUFFER_MAX) {
            flushBuffer(arquivo, buffer);
        }

        // Critério 2: tempo
        auto agora = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            agora - ultimoFlush
        ).count();

        if (elapsed >= FLUSH_INTERVAL_MS) {
            if (!buffer.empty()) {
                flushBuffer(arquivo, buffer);
            }
            ultimoFlush = agora;
        }
    }

    // ============================
    // GRACEFUL SHUTDOWN
    // ============================
    std::cout << "\nEncerrando... ";

    if (g_signal == SIGINT)
        std::cout << "(SIGINT - usuario)\n";
    else if (g_signal == SIGTERM)
        std::cout << "(SIGTERM - sistema)\n";

    if (!buffer.empty()) {
        std::cout << "Salvando pendencias...\n";
        flushBuffer(arquivo, buffer);
    }

    std::cout << "Arquivo fechado com sucesso.\n";
}

// ============================
// MODO READ
// ============================
void modoRead(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);

    if (!arquivo) {
        std::cerr << "Erro ao abrir arquivo.\n";
        return;
    }

    std::string linha;

    while (!g_signal && std::getline(arquivo, linha)) {
        std::cout << linha << '\n';
    }

    std::cout << "\nLeitura encerrada por sinal: " << g_signal << "\n";
}

// ============================
// MAIN
// ============================
int main(int argc, char* argv[]) {
    set_signals();

    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <read|write> <arquivo>\n";
        return 1;
    }

    std::string modo = argv[1];
    std::string arquivo = argv[2];

    if (modo == "write") {
        modoWrite(arquivo);
    }
    else if (modo == "read") {
        modoRead(arquivo);
    }
    else {
        std::cerr << "Modo invalido.\n";
        return 1;
    }

    return 0;
}