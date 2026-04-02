// Executa um comando informado a partir de um processo filho do programa vigente

#include <iostream> // cout
#include <unistd.h> // fork()
#include <sys/wait.h> // wait()

// Executa o comando informado em um processo filho e aguarda sua finalização.
void cmd(char* argv[]){
    pid_t pid = fork();

    if (!pid){ // É o processo filho? (PID == 0)
        execvp(argv[1], (char* const*)&argv[1]);
        _exit(0);
    }

    wait(nullptr);
    std::cout << "Finalizado com sucesso" << std::endl;
}

// Função principal: valida os argumentos e delega a execução do comando para `cmd`.
int main(int argc, char* argv[]){
 
    if (argc < 2) {
        std::cout << "Uso: ./executa_comando <comando> [args...]\n";
        return 1;
    }

    cmd(argv);

    return 0;
}
