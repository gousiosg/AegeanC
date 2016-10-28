%{
#include "y.tab.h"
%}

%option noyywrap


%%


[\ \t\n]+			

[0-9]*				{ yylval.n = atoi(yytext); return NUM; }

while				    	{ return WHILE; }

if		        		{ return IF; }

else			    		{ return ELSE; }

putstring				{ return PUTSTR; }

getint 				    	{ return GETINT; }

putint 				    	{ return PUTINT; }

int\  				    	{ return INT; }

[a-zA-Z]([a-zA-Z0-9_])*        		{ yylval.s = strdup(yytext);return VAR; }

\"[^".]*\"				{ yylval.s = strdup(yytext);return STR;}

"\/\/".*			          /*comments*/

.				 	{return *yytext;}

%%
