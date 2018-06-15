#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"



/*
* TP 00 - K2055
* Un  escaner  elemental
* Grupo 1
* 
* Integrantes
* Matias Buch	248397-0
*/






int contador_identificador = 0;
int contador_constante = 0;
int contador_error = 0;

void imprimir_token(t_token* token);
void imprimir_total();

void imprimir_token(t_token* token) {
	
	if (token != NULL) {
		printf("%s\n", token->lexema);

		switch (token->tipo) {
		case tk_IDENTIFICADOR:
			contador_identificador++;
			break;
		case tk_CONSTANTE:
			contador_constante++;
			break;
		case tk_ERROR:
			contador_error++;
			break;
		}
	}
}

void imprimir_total() {
	printf("----\n");
	printf("Totales:\n");
	printf("Identificadores %i\n", contador_identificador);
	printf("Constantes enteras %i\n", contador_constante);
	printf("Errores %i\n", contador_error);
}

int main()
{
	t_token *token;

	// Cargo la tabla de transiciones
	iniciar_tabla_transicion();

	// El bucle intenta generar todos los tokens posibles a partir de los caracteres del input
	do {
		token = scan_token();
		imprimir_token(token);
		free_token(token);
	} while (token != NULL);

	// imprime la contabilizacion total de tokens agrupados por tipo
	imprimir_total();

	return 0;
}