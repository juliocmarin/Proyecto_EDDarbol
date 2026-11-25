#include <iostream>
#include "editor.h"
#include "entrada.h"
using namespace std;

void mostrarDatos(Nodo* m) {
    cout << endl;
    imprimirNombre(m);
    cout << endl;
    cout << "  1. Nombre      : " << m->nombre << endl;
    cout << "  2. Apellido    : " << m->apellido << endl;
    cout << "  3. Genero      : " << m->genero << endl;
    cout << "  4. Edad        : " << m->edad << endl;
    cout << "  5. Esta muerto : " << m->muerto << endl;
    cout << "  6. Esta preso  : " << m->preso << endl;
    cout << "  7. Fue jefe    : " << m->fueJefe << endl;
    cout << "  8. Es jefe     : " << m->esJefe << endl;
    cout << "  0. Terminar" << endl;
}


void volverUnicoJefe(Arbol& arbol, Nodo* nuevo) {
    Nodo* actual = buscarJefeActual(arbol);
    if (actual != NULL && actual != nuevo) {
        actual->esJefe = false;
        actual->fueJefe = true;
    }
    nuevo->esJefe = true;
}

bool editarMiembro(Arbol& arbol) {
    int id = leerEntero("ID del miembro a editar (0 para cancelar): ", 0, 999999999);
    if (id == 0) return false;

    Nodo* m = buscarPorId(arbol, id);
    if (m == NULL) {
        cout << "No existe un miembro con id " << id << "." << endl;
        return false;
    }

    bool huboCambios = false;
    int opcion;
    do {
        mostrarDatos(m);
        opcion = leerEntero("Campo a modificar: ", 0, 8);

        if (opcion == 1) {
            leerTexto("Nuevo nombre: ", m->nombre, 40);
            huboCambios = true;
        } else if (opcion == 2) {
            leerTexto("Nuevo apellido: ", m->apellido, 40);
            huboCambios = true;
        } else if (opcion == 3) {
            m->genero = leerGenero("Nuevo genero (H/M): ");
            huboCambios = true;
        } else if (opcion == 4) {
            m->edad = leerEntero("Nueva edad (0-120): ", 0, 120);
            huboCambios = true;
        } else if (opcion == 5) {
            m->muerto = leerBandera("Esta muerto (0/1): ");
            huboCambios = true;
        } else if (opcion == 6) {
            m->preso = leerBandera("Esta preso (0/1): ");
            huboCambios = true;
        } else if (opcion == 7) {
            m->fueJefe = leerBandera("Fue jefe (0/1): ");
            huboCambios = true;
        } else if (opcion == 8) {
            if (leerBandera("Es jefe (0/1): ")) volverUnicoJefe(arbol, m);
            else m->esJefe = false;
            huboCambios = true;
        }
    } while (opcion != 0);

    return huboCambios;
}

