#include <functional>

#ifndef TABU_SEARCH
#define TABU_SEARCH

vector<int> tabuSearch(Grafo &graph,function<vector<int>(Grafo &graph, vector<int> &circuito)> algorithmFunction, int tabuMemorySize, int cantIteraciones);
vector<int> localSearch_2_Opt(Grafo &graph,function<vector<int>(Grafo &graph, vector<int> &circuito)> algorithmFunction);
vector<int> armarCicloCandidato(vector<int> solucionFactible, int i, int j);
void procesarSolucionCandidata(vector<int> &solCandidata, vector<int> solFactible, int i, int j);

vector<vector<int>> obtenerVecindario(Grafo &graph, vector<int> circuito, vector<vector<int>> tabuMemory, int porcentajeRandomizacionVecindario);
vector<int> obtenerMejorCandidato(Grafo &graph, vector<vector<int>> vecinos, vector<int> memoria);
bool esta(vector<int> vecino, vector<vector<int>> memoria);
bool aspirationFunction(Grafo &graph, vector<int> vecino, vector<vector<int>> memoria);

#endif