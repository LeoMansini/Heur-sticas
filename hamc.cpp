#include "hamc.h"
#include <list>


bool hay_nodo_grado3(vector<int> &grados){
	bool res=false;
	for(int i=0; i<grados.size(); i++){
		if(grados[i]>2){
			res=true;
			return res;
		}
	}
	return res;
}

vector<arista> completarCircuito(list<arista> aristas, Grafo& G){
	vector<int> grados_de_nodos(G.cantNodos()+1,0); 
	vector<arista> camino;
	vector<int> nodosConectar;
	arista aristaFaltante;

	for (auto it = aristas.begin(); it != aristas.end(); ++it) {
				grados_de_nodos[(*it).origen()]++;
				grados_de_nodos[(*it).destino()]++;
				camino.push_back(*it);
	}
	for (int i = 0; i <grados_de_nodos.size() ; ++i){
		if(grados_de_nodos[i]==1){
			nodosConectar.push_back(i);		
		}
	}
    aristaFaltante.vertices = make_tuple(nodosConectar[0], nodosConectar[1]);
    aristaFaltante.peso = G.pesoArista(nodosConectar[0],nodosConectar[1]);
	camino.push_back(aristaFaltante);
    return camino;
}

bool agregoSiNoHayCiclo(arista aristaActual, vector<list<arista>> &aristas_conexas){
	vector<int> indices;
	int cantApariciones;
	for(int i=0; i<aristas_conexas.size();i++){
		cantApariciones=0;
		for(auto j= aristas_conexas[i].begin() ; j != aristas_conexas[i].end(); j++){
			if( (*j).origen()==aristaActual.origen() ||
				(*j).origen()==aristaActual.destino() ||
				(*j).destino()==aristaActual.origen() ||
				(*j).destino()==aristaActual.destino()){
				cantApariciones++;
				indices.push_back(i);
			}
		}
		if(cantApariciones>1){
			return false;
		}
	}
	if(indices.size()>2){
		return false;
	}
	if(indices.size()==2){
		aristas_conexas[indices[0]].push_back(aristaActual);
		aristas_conexas[indices[0]].splice(aristas_conexas[indices[0]].end(), aristas_conexas[indices[1]]);
		aristas_conexas.erase(aristas_conexas.begin()+indices[1]);
	}
	if(indices.size()==1){
		aristas_conexas[indices[0]].push_back(aristaActual);
	}else{
		list<arista> lista_nueva;
		lista_nueva.push_back(aristaActual);
		aristas_conexas.push_back(lista_nueva);
	}
	return true;
}

bool verticeYaAgrgado(vector<arista> &aristas_camino, int v){
	for (int i = 0; i < aristas_camino.size(); ++i){
		if(aristas_camino[i].origen()==v || aristas_camino[i].destino()==v);{
			return true;
		}
	}
	return false;
}

bool verticeVicitado(vector<int> &res, int vertice){
	for (int i = 0; i < res.size(); ++i){
		if (vertice==res[i]){
			return true;
		}
	}
	return false;
}


vector<int> crearCamino(vector<arista> &aristas_camino,int n){
	vector<int> res;
	int i =0;
	res.push_back(1);
	int ultima_pos=0;
	int h;
	int cantIteraciones=0;
	while(res.size()<n && cantIteraciones<=aristas_camino.size()){
		for (i = 0; i < aristas_camino.size(); ++i){
			if(aristas_camino[i].origen()==res[ultima_pos]){
				res.push_back(aristas_camino[i].destino());
				ultima_pos++;
			}
			if((aristas_camino[i].destino()==res[ultima_pos] && !verticeVicitado(res,aristas_camino[i].origen()))){
				res.push_back(aristas_camino[i].origen());
				ultima_pos++;
			}
		}
		cantIteraciones++;
	}
	res.push_back(res[0]);
	return res;
}


// Devuelve true y guardo el camino en el vector camino.
bool largo_heuristica_aristaMasCorta(Grafo &G, vector<int> &camino){
	vector<arista> aristas_de_G = G.aristasOrdenadas();
	vector<int> grados_de_nodos = vector<int>(G.cantNodos()+1, 0); // +1 para que pueda indexar con el nodo directamente (no existe el nodo 0) 
	int i=0;
	vector<list<arista>> aristas_conexas;
	vector<arista> aristas_camino;
	int aristas_agregadas=0;
	while(aristas_agregadas<G.cantNodos()-1 && i<aristas_de_G.size()){
		if(agregoSiNoHayCiclo(aristas_de_G[i],aristas_conexas)){
			aristas_agregadas++;
		}
		i++;
	}
	list<arista> camino_aristas=aristas_conexas[0];
	printf("largo del camino %i\n",(int) camino_aristas.size());
	vector<arista> vectorAristas = completarCircuito(camino_aristas, G);
	camino = crearCamino(vectorAristas,G.cantNodos());
	return true;
}



int heuristica_aristaMasCorta(Grafo &G, vector<int> &circuito){
	bool hayCamino = largo_heuristica_aristaMasCorta(G, circuito);
    return G.calcularPesoCamino(circuito);
}