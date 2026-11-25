#include <iostream>
#include <cstring>
#include "arbol.h"
using namespace std;

Nodo* crearNodo() {
    Nodo* n = new Nodo;
    n->id = 0;
    strcpy(n->nombre, "");
    strcpy(n->apellido, "");
    n->genero = 'H';
    n->edad = 0;
    n->idJefe = 0;
    n->muerto = false;
    n->preso = false;
    n->fueJefe = false;
    n->esJefe = false;
    n->izq = NULL;
    n->der = NULL;
    n->padre = NULL;
    return n;
}

void iniciarArbol(Arbol& arbol) {
    arbol.raiz = NULL;
    arbol.cantidad = 0;
}

// esto borra primero los hijos y despues el nodo(recordatorio)
void borrarNodos(Nodo* n) {
    if (n == NULL) return;
    borrarNodos(n->izq);
    borrarNodos(n->der);
    delete n;
}

void liberarArbol(Arbol& arbol) {
    borrarNodos(arbol.raiz);
    arbol.raiz = NULL;
    arbol.cantidad = 0;
}

Nodo* buscarEnNodo(Nodo* n, int id) {
    if (n == NULL) return NULL;
    if (n->id == id) return n;
    Nodo* enIzq = buscarEnNodo(n->izq, id);
    if (enIzq != NULL) return enIzq;
    return buscarEnNodo(n->der, id);
}

Nodo* buscarPorId(Arbol& arbol, int id) {
    return buscarEnNodo(arbol.raiz, id);
}

bool insertar(Arbol& arbol, Nodo* nuevo) {
    if (nuevo == NULL) return false;
    if (buscarPorId(arbol, nuevo->id) != NULL) return false;   // id repetido

    // sin jefe, es la raiz, y solo puede haber una(recoordatorioo)
    if (nuevo->idJefe == 0) {
        if (arbol.raiz != NULL) return false;
        arbol.raiz = nuevo;
        nuevo->padre = NULL;
        arbol.cantidad++;
        return true;
    }

    Nodo* padre = buscarPorId(arbol, nuevo->idJefe);
    if (padre == NULL) return false;

    if (padre->izq == NULL) padre->izq = nuevo;
    else if (padre->der == NULL) padre->der = nuevo;
    else return false;   // ya tiene sus dos sucesores

    nuevo->padre = padre;
    arbol.cantidad++;
    return true;
}

Nodo* buscarJefeEnNodo(Nodo* n) {
    if (n == NULL) return NULL;
    if (n->esJefe) return n;
    Nodo* enIzq = buscarJefeEnNodo(n->izq);
    if (enIzq != NULL) return enIzq;
    return buscarJefeEnNodo(n->der);
}

Nodo* buscarJefeActual(Arbol& arbol) {
    return buscarJefeEnNodo(arbol.raiz);
}

int contarJefesEnNodo(Nodo* n) {
    if (n == NULL) return 0;
    int suma = contarJefesEnNodo(n->izq) + contarJefesEnNodo(n->der);
    if (n->esJefe) suma++;
    return suma;
}

int contarJefes(Arbol& arbol) {
    return contarJefesEnNodo(arbol.raiz);
}

void imprimirNombre(Nodo* n) {
    cout << "[" << n->id << "] " << n->nombre << " " << n->apellido;
}

void imprimirNodo(Nodo* n, int nivel) {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) cout << "    ";
    imprimirNombre(n);
    cout << " (" << n->genero << ", " << n->edad << " anios)";
    if (n->esJefe) cout << " <JEFE ACTUAL>";
    else if (n->fueJefe) cout << " (ex jefe)";
    if (n->muerto) cout << " [MUERTO]";
    if (n->preso) cout << " [PRESO]";
    cout << endl;
    imprimirNodo(n->izq, nivel + 1);
    imprimirNodo(n->der, nivel + 1);
}

void mostrarArbol(Arbol& arbol) {
    if (arbol.raiz == NULL) {
        cout << "el arbol se encuentra vacio." << endl;
        return;
    }
    imprimirNodo(arbol.raiz, 0);
}

Nodo* hermano(Nodo* n) {
    if (n == NULL || n->padre == NULL) return NULL;
    if (n->padre->izq == n) return n->padre->der;
    return n->padre->izq;
}

int profundidad(Nodo* n) {
    int nivel = 0;
    while (n->padre != NULL) {
        n = n->padre;
        nivel++;
    }
    return nivel;
}

// se sube el mas profundo hasta igualar niveles y despues los dos juntos
// hasta que lleguen al mismo ancestro
int distancia(Nodo* a, Nodo* b) {
    int nivelA = profundidad(a);
    int nivelB = profundidad(b);
    int saltos = 0;
    while (nivelA > nivelB) { a = a->padre; nivelA--; saltos++; }
    while (nivelB > nivelA) { b = b->padre; nivelB--; saltos++; }
    while (a != b) {
        a = a->padre;
        b = b->padre;
        saltos = saltos + 2;
    }
    return saltos;
}
