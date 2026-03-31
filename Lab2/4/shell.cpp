// executa_comando.cpp

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

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