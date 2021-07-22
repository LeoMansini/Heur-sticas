//
//

#ifndef ALGO3_TP2_MBE_H
#define ALGO3_TP2_MBE_H

class listaTabu; // funciona esto?
tuple<vector<int>, float> hallarVecino(Grafo &G, vector<int> &camino, int longVecinos,
                                     float pesoActual, float mejorPeso, listaTabu &T);
float metaheuristica_basado_en_estructura(Grafo &G, vector<int> camino, int longTabu, int longVecinos, int maxIter);

#endif //ALGO3_TP2_MBE_H
