#include <fstream>
#include "escritor_csv.h"
using namespace std;

void escribirNodo(ofstream& salida, Nodo* n) {
    if (n == NULL) return;
    salida << n->id << "," << n->nombre << "," << n->apellido << "," << n->genero << ","
           << n->edad << "," << n->idJefe << ","
           << n->muerto << "," << n->preso << ","
           << n->fueJefe << "," << n->esJefe << "\n";
    escribirNodo(salida, n->izq);
    escribirNodo(salida, n->der);
}

bool guardarCsv(const char* ruta, Arbol& arbol) {
    ofstream salida(ruta);
    if (!salida.is_open()) return false;

    salida << "id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss\n";
    escribirNodo(salida, arbol.raiz);
    salida.close();
    return true;
}
