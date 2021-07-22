#include "hvmc.h"
#include <climits>

const float MAX_DIST = FLT_MAX;


// Devuelve true y guardo el camino en el vector camino.
bool heuristica_vecinoMasCercano(Grafo &G, vector<int> &camino){

	vector<bool> visitado(G.cantNodos(), false);
	int verticeActual = 1;
	float MinimaDistanciaActual = MAX_DIST;
	int vecino_mas_cercano=verticeActual;
	
	// Es necesario y suficiente que salte n veces de nodo.
	camino.push_back(1);
	printf("largo de G: %i\n",G.cantNodos());
	while(camino.size() < G.cantNodos()) // O(|V|)
	{
		MinimaDistanciaActual = MAX_DIST;
		
		// Como uso verticeActual para elegir el primer eje, debo tener al dia el vec de visitados, sino error
		visitado[verticeActual] = true;

		for(int i = 1;i <= G.cantNodos();i++){
			if(i != verticeActual){//
				if(!visitado[i]){ // Elijo un eje por el cual no hayamos pasado, sino rompemos el algoritmo
					if(G.pesoArista(verticeActual,i) < MinimaDistanciaActual){
						MinimaDistanciaActual = G.pesoArista(verticeActual,i);
						vecino_mas_cercano=i;
					}
				}
			}
		}
		visitado[vecino_mas_cercano]=true;
		camino.push_back(vecino_mas_cercano);
		verticeActual=vecino_mas_cercano;
	}

	camino.push_back(camino[0]);
	return true;
}

float largo_heuristica_vecinoMasCercano(Grafo &G, vector<int> &circuito){
	bool hayCamino = heuristica_vecinoMasCercano(G, circuito);
	return G.calcularPesoCamino(circuito);
}