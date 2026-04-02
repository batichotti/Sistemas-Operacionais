// Cria um proto-shell responsável por executar comandos informados por usuário com capacidade de execução não bloqueante em segundo plano (&).

#include <iostream> // cout
#include <unistd.h> // fork()
#include <sys/wait.h> // wait()
#include <vector> // vector<T>
#include <sstream> //  stringstream()
#include <cstring> // strcmp()

using namespace std;

// Executa um comando tokenizado. Recebe os tokens já separados por espaço. Detecta o símbolo "&" para execução em segundo plano. Converte os tokens para o formato esperado por execvp(). Cria um processo filho para executar o comando. Aguarda o término do filho apenas quando não houver "&".
void cmd(vector<string>& tokens) {
    vector<char*> args;
    bool daemon_flag = false;

    for (auto& token : tokens) {
        if(token == "&") {daemon_flag = true; continue; }
        args.push_back(strdup(token.c_str()));
    }
    args.push_back(nullptr); // obrigatório pro execvp

    pid_t pid = fork();

    if (!pid) { // filho
        execvp(args[0], args.data());
        perror("Erro no exec");
        exit(1);
    }

    if (!daemon_flag) wait(nullptr);

    // libera memória
    for (char* arg : args) {
        free(arg);
    }
}

// Loop principal do shell. Exibe o prompt, lê uma linha da entrada padrão, encerra ao receber "exit" e encaminha os comandos tokenizados para cmd() quando houver entrada válida.
int main() {
    string linha;

    while (true) {
        cout << "# ";
        getline(cin, linha);

        if (linha == "exit") break;

        stringstream ss(linha);
        vector<string> tokens;
        string temp;

        while (ss >> temp) {
            tokens.push_back(temp);
        }

        if (!tokens.empty()) {
            cmd(tokens);
        }
    }

    return 0;
}