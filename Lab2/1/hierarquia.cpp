// Cria uma árvore de processos com N níveis de profundidade, onde cada processo cria outros dois postos os níveis informados por usuário

#include <iostream> // cout
#include <unistd.h> // fork()
#include <sys/wait.h> // wait()

using namespace std;

void cria_niveis(int N_atual, int N) {
    if (N_atual >= N) return; // caso base

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();

        if (pid == 0) { // é filho
            cout << "PID: " << getpid()
                 << " | Pai: " << getppid()
                 << " | Nivel: " << N_atual + 1 << endl;

            cria_niveis(N_atual + 1, N);
            exit(0);
        }
    }

    wait(nullptr);
    wait(nullptr);
}

void hierarquia(int N){
    cria_niveis(1, N);
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        cout << "Uso: ./hierarquia N\n";
        return 1;
    }

    int N = atoi(argv[1]);

    cout << "PID: " << getpid() << " | PROCESSO PAI | Nível: 1" << endl;

    hierarquia(N);

    return 0;
}