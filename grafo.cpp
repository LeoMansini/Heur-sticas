#include "grafo.h"
#include <iostream>
#include <sstream>


using namespace std;

Grafo::Grafo(int cant_vertices){
    _cantNodos = cant_vertices;
    _matriz_adyacencias = vector<vector<float>>(cant_vertices+1, vector<float>(cant_vertices+1, 0)); // nunca accedamos a la matriz en las pociciones [0][*] o [*][0]
    _matriz_adyacencias[0][0] = bottom;
    for(int i=1; i< cant_vertices+1; i++){
        _vertices.insert(i);
        _matriz_adyacencias[0][i] = bottom;
        _matriz_adyacencias[i][0] = bottom;
        for(int j=1; j< cant_vertices+1; j++){
            _matriz_adyacencias[i][j] = bottom;
            _matriz_adyacencias[j][i] = bottom;
        }
    }
}

Grafo::Grafo(const Grafo& original){
    _matriz_adyacencias = original._matriz_adyacencias;
    _aristas = original._aristas;
    _vertices = original._vertices;
    _cantNodos = original._cantNodos;
}

int Grafo::cantNodos(){
    return _cantNodos;
}

float Grafo::pesoArista(int i, int j){
    return _matriz_adyacencias[i][j];
}

void Grafo::agregarArista(int i, int j, float peso) {
    if (_matriz_adyacencias[i][j] == bottom){
        _matriz_adyacencias[i][j] = peso;
        _matriz_adyacencias[j][i] = peso;
        arista _arista;
        _arista.vertices = make_tuple(i, j);
        _arista.peso = peso;
        _aristas.push_back(_arista);
    }
}

void Grafo::agregarArista(arista arista){
    agregarArista(arista.origen(), arista.destino(), arista.peso);
}

bool Grafo::esAdvancente(int i, int j){
    return _matriz_adyacencias[i][j] != bottom;
}

set<int> Grafo::nodos(){
    return _vertices;
}

int Grafo::dameUnVecinoConListaNegra(int nodo, set<int> listaNegra){
    int vecino = 0;
    int i = 0;
    while(vecino == 0 && i < _matriz_adyacencias[nodo].size()){
        if(_matriz_adyacencias[nodo][i] != bottom && listaNegra.count(i) == 0) vecino = i;
        else i++;
    }
    return vecino;
}
vector<arista> Grafo::aristasOrdenadas(){//n^4
	vector<arista> ordenado = _aristas;

	float min_actual = ordenado[0].peso;
	float min_actual_i;
	arista aux;

	for(int desde=0; desde<_aristas.size(); desde++){ 
		
		min_actual=ordenado[desde].peso;

		for(int i=desde; i< _aristas.size(); i++){
			if ( ordenado[i].peso<= min_actual ){
				min_actual = ordenado[i].peso;
				min_actual_i = i;
			}
		} 
		
		aux=ordenado[desde];
		ordenado[desde]= ordenado[min_actual_i];
		ordenado[min_actual_i]=aux;
	}
	return ordenado;
}

float Grafo::calcularPesoCamino(vector<int>& camino){
    float peso=0;
    if(camino.size()>1) {
        for (int i = 0; i < camino.size() - 1; i++) {
            peso = peso + pesoArista(camino[i], camino[i + 1]);
        }
    }
    return peso;
}

float Grafo::calcularPesoCircuito(vector<int>& circuito){
    float peso=0;
    if(circuito.size()>1) {
        for (int i = 0; i < circuito.size() - 1; i++) {
            peso = peso + pesoArista(circuito[i], circuito[i + 1]);
        }
        peso = peso + pesoArista(circuito[0], circuito[circuito.size()-1]);
    }
    return peso;
}


void Grafo::print(){
   for (int i = 1; i < _matriz_adyacencias.size(); i++) {
       for (int j = 1; j < _matriz_adyacencias[i].size(); j++) {
           float valor = _matriz_adyacencias[i][j];
           if(valor == bottom) std::cout << "N" << ' ';
           else std::cout << _matriz_adyacencias[i][j] << ' ';
       }
       std::cout <<"\n";
   }
}

Grafo Grafo::obtenerAGM(){

    vector<arista> aristasOrdenadasPorPeso = aristasOrdenadas();//O(n4)
    Grafo agm(cantNodos());//O(n2)
    set<int> yaAgregados;
    //construyo un AGM
    if(aristasOrdenadasPorPeso.size() > 0){
        agm.agregarArista(aristasOrdenadasPorPeso[0]);
        yaAgregados.insert(aristasOrdenadasPorPeso[0].origen());
        yaAgregados.insert(aristasOrdenadasPorPeso[0].destino());
    }
    int i=1;
    bool salteoEjesSueltos = false;
    while(yaAgregados.size() < cantNodos() && i != aristasOrdenadasPorPeso.size()){//O(n4)
        bool yaEstaOrigen = yaAgregados.count(aristasOrdenadasPorPeso[i].origen()) != 0;//O(n)
        bool yaEstaDestino = yaAgregados.count(aristasOrdenadasPorPeso[i].destino()) != 0;//O(n)
        if(yaEstaOrigen != yaEstaDestino){
            //uno esta agregado y otro no
            if(yaEstaOrigen) yaAgregados.insert(aristasOrdenadasPorPeso[i].destino());
            else yaAgregados.insert(aristasOrdenadasPorPeso[i].origen());
            agm.agregarArista(aristasOrdenadasPorPeso[i]);
            if(salteoEjesSueltos){
                i = 0;
                salteoEjesSueltos = false;
            }
        }else{
            if(!yaEstaOrigen) salteoEjesSueltos=true;
        }
        i++;
    }
    return agm;
}

vector<int> Grafo::nodosEnOrdenDFS(int nodoRaiz){
    vector<int> camino;
    set<int> yaAgregados;
    int n = nodoRaiz;
    vector<int> pilaDFS;
    pilaDFS.push_back(n);
    camino.push_back(n);
    yaAgregados.insert(n);
    //mientras quede algo por recorrer en dfs
    int vecino;
    while(pilaDFS.size() != 0) {
        vecino = dameUnVecinoConListaNegra(n, yaAgregados);
        if(vecino == 0){//no hay vecinos disponibles, desapilo
            pilaDFS.pop_back();
            n = pilaDFS.back();
        }
        else{//hay al menos un vecino disponible, apilo el vecino encontrado
            n = vecino;
            pilaDFS.push_back(n);
            camino.push_back(n);
            yaAgregados.insert(n);
        }
    }
    return camino;
}

