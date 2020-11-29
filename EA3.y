%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <conio.h>
#include "y.tab.h"
#include "funciones.c"

int yylex();
int yyerror(char* mensaje);
extern int yylineno;
FILE *fp;
char str_aux[100];

/* Cosas para la declaracion de variables y la tabla de simbolos */
int tipoDatoADeclarar[TAMANIO_TABLA];
int indice_tabla = -1;
int cont=0;

FILE  *yyin;

//Indices tercetos
int IdxWrite;
int IdxRead;
int asigIdx;
int posicionIdx;
int listaIdx;
int posIdx;
int resultIdx;
int flagIdx;


%}

%union {
int valor_int;
char *valor_string;
}


%token READ POSICION WRITE
%token COMA PYC
%token PARA PARC
%token CA CC
%token ASIGNA

%token <valor_string>ID
%token <valor_string>CTE_S 
%token <valor_int>CTE


%%

s : prog          {
                        printf("\nCompilacion OK \n");
                        generarAsm();
                  };

prog : sent
      | prog sent

sent : read 
      | write 
      | asig

read : READ ID    {
                        agregarEnTabla(yylval.valor_string,yylineno);
				crearTercetoSimple("READ",$2,Int); 
                  }
asig : ID ASIGNA posicion     {
                                    agregarEnTabla($1,yylineno);
                                    if(posicionIdx==-1){
                                          crearTercetoSimple("MENSAJE","@mensajeListavacia",CteString);
                                    }
                                    else{
                                          crearTercetoIdx("BEQ",crearTerceto("0"),posicionIdx);
                                          asigIdx=crearTercetoIdx("=",crearTerceto($1),posicionIdx);
                                          crearTerceto("FIN_IF");
                                          crearTercetoIdx("BNE",crearTerceto("0"),posicionIdx);
                                          crearTercetoSimple("MENSAJE","@mensajeNoEncontrado",CteString);//Elemnto no encontrado
                                          crearTerceto("FIN_IF");
                                    }
                              }

posicion : POSICION PARA ID PYC CA  {
                                          posicionIdx=crearTerceto($3);
                                    }
            lista CC PARC
                              {
                                    posicionIdx=listaIdx;
                              }
      | POSICION PARA ID PYC CA CC PARC
                              {
                                    posicionIdx=-1;
                              }

lista : CTE             {
                              agregarCteATabla(Cte);cont++;
                              flagIdx = crearTerceto("_flag");
                              crearTercetoIdx("=",flagIdx,crearTerceto("0"));//Flag 
                              crearTercetoIdx("=",crearTerceto("_cont"),crearTerceto("1"));//cont
                              posIdx=crearTercetoIdx("=",crearTerceto("_posf"),crearTerceto("0"));//posicion encontrada del elemento
                              sprintf(str_aux,"%d",$1);
                              crearTercetoIdx("BNE",crearTerceto(str_aux),posicionIdx);
                              listaIdx=crearTerceto("_posf");
                              crearTercetoIdx("=",listaIdx,crearTerceto("1"));//posicion Encontrado 
                              crearTercetoIdx("=",flagIdx,crearTerceto("1"));//Flag Encontrado 
                              crearTerceto("FIN_IF");
                        }
      | lista COMA CTE  {
                              agregarCteATabla(Cte);cont++;
                              sprintf(str_aux,"%d",$3);
                              crearTercetoIdx("BNE",crearTerceto(str_aux),posicionIdx);
                              crearTercetoIdx("BNE",crearTerceto("0"),flagIdx);
                              crearTercetoIdx("=",flagIdx,crearTerceto("1"));//Flag Encontrado
                              listaIdx=crearTerceto("_posf");
                              sprintf(str_aux,"%d",cont);
                              insertarEnTabla(cont);
                              crearTercetoIdx("=",listaIdx,crearTerceto(str_aux));//posicion Encontrado 
                              crearTerceto("FIN_IF");
                              crearTerceto("FIN_IF");
                        }

write : WRITE CTE_S     {
                              agregarCteATabla(CteString);
					crearTercetoSimple("WRITE",$2,CteString); 
                        }
      | WRITE ID        {
                              agregarEnTabla(yylval.valor_string,yylineno);
					crearTercetoSimple("WRITE",$2,Int); 
                        }

%%

int main(int argc,char *argv[])
{
      if ((yyin = fopen(argv[1], "rt")) == NULL)
      {
            printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
      }
      else
      {
            yyparse();
            fclose(yyin);
      }      
      grabarTabla();
      grabarTercetos();
      
      return 0;
}
