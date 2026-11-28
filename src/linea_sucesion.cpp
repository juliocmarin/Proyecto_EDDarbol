#include <iostream>
#include "linea_sucesion.h"
using namespace std;


void listarVivos(Nodo* n, int& posicion) {
    if (n == NULL) return;
    if (!n->muerto) {
        cout << "  " << posicion << ". ";
        imprimirNombre(n);
        cout << " (" << n->edad << " anios)";
        if (n->preso) cout << " [en prision]";
        cout << endl;
        posicion++;
    }
    listarVivos(n->izq, posicion);
    listarVivos(n->der, posicion);
}

void mostrarLineaSucesion(Arbol& arbol) {
    Nodo* jefe = buscarJefeActual(arbol);
    if (jefe == NULL) {
        cout << "No hay jefe actual, no se puede armar la linea de sucesion." << endl;
        return;
    }

    cout << endl << "Jefe actual: ";
    imprimirNombre(jefe);
    cout << " (" << jefe->edad << " anios)" << endl;
    cout << "Linea de sucesion (solo vivos):" << endl;

    int posicion = 1;

    
    listarVivos(jefe->izq, posicion);
    listarVivos(jefe->der, posicion);

    // despues, subiendo, el arbol del companero sucesor de cada ancestro
    Nodo* actual = jefe;
    while (actual->padre != NULL) {
        listarVivos(hermano(actual), posicion);
        actual = actual->padre;
    }

    if (posicion == 1) cout << "  (no hay sucesores vivos)" << endl;
}
