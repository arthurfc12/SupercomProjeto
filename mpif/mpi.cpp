#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <mpi.h>

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();

    return grafo;
}

bool Eclique(const std::vector<std::vector<int>>& grafo, const std::vector<int>& vertices) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (grafo[vertices[i]][vertices[j]] == 0) {
                return false;
            }
        }
    }
    return true;
}


// Funções LerGrafo e Eclique permanecem as mesmas

std::vector<int> EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices, int rank, int size) {
    std::vector<int> cliqueMaxima;
    std::vector<int> cliqueAtual;

    int range = (1 << numVertices) / size;  // Dividir o espaço de busca pelo número de processos
    int start = rank * range;
    int end = (rank + 1) * range;

    for (int i = start; i < end; ++i) {
        cliqueAtual.clear();
        for (int j = 0; j < numVertices; ++j) {
            if (i & (1 << j)) {
                cliqueAtual.push_back(j);
            }
        }

        if (Eclique(grafo, cliqueAtual)) {
            if (cliqueAtual.size() > cliqueMaxima.size()) {
                cliqueMaxima = cliqueAtual;
            }
        }
    }

    // Comunicar cliques máximas locais para encontrar a clique máxima global
    // Código para reduzir e encontrar a clique máxima global aqui

    return cliqueMaxima;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<int>> grafo;
    int numVertices;

    if (rank == 0) {
        grafo = LerGrafo("../grafo.txt", numVertices);
        // Código para distribuir o grafo entre os processos aqui
    }

    // Código para sincronizar e receber o grafo em outros processos aqui

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices, rank, size);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

    if (rank == 0) {
        std::cout << "Clique maxima encontrada: ";
        for (int v : cliqueMaxima) {
            std::cout << v + 1 << " ";
        }
        std::cout << std::endl;

        std::cout << "Tempo de execução: " << duration_ms.count() << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
