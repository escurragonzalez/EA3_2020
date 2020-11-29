#ifndef FUNCIONES_H
#define FUNCIONES_H

enum tipoError
{
    ErrorSintactico,
    ErrorLexico
};
/* Tipos de datos para la tabla de simbolos */

#define Int 1
#define Cte 2
#define CteString 3
#define SIN_MEM -4
#define NODO_OK -3
#define TRUE 1
#define FALSE 0

#define TAMANIO_TABLA 300
#define TAM_NOMBRE 51

/* Defino estructura de informacion*/
	typedef struct {
		char dato[40];
		int tipoDato;		
	}tInfo;

/* Defino estructura de nodo*/
typedef struct sNodo{
	tInfo info;
	struct sNodo *izq, *der;
}tNodo;

/* Defino estructura de arbol*/
typedef tNodo* tArbol;
tInfo infoArbol;

//Estructura del terceto
struct terceto {
	char *uno;
	char *dos;
	char *tres;
	int tipoDato;
};
struct terceto tercetos[1000];

int crearTercetoIdx(char *uno, int dos, int tres);
int crearTercetoSimple(char *uno, char * dos,int tipo);
int crearTerceto(char *uno);
void grabarTercetos();
void recorrerTercetos(FILE *);

typedef struct {
		char nombre[TAM_NOMBRE];
		int tipo_dato;
		char valor_s[TAM_NOMBRE];
		int valor_i;
		int longitud;
} TS_Reg;

TS_Reg tabla_simbolo[TAMANIO_TABLA];

void mensajeDeError(enum tipoError error,const char* info, int linea);
void agregarEnTabla(char* nombre,int linea);
void agregarCteATabla(int num);
int chequearVarEnTabla(char* nombre,int linea);
int buscarEnTabla(char * nombre);
void grabarTabla(void);
char* normalizarNombre(const char* nombre);
char * reemplazarCaracter(char * aux);
char* normalizarId(const char* cadena);
void agregarValorACte(int tipo);
void generarAsm();


#endif