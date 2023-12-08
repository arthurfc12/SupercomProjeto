import matplotlib.pyplot as plt

# Dados para plotagem
lista_vertices = [10, 20, 30]
lista_mpi = [0.061566, 41.8498, 41893.3]
lista_openmp = [11.7029, 36.0252, 19665.9]
lista_heuristica = [0.071788, 0.078141, 0.111251]
lista_exaustiva = [1.06789, 445.013, 505186]

# Criando o gráfico
plt.figure(figsize=(10, 6))
plt.plot(lista_vertices, lista_mpi, label='MPI', marker='o')
plt.plot(lista_vertices, lista_openmp, label='OpenMP', marker='o')
plt.plot(lista_vertices, lista_heuristica, label='Heurística', marker='o')
plt.plot(lista_vertices, lista_exaustiva, label='Exaustiva', marker='o')

# Adicionando título e rótulos
plt.title('Comparação de Desempenho por Método de Paralelismo')
plt.xlabel('Número de Vértices')
plt.ylabel('Tempo de Execução (ms)')
#plt.yscale('log')  # Escala logarítmica para melhor visualização de grandes variações
plt.legend()

# Mostrando o gráfico
plt.show()

