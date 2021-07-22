//#include "grafo.h"

float heuristicaArbolgeneradorMinimo(Grafo &G, vector<int> &circuito){
    Grafo agm = G.obtenerAGM();
    int nodoRaiz = 1;
    circuito = agm.nodosEnOrdenDFS(nodoRaiz);
    circuito.push_back(nodoRaiz);
    return G.calcularPesoCamino(circuito);
}