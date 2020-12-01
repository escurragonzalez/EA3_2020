#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "funciones.h"

extern int tipoDatoADeclarar[TAMANIO_TABLA];
extern int indice_tabla;
extern int auxOperaciones;
int terceto_idx=0;

char msg[100];
char aux_str[100];
		
void mensajeDeError(enum tipoError error,const char* info, int linea){
  switch(error){ 
        case ErrorLexico: 
            printf("ERROR Lexico en la linea %d. Descripcion: %s\n",linea,info);
            break ;

		case ErrorSintactico: 
            printf("ERROR Sintactico en la linea %d. Descripcion: %s.\n",linea,info);
            break ;
  }

  system ("Pause");
  exit (1);
}


/* Devuleve la posicion en la que se encuentra el elemento buscado, -1 si no encontro el elemento */
int buscarEnTabla(char * nombre){
   int i=0;
   while(i<=indice_tabla){
	   if(strcmp(tabla_simbolo[i].nombre,normalizarId(nombre)) == 0){
		   return i;
	   }
	   i++;
   }
   return -1;
}

int yyerror(char* mensaje){
	printf("Syntax Error: %s\n", mensaje);
	system ("Pause");
	exit (1);
}

 void agregarEnTabla(char* nombre,int linea){
	 if(indice_tabla >= TAMANIO_TABLA - 1){
		 printf("Error: No hay mas espacio en la tabla de simbolos.\n");
		 system("Pause");
		 exit(2);
	 }

	 if(buscarEnTabla(nombre) == -1){
		 //Agregar a tabla
		 indice_tabla ++;
		 strcpy(tabla_simbolo[indice_tabla].nombre,normalizarId(nombre));
		 tabla_simbolo[indice_tabla].tipo_dato = Int;
	 }
 }


/** Guarda la tabla de simbolos en un archivo de texto */
void grabarTabla(){
	if(indice_tabla == -1)
		mensajeDeError(ErrorSintactico,"No se encontro la tabla de simbolos",0);

	FILE* arch = fopen("ts.txt", "w+");
	if(!arch){
		printf("No se pudo crear el archivo ts.txt\n");
		return;
	}
	
	int i;
	char valor[TAM_NOMBRE];
	fprintf(arch, "%-50s|%-14s|%-10s|%-30s\n","NOMBRE","TIPO","LONGITUD","VALOR");
	fprintf(arch, ".....................................................................................................................\n");
	for(i = 0; i <= indice_tabla; i++){
		fprintf(arch, "%-50s", &(tabla_simbolo[i].nombre) );
			
		switch (tabla_simbolo[i].tipo_dato){
		case Int:
			fprintf(arch, "|%-14s|%-10s|%-30s", "INT","--","--");
			break;
		case Cte:
			fprintf(arch, "|%-14s|%-10s|%-30d", "CTE_INT","--",tabla_simbolo[i].valor_i);
			break;
		case CteString:
			fprintf(arch, "|%-14s|%-10d|%-30s", "CTE_STRING",tabla_simbolo[i].longitud,&(tabla_simbolo[i].valor_s));
			break;
		}

		fprintf(arch, "\n");
	}
	fclose(arch);
}

 

/** Agrega una constante a la tabla de simbolos */
void agregarCteATabla(int num){
	char nombre[30];

	if(indice_tabla >= TAMANIO_TABLA - 1){
		printf("Error: No hay mas espacio en la tabla de simbolos.\n");
		system("Pause");
		exit(2);
	}
	
	switch(num){
		case Cte:
			sprintf(nombre, "%d", yylval.valor_int);
			//Si no hay otra variable con el mismo nombre...
			if(buscarEnTabla(nombre) == -1){
			//Agregar nombre a tabla
				indice_tabla++;
				strcpy(tabla_simbolo[indice_tabla].nombre,normalizarId(nombre));
			//Agregar tipo de dato
				tabla_simbolo[indice_tabla].tipo_dato = Cte;
			//Agregar valor a la tabla
				tabla_simbolo[indice_tabla].valor_i = yylval.valor_int;
			}
		break;

		case CteString:
			strcpy(nombre,normalizarNombre(yylval.valor_string));
			memmove(&nombre[0], &nombre[1], strlen(nombre));//Remover el primer guion "_"
			if(buscarEnTabla(nombre) == -1){
			//Agregar nombre a tabla
				indice_tabla ++;
				strcpy(tabla_simbolo[indice_tabla].nombre,normalizarNombre(yylval.valor_string));				
				//Agregar tipo de dato
				tabla_simbolo[indice_tabla].tipo_dato = CteString;

				//Agregar valor a la tabla
				int length = strlen(yylval.valor_string);
				char auxiliar[length];
				strcpy(auxiliar,yylval.valor_string);
				auxiliar[strlen(auxiliar)-1] = '\0';
				strcpy(tabla_simbolo[indice_tabla].valor_s, auxiliar+1);
				//Agregar longitud
				tabla_simbolo[indice_tabla].longitud = length -2;
				
			}
		break;
	}
}

