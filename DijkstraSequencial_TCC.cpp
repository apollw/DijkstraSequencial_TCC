// DijkstraSequencial_TCC.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#define _CRT_SECURE_NO_WARNINGS 
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define MAX_NOS 16384
#define NUM_VERTICES 256 /*512*/ /*1024*/ /*2048*/ /*4096*/ /*8192*/ /*16384*/
#define MIN_PESO 1
#define MAX_PESO 20
#define DIST_MAX INT_MAX

struct No {
    int vertice;
    int peso;
    struct No* prox;
};

struct Grafo {
    struct No* cabeca[NUM_VERTICES];
    int numVertices;
};

struct No* criarNo(int v, int p) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    novoNo->vertice = v;
    novoNo->peso = p;
    novoNo->prox = NULL;
    return novoNo;
}

struct Grafo* criarGrafo(int vertices) {
    struct Grafo* grafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    grafo->numVertices = vertices;

    for (int i = 0; i < vertices; i++) {
        grafo->cabeca[i] = NULL;
    }

    return grafo;
}

void adicionarAresta(struct Grafo* grafo, int orig, int dest, int peso) {
    struct No* novoNo = criarNo(dest, peso);
    novoNo->prox = grafo->cabeca[orig];
    grafo->cabeca[orig] = novoNo;
}

void imprimirGrafo(struct Grafo* grafo) {
    printf("\nGrafo:\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        struct No* temp = grafo->cabeca[i];
        printf("Vertice %d: ", i);
        while (temp != NULL) {
            printf("(%d,%d) -> ", temp->vertice, temp->peso);
            temp = temp->prox;
        }
        printf("NULL\n");
    }
}

/*Salvar o grafo em um arquivo*/
void salvarGrafo(struct Grafo* grafo, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    // Escreve o número de vértices no arquivo
    fprintf(arquivo, "%d\n", grafo->numVertices);

    // Escreve as arestas do grafo no arquivo
    for (int i = 0; i < grafo->numVertices; i++) {
        struct No* temp = grafo->cabeca[i];
        while (temp != NULL) {
            fprintf(arquivo, "%d %d %d\n", i, temp->vertice, temp->peso);
            temp = temp->prox;
        }
    }

    fclose(arquivo);
    printf("Grafo salvo com sucesso no arquivo %s.\n", nomeArquivo);
}

/*Carregar o grafo de um arquivo salvo*/
struct Grafo* carregarGrafo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return NULL;
    }

    int numVertices;
    fscanf(arquivo, "%d", &numVertices);

    struct Grafo* grafo = criarGrafo(numVertices);

    int origem, destino, peso;
    while (fscanf(arquivo, "%d %d %d", &origem, &destino, &peso) == 3) {
        adicionarAresta(grafo, origem, destino, peso);
    }

    fclose(arquivo);
    printf("Grafo carregado com sucesso do arquivo %s.\n", nomeArquivo);
    return grafo;
}

/*Calcula o tempo decorrido na execução*/
double get_current_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * 1e-6;
}

/*Algoritmo de Dijkstra Implementado*/
void dijkstra(struct Grafo* grafo, int inicio) {
    int distancias[NUM_VERTICES];
    bool visitados[NUM_VERTICES];

    for (int i = 0; i < NUM_VERTICES; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = false;
    }

    distancias[inicio] = 0;

    for (int count = 0; count < NUM_VERTICES - 1; count++) {
        int u = -1;

        for (int i = 0; i < NUM_VERTICES; i++) {
            if (!visitados[i] && (u == -1 || distancias[i] < distancias[u])) {
                u = i;
            }
        }

        visitados[u] = true;

        struct No* v = grafo->cabeca[u];

        while (v != NULL) {
            if (!visitados[v->vertice] &&
                distancias[u] + v->peso < distancias[v->vertice]) {
                distancias[v->vertice] = distancias[u] + v->peso;
            }
            v = v->prox;
        }
    }

    //printf("\nDistancias minimas a partir do vertice %d:\n", inicio);
    //for (int i = 0; i < NUM_VERTICES; i++) {
    //    printf("Vertice %d: %d\n", i, distancias[i]);
    //}
}

int main() {

    struct Grafo* grafo = criarGrafo(NUM_VERTICES);
    int peso = 0;
    int numArestas = 0;
    
    const char* grafo256   = "D:\\Grafos\\grafo256.txt";
    /*const char* grafo512 = "D:\\Grafos\\grafo512.txt";
    const char* grafo1024  = "D:\\Grafos\\grafo1024.txt";
    const char* grafo2048  = "D:\\Grafos\\grafo2048.txt";
    const char* grafo4096  = "D:\\Grafos\\grafo4096.txt";
    const char* grafo8192  = "D:\\Grafos\\grafo8192.txt";
    const char* grafo16384 = "D:\\Grafos\\grafo16384.txt";*/

    //Inicializa os pesos da cada aresta do grafo
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = i + 1; j < NUM_VERTICES; j++) {
            peso++;
            adicionarAresta(grafo, i, j, peso);
            adicionarAresta(grafo, j, i, peso);

            numArestas++;
            if (peso > 20)
                peso = 0;
        }
    }

    //Calculo do Tamanho do Grafo
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = i + 1; j < NUM_VERTICES; j++) {
            numArestas++;
        }
    }

    printf("Numero de Vertices = %d\n", NUM_VERTICES);
    printf("Numero de Arestas  = %d\n", numArestas);

    grafo = carregarGrafo(grafo256);
    //imprimirGrafo(grafo);

    double start_time = get_current_time();
    //Executa o algoritmo 30 vezes a partir da origem m
    for (int m = 0; m < 30; m++)
        dijkstra(grafo, m);
    double end_time = get_current_time();
    printf("\nTempo de execucao: %f segundos\n", end_time - start_time);

    free(grafo);

    return 0;
}
