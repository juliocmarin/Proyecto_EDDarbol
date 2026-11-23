#include <iostream>
#include "jefe_control.h"
#include "reglas_muerte.h"
#include "reglas_extendidas.h"
using namespace std;

Nodo* elegirNuevoJefe(Arbol& arbol, Nodo* base) {
    Nodo* elegido = sucesorBasico(base);                                  // reglas 1 a 4
    if (elegido == NULL) elegido = ancestroConDosSucesores(base);         // regla 5
    if (elegido == NULL) elegido = buscarEntrePresos(arbol, base);        // regla 6
    if (elegido == NULL) elegido = libreMasCercano(arbol, base);          // ultimo recurso
    return elegido;
}

void traspasarJefatura(Nodo* viejo, Nodo* nuevo) {
    if (viejo != NULL) {
        viejo->esJefe = false;
        viejo->fueJefe = true;
    }
    nuevo->esJefe = true;
}

void revisarJefe(Arbol& arbol) {
    
    int vueltas = arbol.cantidad + 2;

    while (vueltas > 0) {
        vueltas--;
        Nodo* jefe = buscarJefeActual(arbol);

        if (jefe == NULL) {
            Nodo* nuevo = primerLibre(arbol.raiz, true);
            if (nuevo == NULL) {
                cout << "No hay ningun miembro vivo y libre para ser jefe." << endl;
                return;
            }
            traspasarJefatura(NULL, nuevo);
            cout << "No habia jefe. Nuevo jefe: ";
            imprimirNombre(nuevo);
            cout << endl;
            continue;
        }

        if (jefe->muerto) {
            cout << "El jefe ";
            imprimirNombre(jefe);
            cout << " murio. Buscando sucesor..." << endl;

            Nodo* nuevo = elegirNuevoJefe(arbol, jefe);
            if (nuevo == NULL) {
                jefe->esJefe = false;
                jefe->fueJefe = true;
                cout << "No se encontro ningun sucesor vivo y libre. La familia queda sin jefe." << endl;
                return;
            }
            traspasarJefatura(jefe, nuevo);
            cout << "Nuevo jefe: ";
            imprimirNombre(nuevo);
            cout << endl;
            continue;
        }

        if (jefe->edad > 70 || jefe->preso) {
            cout << "El jefe ";
            imprimirNombre(jefe);
            if (jefe->preso) cout << " esta en prision." << endl;
            else cout << " paso de los 70 anios." << endl;

            Nodo* nuevo = primerLibre(jefe, false);
            if (nuevo == NULL) {
                cout << "No tiene sucesores libres en su arbol, se mantiene en el puesto." << endl;
                return;
            }
            traspasarJefatura(jefe, nuevo);
            cout << "Nuevo jefe: ";
            imprimirNombre(nuevo);
            cout << endl;
            continue;
        }

        return;   // el jefe esta en regla rn
    }
}
