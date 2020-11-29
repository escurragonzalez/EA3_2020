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
int pos;

FILE  *yyin;

//Indices tercetos
int IdxWrite;
int IdxRead;
int asigIdx;
int posicionIdx;
int listaIdx;
int posIdx;
int resultIdx;

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
                                    crearTercetoIdx("BEQ",crearTerceto("0"),posicionIdx);
                                    asigIdx=crearTercetoIdx("=",crearTerceto($1),posicionIdx);
                                    crearTerceto("SEGUIR");
                                    crearTercetoIdx("BNE",crearTerceto("0"),posicionIdx);
                                    crearTerceto("MENSAJE");//Elemnto no encontrado
                                    crearTerceto("SEGUIR");
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
                                    posicionIdx=crearTerceto("-2");
                              }

lista : CTE             {
                              agregarCteATabla(Cte);pos=1;
                              posIdx=crearTercetoIdx("=",crearTerceto("_pos"),crearTerceto("0"));//posicion encontrada del elemnto
                              sprintf(str_aux,"%d",$1);
                              crearTercetoIdx("BNE",crearTerceto(str_aux),posicionIdx);
                              listaIdx=crearTerceto("_pos");
                              crearTercetoIdx("=",listaIdx,crearTerceto("1"));//posicion Encontrado 
                              crearTercetoIdx("=",crearTerceto("_flag"),crearTerceto("1"));//Flag Encontrado 
                              crearTerceto("SEGUIR");
                        }
      | lista COMA CTE  {
                              agregarCteATabla(Cte);pos++;
                              sprintf(str_aux,"%d",$3);
                              listaIdx=crearTerceto(str_aux);
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
