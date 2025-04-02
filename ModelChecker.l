%{
	#include "y.tab.h"


%}

%option noyywrap


%%
"beginKripke" {return BEGK;}
"endKripke" {return ENDK;}
"beginCTL" {return BEGCTL;}
"endCTL" {return ENDCTL;}
"&" {return AND;}
"|" {return OR;}
"->" {return IMPLIES;}
"~" {return NOT;}
"T" {return TOP;}
"F" { return BOTTOM;}

"AG" {return AG;}
"EG" { return EG;}
"EF" {return EF;}
"AF" {return AF;}
"AX" {return AX;}
"EX" {return EX;}
"A" { return A;}
"E" { return E;}
"U" {return U;}
"(" {return LPAREN;}
")" {return RPAREN;}
"[" {return yytext[0];}
"]" {return yytext[0];}
":" {return yytext[0];}
"," {return yytext[0];}
"STATES" {return STATES;}
"TRANSITIONS" {return TRANS;}
"LABELS" {return LS;}
[A-Z][0-9] {yylval.state_var = strdup(yytext);
return STATE_VAR;}
[a-z] {yylval.prop_var = yytext[0];
return PROP_VAR;
}

[ \t\n]			{}	
.				{ return 0;
				}		


%%
