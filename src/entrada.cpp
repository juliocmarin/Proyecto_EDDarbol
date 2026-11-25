#include <iostream>
#include <cstdlib>
#include <cstring>
#include "entrada.h"
#include "validaciones.h"
using namespace std;


void leerLinea(const char* mensaje, char* buffer, int tam) {
    cout << mensaje;
    cin.getline(buffer, tam);
    if (cin.eof()) {
        cout << endl << "Entrada finalizada. Hasta luego." << endl;
        exit(0);
    }
    if (cin.fail()) {          
        cin.clear();
        cin.ignore(10000, '\n');
        strcpy(buffer, "");
    }
    quitarEspacios(buffer);
}

void leerTexto(const char* mensaje, char* destino, int tam) {
    while (true) {
        leerLinea(mensaje, destino, tam);
        if (textoValido(destino)) return;
        cout << "  Dato invalido: no puede estar vacio, tener comas ni ser muy largo." << endl;
    }
}

int leerEntero(const char* mensaje, int minimo, int maximo) {
    char texto[30];
    while (true) {
        leerLinea(mensaje, texto, 30);
        if (esNumero(texto)) {
            int valor = atoi(texto);
            if (valor >= minimo && valor <= maximo) return valor;
        }
        cout << "  Ingrese un numero entre " << minimo << " y " << maximo << "." << endl;
    }
}

bool leerBandera(const char* mensaje) {
    char texto[30];
    while (true) {
        leerLinea(mensaje, texto, 30);
        if (esBandera(texto)) return texto[0] == '1';
        cout << "  Solo se permite 0 (no) o 1 (si)." << endl;
    }
}

char leerGenero(const char* mensaje) {
    char texto[30];
    while (true) {
        leerLinea(mensaje, texto, 30);
        if (esGenero(texto)) return toupper(texto[0]);
        cout << "  Solo se permite H o M." << endl;
    }
}

bool confirmar(const char* mensaje) {
    char texto[30];
    while (true) {
        cout << mensaje << " (s/n)" << endl;
        leerLinea("> ", texto, 30);
        if (strcmp(texto, "s") == 0 || strcmp(texto, "S") == 0) return true;
        if (strcmp(texto, "n") == 0 || strcmp(texto, "N") == 0) return false;
        cout << "  Responda s o n." << endl;
    }
}

