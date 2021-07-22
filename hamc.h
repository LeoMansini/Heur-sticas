#ifndef HVMC
#define HVMC


int largo_heuristica_aristaMasCorta(Grafo &G, vector<int> &circuito);
bool heuristica_aristaMasCorta(Grafo &G, vector<int> &camino);
bool hay_nodo_grado3(vector<int> &grados);
bool esCiclo(vector<arista> &aristas_camino);
vector<int> crearCamino(vector<arista> &aristas_camino,int nodoInicial,int nodoFinal,int cantNodos);

#endif
