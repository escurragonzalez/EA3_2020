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
int auxOperaciones=0;

FILE  *yyin;

//Indices tercetos
int IdxWrite;
int IdxRead;
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
                              }

posicion : POSICION PARA ID PYC CA lista CC PARC
      | POSICION PARA ID PYC CA CC PARC

lista : CTE             {
                              agregarCteATabla(Cte);
                        }
      | lista COMA CTE  {
                              agregarCteATabla(Cte);
                        }

write : WRITE CTE_S     {
                              agregarCteATabla(CteString);
					crearTercetoSimple("WRITE",$2,CteString); 
                        }
      | WRITE ID        {
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
