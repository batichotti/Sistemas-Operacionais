// Executa comandos como filho

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void cmd(const char* argv[]){
    pid_t pid = fork();

    if (!pid){ // É o processo filho? (PID == 0)
        execvp(argv[1], (char* const*)&argv[1]);
    }

    wait(nullptr);
    std::cout << "Finalizado com sucesso" << std::endl;
}

int main(int argc, char const *argv[]){
 
    if (argc < 2) {
        std::cout << "Uso: ./executa_comando <comando> [args...]\n";
        return 1;
    }

    cmd(argv);

    return 0;
}
