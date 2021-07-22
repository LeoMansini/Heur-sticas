//
//

#include "mbe.h"
#include <string>
#include <cstdlib>



class listaTabu{ // La lista tabu que va a memorizar las ultimas |T| aristas removidas
    public:
        listaTabu(int longTabu){ // O(T)
            lista = vector<arista>(longTabu);
            elementos = 0;
        }

    // Si superé la cantidad de aristas a memorizar,
    // entonces empiezo a pisar la lista por el comienzo
        void agregarArista(arista a){ // O(1)
            int indice = elementos % (lista.size());
            lista[indice] = a;
            elementos++;
        }

        bool esTabu(arista a){ // O(T)
            int longitud = (elementos < lista.size()) ? elementos : lista.size();
            for(int i = 0; i < longitud; i++){
                if(lista[i] == a){ // Operador == se fija si es la misma arista o la invertida.
                    return true;
                }
            }
            return false;
        }

        vector<arista> elems(){ // O(T)
            return lista;
        }
    private:
    // Vector de par de aristas, representan reemplazar la primera arista por la segunda.
        vector<arista> lista;
        int elementos;
};

// O(1)
float crearVecino(Grafo &G, vector<int> &camino, int &n1, int &n2,
                arista &a1, arista &a2, vector<arista> &aristasQuitadas){ // Devuelve difPeso.
    n1 = 0;
    n2 = 0;
    while(n1 == n2 || n1 == 0 || n2 == 0){ // no quiero que sean iguales ni que alguna sea 0
        n1 = std::rand() % camino.size();
        n2 = std::rand() % camino.size();
    }

    if(n2 < n1){
        int n3 = n2;
        n2 = n1;
        n1 = n3;
    } // n1 < n2

    // cambio las aristas indiceAnteriorN1 -> N1 y N2 -> indiceSiguienteN2 por
    //                    indiceAnteriorN1 -> N2 y N1 -> indiceSiguienteN2
    int indiceAnteriorN1 = n1 - 1;

    int indiceSiguienteN2;
    if(n2 == (camino.size() - 1)){
        indiceSiguienteN2 = 0;
    }
    else{
        indiceSiguienteN2 = n2 + 1;
    }

    // armo aristas que seran removidas
    arista r1;
    arista r2;
    r1.vertices = make_tuple(camino[n1], camino[indiceAnteriorN1]); // indiceAnteriorN1 -> N1
    r1.peso = G.pesoArista(camino[n1], camino[indiceAnteriorN1]);
    r2.vertices = make_tuple(camino[n2], camino[indiceSiguienteN2]); // N2 -> indiceSiguienteN2
    r2.peso = G.pesoArista(camino[n2], camino[indiceSiguienteN2]);


    // armo aristas que seran agregadas
    a1.vertices = make_tuple(camino[indiceAnteriorN1], camino[n2]); // indiceAnteriorN1 -> N2
    a1.peso = G.pesoArista(camino[indiceAnteriorN1], camino[n2]);
    a2.vertices = make_tuple(camino[n1], camino[indiceSiguienteN2]); // N1 -> indiceSiguienteN2
    a2.peso = G.pesoArista(camino[n1], camino[indiceSiguienteN2]);


    // me fijo que no este removiendo y agregando la misma arista
    // si es el caso, en realidad no la removi, entonces no la agrego a listaTabu

    if(a1 != r1 && a2 != r1){
        aristasQuitadas.push_back(r1);
    }
    if(a1 != r2 && a2 != r2){
        aristasQuitadas.push_back(r2);
    }

    float diferenciaPeso = a1.peso + a2.peso - r1.peso - r2.peso;

    return diferenciaPeso;
}