char* normalizarNombre(const char* nombre){
    char *aux = (char *) malloc( sizeof(char) * (strlen(nombre)) + 2);
	char *retor = (char *) malloc( sizeof(char) * (strlen(nombre)) + 2);
	
	strcpy(retor,nombre);
	int len = strlen(nombre);
	retor[len-1] = '\0';
	
	strcpy(aux,"_");
	strcat(aux,++retor);

	return reemplazarCaracter(aux);
}

char* normalizarId(const char* cadena){
	char *aux = (char *) malloc( sizeof(char) * (strlen(cadena)) + 2);
	strcpy(aux,"_");
	strcat(aux,cadena);
	reemplazarCaracter(aux);
	return aux;
}

char * reemplazarCaracter(char * aux){
	int i=0;
	for(i = 0; i <= strlen(aux); i++)
  	{
  		if(aux[i] == '\t' || aux[i] == '\r' || aux[i] == ' ' || aux[i] == ':')  
		{
  			aux[i] = '_';
 		}

		if(aux[i] == '.')  
		{
  			aux[i] = 'p';
 		}
	}
	return aux;
}

void grabarTercetos() {
	FILE *f = fopen("Intermedia.txt", "w");

	if(f == NULL){
    	printf("No se pudo crear el archivo Intermedia.txt\n");
	}
	else{
		int i = 0;
		for (i;i<terceto_idx;i++) {
			fprintf(f, "%d (%s,%s ,%s )\n", i, tercetos[i].uno, tercetos[i].dos, tercetos[i].tres);
		}
		fclose(f);
	}
}

int crearTercetoIdx(char *uno, int dos, int tres) {
	struct terceto terc;
	int idx = terceto_idx;
	terc.uno = malloc(sizeof(char)*strlen(uno));
	strcpy(terc.uno, uno);
	
	if(dos!=-1){
		char *dos_char = (char*) malloc(sizeof(int));
		itoa(dos, dos_char, 10);
		terc.dos = malloc(sizeof(char)*strlen(dos_char));
		strcpy(terc.dos, dos_char);
	}
	else{
		terc.dos = malloc(sizeof(char)*strlen("_"));
		strcpy(terc.dos, "_");	
	}

	if(tres!=-1){
		char *tres_char = (char*) malloc(sizeof(int));
		itoa(tres, tres_char, 10);
		terc.tres = malloc(sizeof(char)*strlen(tres_char));
		strcpy(terc.tres, tres_char);
	}else{
		terc.tres = malloc(sizeof(char)*strlen("_"));
		strcpy(terc.tres, "_");
	}
	tercetos[idx] = terc;
	terceto_idx++;
	return idx;
}

int crearTercetoSimple(char *uno, char * dos,int tipo) {
	struct terceto terc;
	int idx = terceto_idx;
	terc.uno = malloc(sizeof(char)*strlen(uno));
	strcpy(terc.uno, uno);
	
	terc.dos = malloc(sizeof(char)*strlen(dos));
	strcpy(terc.dos, dos);	

	terc.tres = malloc(sizeof(char)*strlen("_"));
	strcpy(terc.tres, "_");
	terc.tipoDato = tipo;
	tercetos[idx] = terc;
	terceto_idx++;
	return idx;
}

int crearTerceto(char *uno) {
	struct terceto terc;
	int idx = terceto_idx;
	terc.uno = malloc(sizeof(char)*strlen(uno));
	strcpy(terc.uno, uno);		
	terc.dos = malloc(sizeof(char)*strlen("_"));
	strcpy(terc.dos, "_");		
	terc.tres = malloc(sizeof(char)*strlen("_"));
	strcpy(terc.tres, "_");
	tercetos[idx] = terc;
	terceto_idx++;
	return idx;
}

insertarEnTabla(int constante){
	char nombre[30];
	sprintf(nombre, "%d", constante);
	//Si no hay otra variable con el mismo nombre...
	if(buscarEnTabla(nombre) == -1){
	//Agregar nombre a tabla
		indice_tabla++;
		strcpy(tabla_simbolo[indice_tabla].nombre,normalizarId(nombre));
	//Agregar tipo de dato
		tabla_simbolo[indice_tabla].tipo_dato = Cte;
	//Agregar valor a la tabla
		tabla_simbolo[indice_tabla].valor_i = constante;
	}
}
void generarAsm(){	
      
	int i;//Contador for
	//Auxiliares
	insertarEnTabla(0);
	insertarEnTabla(1);
	agregarEnTabla("_posf",0);	
	agregarEnTabla("_flag",0);	
	FILE *pf = fopen("Final.asm", "w+");
	if (!pf){
		printf("Error al guardar el archivo assembler.\n");
		exit(1);
	}
	//includes asm
	fprintf(pf, "include macros2.asm\n");
	fprintf(pf, "include number.asm\n\n");
	fprintf(pf,".MODEL LARGE\n.STACK 200h\n.386\n.387\n.DATA\n\n\tMAXTEXTSIZE equ 50\n");
	
	//Data
	for (i = 0; i <= indice_tabla; i++){
		switch (tabla_simbolo[i].tipo_dato){
			case Int:
				fprintf(pf, "\t@%s \tDD 0\n",tabla_simbolo[i].nombre);
				break;
			case Cte:
				fprintf(pf,"\t@%s \tDD %d\n",tabla_simbolo[i].nombre,tabla_simbolo[i].valor_i);
				break;
			default:
				break;
			}
	}

	for (i = 0; i <= indice_tabla; i++){
		if(tabla_simbolo[i].tipo_dato ==CteString){
			fprintf(pf,"\t@%s \tDB \"%s\",'$',%d dup(?)\n",tabla_simbolo[i].nombre,tabla_simbolo[i].valor_s,50-tabla_simbolo[i].longitud);
		}
	}
	fprintf(pf,"\t@mensajeValidacion \tDB \"El valor debe ser >=1\",'$',29 dup(?)\n");
	fprintf(pf,"\t@mensajeListavacia \tDB \"La lista esta vacia\",'$',31 dup(?)\n");
	fprintf(pf,"\t@mensajeNoEncontrado \tDB \"Elemento no encontrado\",'$',28 dup(?)\n");

	fprintf(pf,"\n.CODE\n.startup\n\tmov AX,@DATA\n\tmov DS,AX\n\n\tFINIT\n");
	recorrerTercetos(pf);
	
	fprintf(pf,"\tffree\n");
	fprintf(pf,"\tmov ah, 4ch\n\tint 21h\n\n");

	//Fin archivo
	fprintf(pf, "\nEND");
	fclose(pf);	
}


