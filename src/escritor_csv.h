#ifndef ENTRADA_H
#define ENTRADA_H


// "tam" es el tamano del arreglo destino
void leerTexto(const char* mensaje, char* destino, int tam);
int leerEntero(const char* mensaje, int minimo, int maximo);
bool leerBandera(const char* mensaje);   // 0 o 1
char leerGenero(const char* mensaje);    // H o M
bool confirmar(const char* mensaje);     // s o n

#endif
