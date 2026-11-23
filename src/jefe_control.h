#ifndef JEFE_CONTROL_H
#define JEFE_CONTROL_H

#include "arbol.h"


Nodo* elegirNuevoJefe(Arbol& arbol, Nodo* base);

// el jefe viejo pasa a ex jefe y el nuevo queda como jefe actual
void traspasarJefatura(Nodo* viejo, Nodo* nuevo);


void revisarJefe(Arbol& arbol);

#endif
