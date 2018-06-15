#include "scanner.h"

static int tabla_transicion[7][5];


t_caracter get_tipo_caracter(int c) {
	if (isalpha(c))
		return tc_CARACTER;
	
	if (isdigit(c))
		return tc_DIGITO;
	
	if (isspace(c))
		return tc_ESPACIO;
	
	if (c == EOF)
		return tc_EOF;

	return tc_OTRO;
}


t_tipo_token get_tipo_token(t_estado estado) {
	switch (estado)
	{
	case est_IDENTIFICADOR:
		return tk_IDENTIFICADOR;
	case est_CONSTANTE:
		return tk_CONSTANTE;
	default:
		return tk_ERROR;
	}
}


bool aceptor(t_estado estado) {
	
	return (estado == est_IDENTIFICADOR) || (estado == est_CONSTANTE) || (estado == est_ERROR_LEXICO);
}


t_token * scan_token() {
	
	int c;
	int estado = est_INICIAL;		
	t_caracter tipo_caracter;	
	char *lexema = malloc(1);
	int longitud = 0;

	while ((c = getc(stdin)) != EOF) {
		
		tipo_caracter = get_tipo_caracter(c);
		// obtengo el nuevo estado de la tabla de transiciones a partir del estado anterior y el grupo del caracter leido
		estado = tabla_transicion[estado][tipo_caracter];

		// Se ignoran los espacios
		if (tipo_caracter != tc_ESPACIO) {			
			lexema = realloc(lexema, longitud + 1);
			lexema[longitud] = c;
			longitud++;
		} 

		// Si el nuevo estado es de tipo aceptor, corto el bucle y proceso el token
		if (aceptor(estado))
			break;
	}	
	
	if ((c != EOF) && (tipo_caracter != tc_ESPACIO))
		ungetc(c, stdin);

	return nuevo_token(lexema, longitud, estado);
}

t_token * nuevo_token(char *lexema, int longitud, t_estado estado) {
	t_token *token = NULL;

	size_t length = strlen(lexema);

	if (longitud == 0) {
		free(lexema);
		return NULL;		
	}

	if (aceptor(estado)) {
		token = malloc(sizeof(t_token));
		token->lexema = malloc(longitud + 1);
		//lexema[longitud - 1] = '\0';
		strncpy(token->lexema, lexema, longitud);
		token->lexema[longitud] = '\0';
		token->tipo = get_tipo_token(estado);
	}
	
	return token;
}



void free_token(t_token *token) {
	if (token == NULL)
		return;		

	if (token->lexema != NULL)
		free(token->lexema);

	free(token);
}

void iniciar_tabla_transicion() {
	tabla_transicion[est_INICIAL][tc_CARACTER] = est_REC_IDENT;
	tabla_transicion[est_INICIAL][tc_DIGITO] = est_REC_CONST;
	tabla_transicion[est_INICIAL][tc_ESPACIO] = est_INICIAL;
	tabla_transicion[est_INICIAL][tc_OTRO] = est_REC_ERR_LEX;
	tabla_transicion[est_INICIAL][tc_EOF] = est_INICIAL;
	
	tabla_transicion[est_REC_IDENT][tc_CARACTER] = est_REC_IDENT;
	tabla_transicion[est_REC_IDENT][tc_DIGITO] = est_REC_IDENT;
	tabla_transicion[est_REC_IDENT][tc_ESPACIO] = est_IDENTIFICADOR;
	tabla_transicion[est_REC_IDENT][tc_OTRO] = est_IDENTIFICADOR;
	tabla_transicion[est_REC_IDENT][tc_EOF] = est_IDENTIFICADOR;

	tabla_transicion[est_IDENTIFICADOR][tc_CARACTER] = est_FIN_LEXEMA;
	tabla_transicion[est_IDENTIFICADOR][tc_DIGITO] = est_FIN_LEXEMA;
	tabla_transicion[est_IDENTIFICADOR][tc_ESPACIO] = est_FIN_LEXEMA;
	tabla_transicion[est_IDENTIFICADOR][tc_OTRO] = est_FIN_LEXEMA;
	tabla_transicion[est_IDENTIFICADOR][tc_EOF] = est_FIN_LEXEMA;

	tabla_transicion[est_REC_CONST][tc_CARACTER] = est_CONSTANTE;
	tabla_transicion[est_REC_CONST][tc_DIGITO] = est_REC_CONST;
	tabla_transicion[est_REC_CONST][tc_ESPACIO] = est_CONSTANTE;
	tabla_transicion[est_REC_CONST][tc_OTRO] = est_CONSTANTE;
	tabla_transicion[est_REC_CONST][tc_EOF] = est_CONSTANTE;

	tabla_transicion[est_CONSTANTE][tc_CARACTER] = est_FIN_LEXEMA;
	tabla_transicion[est_CONSTANTE][tc_DIGITO] = est_FIN_LEXEMA;
	tabla_transicion[est_CONSTANTE][tc_ESPACIO] = est_FIN_LEXEMA;
	tabla_transicion[est_CONSTANTE][tc_OTRO] = est_FIN_LEXEMA;
	tabla_transicion[est_CONSTANTE][tc_EOF] = est_FIN_LEXEMA;

	tabla_transicion[est_REC_ERR_LEX][tc_CARACTER] = est_ERROR_LEXICO;
	tabla_transicion[est_REC_ERR_LEX][tc_DIGITO] = est_ERROR_LEXICO;
	tabla_transicion[est_REC_ERR_LEX][tc_ESPACIO] = est_ERROR_LEXICO;
	tabla_transicion[est_REC_ERR_LEX][tc_OTRO] = est_REC_ERR_LEX;
	tabla_transicion[est_REC_ERR_LEX][tc_EOF] = est_ERROR_LEXICO;

	tabla_transicion[est_ERROR_LEXICO][tc_CARACTER] = est_FIN_LEXEMA;
	tabla_transicion[est_ERROR_LEXICO][tc_DIGITO] = est_FIN_LEXEMA;
	tabla_transicion[est_ERROR_LEXICO][tc_ESPACIO] = est_FIN_LEXEMA;
	tabla_transicion[est_ERROR_LEXICO][tc_OTRO] = est_FIN_LEXEMA;
	tabla_transicion[est_ERROR_LEXICO][tc_EOF] = est_FIN_LEXEMA;
}