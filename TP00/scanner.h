#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/*
*	Grupos de caracteres
*/
typedef enum {
	tc_CARACTER = 0,
	tc_DIGITO = 1,
	tc_ESPACIO = 2,
	tc_OTRO = 3,	
	tc_EOF = 4
} t_caracter;

/*
*	Tipos de tokens posibles
*/
typedef enum { 
	tk_IDENTIFICADOR, 
	tk_CONSTANTE,
	tk_ERROR
} t_tipo_token;

/*
*	Estructura de token
*	@lexema : cadena de caracteres
*	@tipo : tipo de token resultante, definido por el estado del automata al generar el lexema
*/
typedef struct {
	char* lexema;
	t_tipo_token tipo;
} t_token;


/*
*	Estados del automata
*/
typedef enum {
	est_INICIAL = 0,
	est_REC_IDENT = 1,
	est_IDENTIFICADOR = 2,
	est_REC_CONST = 3,
	est_CONSTANTE = 4,
	est_REC_ERR_LEX = 5,
	est_ERROR_LEXICO = 6,
	est_FIN_LEXEMA = 99
} t_estado;


/*
*	Carga las relaciones de estados y grupos de caracteres en la tabla de transicion
*/
void iniciar_tabla_transicion();

/*
*	Devuelve el grupo al que pertenece el caracter
*/
t_caracter get_tipo_caracter(int c);

/*
*	Devuelve el tipo de token segun el estado en que fue reconocido el lexema
*/
t_tipo_token get_tipo_token(t_estado estado);

/*
*	Determina si el estado del lexema es aceptado como token valido
*/
bool aceptor(t_estado estado);

/*
*	Lee los caracteres del standard input, genera el lexema y cataloga el token. Devuelve una structura con los datos del token
*	generado
*/
t_token * scan_token();

/*
*	Crea un nuevo token
*/
t_token * nuevo_token(char *lexema, int longitud, t_estado estado);


/*
*	Libera la memoria usada en la estructura del token
*/
void free_token(t_token *token);



#endif SCANNER_H_