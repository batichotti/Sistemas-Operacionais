// Faça um aplicativo que execute dois tipos de threads: ping e a pong. As threads ping imprimem na tela
// a mensagem ping e as threads pong imprimem na tela a mensagem pong. As threads devem cooperar
// entre si para imprimir sempre alternadas as palavras ping e pong. O programa deve possibilitar informar
// a quantidade de threads a serem executadas de cada tipo.

#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mtx;
bool pingTurn = true;

void pingThread(int repeticoes) {
	for (int i = 0; i < repeticoes; ++i) {
		while (true) {
			std::unique_lock<std::mutex> lock(mtx);
			if (pingTurn) {
				std::cout << "ping" << std::endl;
				pingTurn = false;
				break;
			}
			lock.unlock();
			std::this_thread::yield();
		}
	}
}

void pongThread(int repeticoes) {
	for (int i = 0; i < repeticoes; ++i) {
		while (true) {
			std::unique_lock<std::mutex> lock(mtx);
			if (!pingTurn) {
				std::cout << "pong" << std::endl;
				pingTurn = true;
				break;
			}
			lock.unlock();
			std::this_thread::yield();
		}
	}
}

int main() {
	int qtdPing, qtdPong;

	std::cout << "Quantidade de threads ping: ";
	std::cin >> qtdPing;
	std::cout << "Quantidade de threads pong: ";
	std::cin >> qtdPong;

	if (qtdPing <= 0 || qtdPong <= 0) {
		std::cout << "As quantidades devem ser maiores que zero." << std::endl;
		return 1;
	}

	int alternancias = std::min(qtdPing, qtdPong);

	std::vector<std::thread> threads;
	threads.reserve(qtdPing + qtdPong);

	for (int i = 0; i < qtdPing; ++i) {
		int reps = (i < alternancias) ? 1 : 0;
		threads.emplace_back(pingThread, reps);
	}

	for (int i = 0; i < qtdPong; ++i) {
		int reps = (i < alternancias) ? 1 : 0;
		threads.emplace_back(pongThread, reps);
	}

	for (auto &t : threads) {
		t.join();
	}

	return 0;
}