void recorrerTercetos(FILE *pf){
	//Escribimos en el .asm 
	int pos;
	int i;
	int nroAuxEntero=0;
	int nroAuxReal=0;
	int ccond=0;
	char aux1[50]="aux\0";
	char nodo[50];
	for(i = 0; i < terceto_idx; i++){
	//Variables y Constantes
		sprintf(nodo,"%s",tercetos[i].uno);
	
		//WRITE
		if(strcmp(nodo,"WRITE")==0){
			switch(tercetos[i].tipoDato){
				case Int:
				case Cte:
					fprintf(pf,"\tdisplayInteger \t@%s,3\n\tnewLine 1\n",normalizarId(tercetos[i].dos));
				break;
				case CteString:
					strcpy(aux_str,normalizarNombre(tercetos[i].dos));
					fprintf(pf,"\tnewLine 1\n\tdisplayString \t@%s\n\tnewLine 1\n",aux_str);
				break;
			}
		}

		//READ
		if(strcmp(nodo,"READ")==0){
			
			fprintf(pf,"\tGetInteger \t@_%s\n",tercetos[i].dos);
			fprintf(pf,"\tfild \t@_0\n");
			fprintf(pf,"\tfild \t@_%s\n",tercetos[i].dos);
			fprintf(pf,"\tfcomp \n\tfstsw ax\n \tfwait \n\tsahf \n");
			fprintf(pf,"\tja FIN_IF%d \n",ccond);
			fprintf(pf,"\tBLOQ%d: \n",ccond);
			fprintf(pf,"\tdisplayString \t @mensajeValidacion\n");
			fprintf(pf,"\tmov ah, 4ch\n\tint 21h\n\n");
			fprintf(pf,"\tffree\n");
			fprintf(pf,"\tFIN_IF%d: \n",ccond);
			ccond++;
		}

		//Asignacion 
		if(strcmp(nodo,"=")==0 ){
			fprintf(pf,"\tffree\n");			
			sprintf(aux_str,"%s",tercetos[atoi(tercetos[i].tres)].uno);
			fprintf(pf,"\tfild \t@_%s\n",aux_str);
			fprintf(pf,"\tfistp \t@_%s\n",tercetos[atoi(tercetos[i].dos)].uno);
		}

		if(strcmp(nodo,"BNE")==0 ){
			fprintf(pf,"\tffree\n");
			fprintf(pf,"\tfild \t@_%s\n", tercetos[atoi(tercetos[i].dos)].uno);
			fprintf(pf,"\tfild \t@_%s\n", tercetos[atoi(tercetos[i].tres)].uno);
			fprintf(pf,"\tfcomp\n\tfstsw\tax\n\tfwait\n\tsahf\n\tjne\t\t");
			fprintf(pf,"FIN_IF%d\n",ccond);
		}

		if(strcmp(nodo,"BEQ")==0 ){
			fprintf(pf,"\tffree\n");
			fprintf(pf,"\tfild \t@_%s\n", tercetos[atoi(tercetos[i].dos)].uno);
			fprintf(pf,"\tfild \t@_%s\n", tercetos[atoi(tercetos[i].tres)].uno);
			fprintf(pf,"\tfcomp\n\tfstsw\tax\n\tfwait\n\tsahf\n\tje\t\t");
			fprintf(pf,"FIN_IF%d\n",ccond);
		}

		//ETIQUETAS
		if(strcmp(nodo,"FIN_IF")==0){
			fprintf(pf,"\tFIN_IF%d:\n",ccond);ccond++;
		}

		if(strcmp(nodo,"MENSAJE")==0){
			fprintf(pf,"\n\tdisplayString \t %s\n",tercetos[i].dos);
		}
	}
}