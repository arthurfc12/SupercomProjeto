#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <vector>
#include <algorithm>


// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;
    printf("Grafo com %d vértices e %d arestas\n", numVertices, numArestas);

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
    
    // Testando todas as combinações possíveis de vértices
    for (int i = 1; i < (1 << numVertices); ++i) {
        std::vector<int> cliqueAtual;
        for (int j = 0; j < numVertices; ++j) {
            if (i & (1 << j)) {
                cliqueAtual.push_back(j);
            }
        }

        if (Eclique(grafo, cliqueAtual) && cliqueAtual.size() > cliqueMaxima.size()) {
            cliqueMaxima = cliqueAtual;
        }
    }

    return cliqueMaxima;
}

int main() {
    // Criar um grafo de exemplo
    int numVertices;
    std::string nomeArquivo = "../grafo.txt";

    // Medir o tempo de início
    auto start_time = std::chrono::high_resolution_clock::now();

    // Encontrar a clique máxima
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    std::vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);

    // Medir o tempo de término
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calcular a duração
    std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

    // Imprimir a clique máxima
    std::cout << "Clique maxima encontrada: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";  // +1 para ajustar a indexação base-0 para base-1
    }
    std::cout << std::endl;

    // Imprimir o tempo de execução
    std::cout << "Tempo de execução: " << duration_ms.count() << " ms" << std::endl;

    return 0;
}
