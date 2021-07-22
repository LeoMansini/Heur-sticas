#include "grafo.cpp"
#include "hvmc.cpp"
#include "hamc.cpp"
#include "hAGM.cpp"
#include "tabu_search.cpp"
#include "mbe.cpp"
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>
using namespace std;

int main(int argc, char** argv){

    printf("Starting program of TSP\n");

    
    int n1;
    int n2;
    float peso;
    int cant_vertices;
    int cant_aristas;
    vector<int> camino;
    float resultado;

    // Leemos el parametro que indica el algoritmo a ejecutar.
    map<string, string> algoritmos_implementados = {
        {"H1", "Vecino mas cercano"}, {"H2", "Arista mas corta"}, {"H3", "Arbol generador minimo"}, 
        {"MH1", "Memoria basada en soluciones exploradas"}, {"MH2", "Memoria basada en estructura"}
    };

    map<string, int> params_metaheuristicas;

    // Verificar que el algoritmo pedido exista.
    if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end()){
        cerr << "Algoritmo no encontrado: " << argv[1] << endl;
        cerr << "Los algoritmos existentes son: " << endl;
        for (auto& alg_desc: algoritmos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
        return 0;
    }
    else if (argc == 6){
        printf("argv1 value is: %s\n", argv[1]);
        printf("argv2 value is: %d\n", stoi(argv[2]));
        printf("argv3 value is: %s\n", argv[3]);
        params_metaheuristicas = {
            {"tabuMemorySize", stoi(argv[2])}, {"cantIteraciones", stoi(argv[3])},
            {"porcentajeRandomizacion", stoi(argv[4])}, {"iteracionesHeuristicas", stoi(argv[5])}
        };
    }
    string algoritmo = argv[1];


    // Leemos el input.
    cin >> cant_vertices >> cant_aristas;

    //vector<vector<int>> matriz_adyacencias = vector<vector<int>>(cant_vertices+1, vector<int>(cant_vertices+1, 0)); // nunca accedamos a la matriz en las pociciones [0][*] o [*][0]

    Grafo grafo(cant_vertices);

    for (int i = 0; i < cant_aristas; i++){
        cin >> n1 >> n2 >> peso;
        grafo.agregarArista(n1, n2, peso);
    }
     /* Entrada3 a mano
    Grafo grafo(5);
    grafo.agregarArista(1, 2, 1);
    grafo.agregarArista(1, 3, 2);
    grafo.agregarArista(1, 4, 3);
    grafo.agregarArista(1, 5, 4);
    grafo.agregarArista(2, 3, 5);
    grafo.agregarArista(2, 4, 6);
    grafo.agregarArista(2, 5, 7);
    grafo.agregarArista(3, 4, 8);
    grafo.agregarArista(3, 5, 9);
    grafo.agregarArista(4, 5, 10);
    */

    // Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.

    auto start = chrono::steady_clock::now();
    if (algoritmo == "H1"){
        printf("Ejecutando Heuristica de Vecino mas cercano:\n");
        resultado = largo_heuristica_vecinoMasCercano(grafo,camino); 
    }
    else if (algoritmo == "H2"){
        printf("Ejecutando Heuristica de Arista mas corta:\n");   
        resultado = heuristica_aristaMasCorta(grafo,camino);
    }
    else if (algoritmo == "H3"){
       printf("Ejecutando Heuristica Arbol generador minimo:\n");
       resultado = heuristicaArbolgeneradorMinimo(grafo,camino);
        
    }
    else if (algoritmo == "MH1"){
        printf("Ejecutando Meta Heuristica de Memoria basada en soluciones exploradas:\n");    
        vector<int> res;
        res = tabuSearch(
            grafo,
            heuristica_vecinoMasCercano,
            params_metaheuristicas["tabuMemorySize"],
            params_metaheuristicas["cantIteraciones"],
            params_metaheuristicas["porcentajeRandomizacion"],
            params_metaheuristicas["iteracionesHeuristicas"]);

        resultado = grafo.calcularPesoCamino(res);
    }
    else if (algoritmo == "MH2"){

        vector<int> caminoInicial;
        float resPreMeta = largo_heuristica_vecinoMasCercano(grafo,caminoInicial);
        printf("Ejecutando Meta Heuristica de Memoria basada en estructura:\n");
        printf("Solucion antes de Meta Heuristica:%f\n", resPreMeta);
        // LongTabu << cantVecinos
        resultado = metaheuristica_basado_en_estructura(
            grafo,
            caminoInicial,
            params_metaheuristicas["tabuMemorySize"],
            params_metaheuristicas["porcentajeRandomizacion"], 
            params_metaheuristicas["cantIteraciones"]);
    }

    auto end = chrono::steady_clock::now();
    double total_time = chrono::duration<double, milli>(end - start).count();

    // Imprimimos el resultado por stdout.
    cout << "Resultado de la ejecucion: " << resultado << endl;

    cout << "Resultado de la ejecucion + " << "Tiempo de ejecucion: " << endl;
    // Imprimimos el tiempo de ejecución por stderr.
    clog << resultado <<" " << total_time << endl;
    
    return 0;
}
