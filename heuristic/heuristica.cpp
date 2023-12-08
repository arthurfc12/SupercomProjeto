#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <vector>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <iostream>     // std::cout
#include <numeric>      // std::iota


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

// Função para calcular o grau de cada vértice
std::vector<int> CalcularGraus(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> graus(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            graus[i] += grafo[i][j];
        }
    }
    return graus;
}

std::vector<int> EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> graus = CalcularGraus(grafo, numVertices);
    std::vector<int> indices(numVertices);
    std::iota(indices.begin(), indices.end(), 0);

    // Ordenar os vértices pelo grau de adjacência
    std::sort(indices.begin(), indices.end(), [&graus](int a, int b) {
        return graus[a] > graus[b];
    });

    std::vector<int> cliqueMaxima;

    for (int i : indices) {
        bool podeAdicionar = true;
        for (int u : cliqueMaxima) {
            if (grafo[u][i] == 0) {
                podeAdicionar = false;
                break;
            }
        }

        if (podeAdicionar) {
            cliqueMaxima.push_back(i);
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
