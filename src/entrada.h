#ifndef ENTRADA_H
#define ENTRADA_H

void leerTexto(const char* mensaje, char* destino, int tam);
int leerEntero(const char* mensaje, int minimo, int maximo);
bool leerBandera(const char* mensaje);   
char leerGenero(const char* mensaje);    
bool confirmar(const char* mensaje);    

#endif
