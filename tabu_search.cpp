#include "tabu_search.h"
#include <functional>
#include <algorithm>

vector<vector<int>> obtenerVecindario(Grafo &graph, vector<int> circuito, vector<vector<int>> tabuMemory, int porcentajeRandomizacionVecindario){
	vector<vector<int>> vecinos;
	vector<int> vecinoCandidato;
	int i;
	int j;
	int cantidadAgregar = (circuito.size()*(circuito.size()-1)/2)*porcentajeRandomizacionVecindario/100;


	while((int) vecinos.size()< (int) cantidadAgregar){
		i=rand() % graph.cantNodos() + 0;
		j=rand() % graph.cantNodos() + 0;
		vecinoCandidato = armarCicloCandidato(circuito, i, j);
		if (!esta(vecinoCandidato, tabuMemory)){
			 vecinos.push_back(vecinoCandidato);
		}
	}
	return vecinos;
}

bool esta(vector<int> vecino, vector<vector<int>> memoria){
	bool res = false;
	for (int i = 0; i < memoria.size(); i++){
		if (vecino == memoria[i]){
			res = true;
			break;
		}
	}
	return res;
}

vector<int> obtenerMejorCandidato(Grafo &graph, vector<vector<int>> vecinos, vector<vector<int>> memoria){
	vector<int> res = vecinos[0];
	for(int i = 0; i < vecinos.size(); i++){
		if ((graph.calcularPesoCamino(res) > graph.calcularPesoCamino(vecinos[i]))){
			res = vecinos[i];		
		}
	}
	return res;
}


vector<int> tabuSearch(Grafo &graph,
	function<bool(Grafo &graph, vector<int> &circuito)> algorithmFunction,
	 int tabuMemorySize,
	 int cantIteraciones,
	 int porcentajeRandomizacionVecindario,
	 bool iteracionesHeuristicas){

	vector<int> solucionFactible;
	vector<int> solucionCandidata;
	vector<vector<int>> tabuMemory(tabuMemorySize);
	vector<vector<int>> vecindario;

	bool noUsar = algorithmFunction(graph, solucionFactible);
	vector<int> mejorSolucion = solucionFactible;
	bool solucionMejoro;
	int indiceMemoria = 0;
	int i = 0;

	srand(time(0));

	while(i < cantIteraciones){
		solucionMejoro = false;
		vecindario = obtenerVecindario(graph, solucionFactible, tabuMemory, porcentajeRandomizacionVecindario);
		solucionCandidata = obtenerMejorCandidato(graph, vecindario, tabuMemory);
		if (tabuMemory.size() == tabuMemorySize){
			indiceMemoria = 0;
		}
		tabuMemory[indiceMemoria] = solucionCandidata;
		indiceMemoria++;
		if (graph.calcularPesoCamino(solucionFactible) > graph.calcularPesoCamino(solucionCandidata)){
			if (graph.calcularPesoCamino(mejorSolucion) > graph.calcularPesoCamino(solucionCandidata))
				mejorSolucion = solucionCandidata;
			solucionMejoro = true;
			printf("Solucion mejorada: %f\n",graph.calcularPesoCamino(solucionCandidata));
		}
		solucionFactible = solucionCandidata;
		if (!iteracionesHeuristicas) i++;
		if (iteracionesHeuristicas && !solucionMejoro) i++;
	}
	return mejorSolucion;
}	

//En principio podriamos ni necesitar esta funcion, a menos que nos interese comparar experimentalmente
//La diferencia de complejidad entre busqueda local comun y tabu search.(Puede servirnos)
vector<int> localSearch_2_Opt(Grafo &graph, 
	function<vector<int>(Grafo &graph, vector<int> &circuito)> algorithmFunction){

	vector<int> solucionFactible; 
	vector<int> solucionCandidata;

	solucionFactible = algorithmFunction(graph, solucionFactible);

	bool solucionMejoro = true;

	while(solucionMejoro){
		solucionMejoro = false;
		for(int i = 0; i < solucionFactible.size()-1; i++){
			for(int j = 0; j < solucionFactible.size()-1; j++){
				solucionCandidata = armarCicloCandidato(solucionFactible, i, j);
				if(graph.calcularPesoCamino(solucionFactible) > graph.calcularPesoCamino(solucionCandidata)){
					solucionFactible = solucionCandidata;
					solucionMejoro = true;
					printf("Solucion mejorada: %f\n",graph.calcularPesoCamino(solucionCandidata));
				}
			}
		}
	}
	return solucionFactible;
}

void procesarSolucionCandidata(vector<int> &solCandidata, vector<int> solFactible, int i, int j){
	for (int k = 0; k < i; k++){
		solCandidata.push_back(solFactible[k]);
	}
	for (int k = j; k >= i; k--){
		solCandidata.push_back(solFactible[k]);
	}
	for(int k = j+1; k < solFactible.size(); k++){
		solCandidata.push_back(solFactible[k]);
	}
}

vector<int> armarCicloCandidato(vector<int> solucionFactible, int i, int j){
	if (i == j){
		return solucionFactible;
	}
	if (i == 0 || j == solucionFactible.size() || j == 0 || j == solucionFactible.size()){
		return solucionFactible;
	}
	vector<int> solucionCandidata;
	bool dandoVuelta = false;

	if (i < j){
		procesarSolucionCandidata(solucionCandidata, solucionFactible, i, j);
	}
	else{
		procesarSolucionCandidata(solucionCandidata, solucionFactible, j, i);
	}

	return solucionCandidata;
}