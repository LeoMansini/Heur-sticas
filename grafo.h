#include <string>
#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <cfloat>


using namespace std;


  struct arista
  {
    tuple<int,int> vertices;
    float peso;
    int origen(){
        return get<0>(vertices);
    }
    int destino(){
        return get<1>(vertices);
    }
    bool operator==(arista& a2) {
        if (((origen() == a2.origen() && destino() == a2.destino()) ||
            (destino() == a2.origen() && origen() == a2.destino())) && a2.peso == peso) {
            return true;
        }
        return false;
    }
    bool operator!=(arista& a2){
        return !(*this == a2);
    }
    void print(){
        printf("%d -> %d", origen(), destino());
    }
  };

class Grafo {
public:

  // Constructor
  Grafo(int cant_vertices);

  // Constructor por copia
  Grafo(const Grafo& original);

    //Agregar una arista
  void agregarArista(int i, int j, float peso);

  //agrega una arista al grafo
  void agregarArista(arista arista);

  //devuelve true si el nodo i es adyacente al nodo j
  bool esAdvancente(int i, int j);


  // Devuelve la cantidad de nodos
  int cantNodos();

  //Devuelve el peso de la arista (i,j)
  float pesoArista(int i, int j);

  //Devuelve el peso de un camino
  float calcularPesoCamino(vector<int>& camino);

  //Devuelve el peso de un circuito
  float calcularPesoCircuito(vector<int>& circuito);

  int dameUnVecinoConListaNegra(int nodo, set<int> listaNegra);

  //devuelve el conjutno de nodos
  set<int> nodos();

  //imprime la estructura del grafo
  void print();

  //Devuelve el vector de aristas ordenado
  vector<arista> aristasOrdenadas();

  //Devuelve un érbol generador mínimo de G
  Grafo obtenerAGM();

  //Devuelve los vértices ordenados por orden de visita usando DFS desde nodoRaiz
  vector<int> nodosEnOrdenDFS(int nodoRaiz);

private:
  const float bottom = FLT_MAX/2;
  set<int> _vertices;
  vector<arista> _aristas;
  vector<vector<float>> _matriz_adyacencias;
  int _cantNodos;
};



