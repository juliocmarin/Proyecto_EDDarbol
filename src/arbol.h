#ifndef ARBOL_H
#define ARBOL_H

#include "nodo.h"

struct Arbol {
    Nodo* raiz;
    int cantidad;
};

Nodo* crearNodo();
void iniciarArbol(Arbol& arbol);
void liberarArbol(Arbol& arbol);

bool insertar(Arbol& arbol, Nodo* nuevo);
Nodo* buscarPorId(Arbol& arbol, int id);
Nodo* buscarJefeActual(Arbol& arbol);
int contarJefes(Arbol& arbol);
void mostrarArbol(Arbol& arbol);

// imprime id con el nombre y apeliido"
void imprimirNombre(Nodo* n);


Nodo* hermano(Nodo* n);

// cantidad de saltos que hay entre dos nodos del arbol
int distancia(Nodo* a, Nodo* b);

#endif
