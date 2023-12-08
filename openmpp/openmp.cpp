#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <algorithm>

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

std::vector<int> EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> cliqueMaxima;
    std::vector<int> cliqueAtual;

    #pragma omp parallel for private(cliqueAtual)
    for (int i = 1; i < (1 << numVertices); ++i) {
        cliqueAtual.clear();
        for (int j = 0; j < numVertices; ++j) {
            if (i & (1 << j)) {
                cliqueAtual.push_back(j);
            }
        }

        if (Eclique(grafo, cliqueAtual)) {
            #pragma omp critical
            {
                if (cliqueAtual.size() > cliqueMaxima.size()) {
                    cliqueMaxima = cliqueAtual;
                }
            }
        }
    }

    return cliqueMaxima;
}

int main() {
    int numVertices;
    std::string nomeArquivo = "../grafo.txt";

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    std::vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

    std::cout << "Clique maxima encontrada: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";
    }
    std::cout << std::endl;

    std::cout << "Tempo de execução: " << duration_ms.count() << " ms" << std::endl;

    return 0;
}
