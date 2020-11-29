%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "y.tab.h"
#include "funciones.h"

%}

%option noyywrap  
%option yylineno 

DIGITO			        [0-9]
LETRA			        [a-zA-Z]
INICIO_FIN_STR          \"
CTE_S               	{INICIO_FIN_STR}{CADENA}*{INICIO_FIN_STR}
CTE               		{DIGITO}+
ID			            {LETRA}({LETRA}|{DIGITO}|_)*
CADENA                  ({LETRA}|{DIGITO}|\.|\:|\;\-|\_|\!|_|" "|\n|\s|\t|\r|\@|\%\!)*

%%


"READ"|"read"		    { return READ;}
"POSICION"|"posicion"	{ return POSICION;}
"WRITE"|"write"		    { return WRITE;}
"("			            { return PARA;}
")"			            { return PARC;}
"["			            { return CA;}
"]"			            { return CC;}
"="                     { return ASIGNA;}
";"						{ return PYC;}   
{ID}			        {
			                if(strlen(yytext) <= 30){
			                  yylval.valor_string = strdup(yytext);
			                  return ID;
			                }
							mensajeDeError(ErrorLexico,"El nombre de variable excede el limite permitido.",yylineno);
			            }                            
{CTE}		        { 
                        if(atoll(yytext)>=-32768 && atoll(yytext)<=32767){
		                      	yylval.valor_int = atoi(yytext);
		                      	return CTE;
		                    }
							 mensajeDeError(ErrorLexico,"El valor de un entero excede el limite permitido.",yylineno);
                        }
{CTE_S}		        {	
							if(strlen(yytext)<=100){
							yylval.valor_string = strdup(yytext);
							return CTE_S;
							}
							mensajeDeError(ErrorLexico,"El contenido de una cadena excede el limite permitido.",yylineno);
						}

","		           		{ return COMA;}

<<EOF>> 				{ yyterminate();}
"\n"
"\r\n"
"\t"
" "
. 						{ mensajeDeError(ErrorLexico,"Caracter invalido",yylineno);}

%%
