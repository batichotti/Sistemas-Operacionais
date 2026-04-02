// Cria uma árvore de processos com N níveis de profundidade, onde cada processo cria outros dois postos os níveis informados por usuário

#include <iostream> // cout
#include <unistd.h> // fork()
#include <sys/wait.h> // wait()

using namespace std;

// Cria recursivamente uma árvore binária de processos até o nível N. Cada filho imprime seu PID, o PID do pai e o nível atual antes de continuar a expansão da árvore.
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

// Função interface que serve para deixar o chamado de função mais bonito e controlar a entrada do parâmetro N
void hierarquia(int N){
    if (N < 0) N*=-1;
    cria_niveis(1, N);
}

// Função principal: valida os argumentos de linha de comando, lê a profundidade desejada da árvore de processos e inicia a criação da hierarquia.
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