// O(n^2 * porcentaje randomizacion * T)
tuple<vector<int>, float> hallarVecino(Grafo &G, vector<int> &camino, int porcentajeRandomizacionVecindario,
                                     float pesoActual, float mejorPeso, listaTabu &T){
    vector<int> mejorCaminoVecino = {}; // camino vecino de menor longitud
    float mejorDiferenciaPeso = 0; // diferencia de peso con el camino original
    vector<arista> aristasQuitadasFinales; // para agregar a la lista tabu al final
    bool soloHayTabu = true; // A priori asumo que no hay ningun no tabu
    pair<int, int> parDeNodosSwapeados = {0, 0}; // Indices de nodos swapeados en el vecino
    int longVecinos = (G.cantNodos()*(G.cantNodos()-1)/2)*porcentajeRandomizacionVecindario/100;
    int maxIter = longVecinos*2; // Si me paso de este numero, devuelvo el menor vecino, sea tabu o no.

    camino.pop_back(); // simplifica unir nodos, despues se revierte
    int k = 0;
    while(longVecinos > 0 && maxIter > 0) {
        k++; // Para debug
        int n1 = 0;
        int n2 = 0;
        arista a1;
        arista a2; // Aristas que se van a agregar en el vecino creado.
        vector<arista> aristasQuitadas;
        float diferenciaPeso = crearVecino(G, camino, n1, n2, a1, a2, aristasQuitadas);
        //                     Lista tabu                           funcion de aspiracion
        bool esTabu = (T.esTabu(a1) || T.esTabu(a2)) && (pesoActual + diferenciaPeso >= mejorPeso);
        if (!(esTabu)) {
            longVecinos--;
            if (soloHayTabu) { // Primera vez que encuentro un vecino no tabu
                parDeNodosSwapeados = {0, 0}; // Borro los nodos swapeados si guarde algun vecino tabu
                mejorDiferenciaPeso = 0;
                soloHayTabu = false; // De ahora en más guardo solo los no tabú.
            }
        }
        // Voy guardando el menor vecino aunque no sea tabu. Desde que encuentro un no tabú solo guardo esos.
        if(!esTabu || soloHayTabu){
            if ((parDeNodosSwapeados == make_pair(0, 0)) || (diferenciaPeso < mejorDiferenciaPeso)) {
                // si ya tengo un camino guardado, lo reemplazo si el ultimo obtenido es mejor
                parDeNodosSwapeados = make_pair(n1, n2);
                mejorDiferenciaPeso = diferenciaPeso;
                aristasQuitadasFinales = aristasQuitadas;
            }
        }
        maxIter--;
    }

    for(int i = 0; i < aristasQuitadasFinales.size(); i++) {
        T.agregarArista(aristasQuitadasFinales[i]);
    }
    // Construyo mejor vecino.
    int n1 = get<0>(parDeNodosSwapeados);
    int n2 = get<1>(parDeNodosSwapeados);
    for (int i = 0; i < n1; i++) {
        mejorCaminoVecino.push_back(camino[i]);
    }
    for (int i = n2; i >= n1; i--) {
        mejorCaminoVecino.push_back(camino[i]);
    }
    for (int i = n2 + 1; i < camino.size(); i++) {
        mejorCaminoVecino.push_back(camino[i]);
    }
    mejorCaminoVecino.push_back(mejorCaminoVecino[0]); // Agrego el primero al final asi es un ciclo.

    camino.push_back(camino[0]); // Agrego el primero al final asi es un ciclo.
    return make_tuple(mejorCaminoVecino, mejorDiferenciaPeso);
}

// O(maxIter * T * porcentajeRandomizacion * n^2)
float metaheuristica_basado_en_estructura(Grafo &G, vector<int> camino, int longTabu,
                                        int porcentajeRandomizacionVecindario, int maxIter){
    listaTabu T(longTabu); // O(T)
    //vector<int> mejorCamino = camino;// no parece necesario
    float mejorPeso = G.calcularPesoCamino(camino); // O(n)
    float pesoActual = mejorPeso;

    srand(time(0)); // Semilla para generacion de vecinos random.

    while(maxIter > 0){ // se puede poner otro criterio de parada
        tuple<vector<int>, float> mejorVecino = hallarVecino(G, camino,
                                                           porcentajeRandomizacionVecindario, pesoActual,
                                                           mejorPeso, T);
        float pesoVecino = pesoActual + get<1>(mejorVecino);
        if (pesoVecino < mejorPeso) {
            mejorPeso = pesoVecino;
        }
        camino = get<0>(mejorVecino);
        pesoActual = pesoVecino;

        maxIter--;
    }
    return mejorPeso;
}