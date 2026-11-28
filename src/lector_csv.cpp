#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "lector_csv.h"
#include "validaciones.h"
using namespace std;

#define NUM_CAMPOS 10
#define TAM_CAMPO 60
#define TAM_LINEA 300


struct Pendiente {
    Nodo* nodo;
    int linea;
    Pendiente* sig;
};

// parte la linea por las comas, devuelve cuantos campos encontro
int separarCampos(char* linea, char campos[][TAM_CAMPO]) {
    int cantidad = 0;
    int pos = 0;
    for (int i = 0; linea[i] != '\0'; i++) {
        if (linea[i] == ',') {
            if (cantidad < NUM_CAMPOS) campos[cantidad][pos] = '\0';
            cantidad++;
            pos = 0;
        } else if (cantidad < NUM_CAMPOS && pos < TAM_CAMPO - 1) {
            campos[cantidad][pos] = linea[i];
            pos++;
        }
    }
    if (cantidad < NUM_CAMPOS) campos[cantidad][pos] = '\0';
    cantidad++;
    return cantidad;
}

// Convierte una linea en un nodo. Si algo esta mal devuelve NULL y deja
// el motivo en "error".
Nodo* leerNodo(char* linea, const char*& error) {
    char c[NUM_CAMPOS][TAM_CAMPO];

    if (separarCampos(linea, c) != NUM_CAMPOS) {
        error = "no tiene exactamente 10 columnas";
        return NULL;
    }
    for (int i = 0; i < NUM_CAMPOS; i++) quitarEspacios(c[i]);

    if (!esNumero(c[0]) || atoi(c[0]) <= 0) { error = "id invalido"; return NULL; }
    if (!textoValido(c[1]) || strlen(c[1]) >= 40) { error = "nombre vacio o muy largo"; return NULL; }
    if (!textoValido(c[2]) || strlen(c[2]) >= 40) { error = "apellido vacio o muy largo"; return NULL; }
    if (!esGenero(c[3])) { error = "gender debe ser H o M"; return NULL; }
    if (!esNumero(c[4]) || !edadValida(atoi(c[4]))) { error = "edad invalida"; return NULL; }
    if (!esNumero(c[5])) { error = "id_boss invalido"; return NULL; }
    if (!esBandera(c[6])) { error = "is_dead debe ser 0 o 1"; return NULL; }
    if (!esBandera(c[7])) { error = "in_jail debe ser 0 o 1"; return NULL; }
    if (!esBandera(c[8])) { error = "was_boss debe ser 0 o 1"; return NULL; }
    if (!esBandera(c[9])) { error = "is_boss debe ser 0 o 1"; return NULL; }
    if (atoi(c[0]) == atoi(c[5])) { error = "un miembro no puede ser su propio jefe"; return NULL; }

    Nodo* n = crearNodo();
    n->id = atoi(c[0]);
    strcpy(n->nombre, c[1]);
    strcpy(n->apellido, c[2]);
    n->genero = toupper(c[3][0]);
    n->edad = atoi(c[4]);
    n->idJefe = atoi(c[5]);
    n->muerto = (c[6][0] == '1');
    n->preso = (c[7][0] == '1');
    n->fueJefe = (c[8][0] == '1');
    n->esJefe = (c[9][0] == '1');
    return n;
}

void avisar(int linea, const char* motivo) {
    cerr << "  [linea " << linea << "] " << motivo << " (se omite)" << endl;
}

bool cargarCsv(const char* ruta, Arbol& arbol) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << ruta << endl;
        return false;
    }

    // 1) leer todas las lineas y guardarlas en la lista de pendientes
    Pendiente* cabeza = NULL;
    Pendiente* cola = NULL;
    int errores = 0;
    int numLinea = 0;
    char linea[TAM_LINEA];

    while (archivo.getline(linea, TAM_LINEA)) {
        numLinea++;
        quitarEspacios(linea);
        if (strlen(linea) == 0) continue;
        if (numLinea == 1 && strncmp(linea, "id", 2) == 0) continue;   // encabezado

        const char* error = "";
        Nodo* n = leerNodo(linea, error);
        if (n == NULL) {
            avisar(numLinea, error);
            errores++;
            continue;
        }

        Pendiente* p = new Pendiente;
        p->nodo = n;
        p->linea = numLinea;
        p->sig = NULL;
        if (cola == NULL) cabeza = p;
        else cola->sig = p;
        cola = p;
    }
    archivo.close();

    while (cabeza != NULL) {
        Pendiente* anterior = NULL;
        Pendiente* actual = cabeza;
        while (actual != NULL) {
            int idJefe = actual->nodo->idJefe;
            if (idJefe == 0 || buscarPorId(arbol, idJefe) != NULL) break;
            anterior = actual;
            actual = actual->sig;
        }
        if (actual == NULL) break;   // ninguna se puede colocar todavia

        Nodo* n = actual->nodo;
        if (buscarPorId(arbol, n->id) != NULL) {
            avisar(actual->linea, "id repetido");
            errores++;
            delete n;
        } else if (n->idJefe == 0 && arbol.raiz != NULL) {
            avisar(actual->linea, "ya hay una raiz (id_boss = 0)");
            errores++;
            delete n;
        } else if (!insertar(arbol, n)) {
            cerr << "  [linea " << actual->linea << "] el jefe " << n->idJefe
                 << " ya tiene dos sucesores (se omite)" << endl;
            errores++;
            delete n;
        }

        // sacar el pendiente de la lista
        if (anterior != NULL) anterior->sig = actual->sig;
        else cabeza = actual->sig;
        delete actual;
    }

    // 3) lo que sobro es porque su jefe nunca aparecio
    while (cabeza != NULL) {
        cerr << "  [linea " << cabeza->linea << "] el jefe " << cabeza->nodo->idJefe
             << " no existe (se omite)" << endl;
        errores++;
        Pendiente* siguiente = cabeza->sig;
        delete cabeza->nodo;
        delete cabeza;
        cabeza = siguiente;
    }

    if (arbol.cantidad == 0) {
        cerr << "No se cargo ningun miembro." << endl;
        return false;
    }
    if (contarJefes(arbol) > 1) {
        cerr << "El csv tiene mas de un jefe (is_boss = 1), solo puede haber uno." << endl;
        return false;
    }

    cout << "Se cargaron " << arbol.cantidad << " miembros";
    if (errores > 0) cout << " (" << errores << " lineas con errores)";
    cout << "." << endl;
    return true;
}
