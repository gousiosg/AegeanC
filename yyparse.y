
%union yylval{           
	char *s;
	int n;
	struct s_tree *t;
}

%token  IF
%token  ELSE
%token  WHILE
%token  PUTSTR
%token  GETINT
%token  PUTINT
%token  INT
%token  <s> VAR
%token  <s> STR
%token  <n> NUM
%type   <t> file prog makefun int while
%type   <t> ifelse commands putstr getint putint
%type   <t> userfun praxis expr command
%left '-' '+'
%left '*' '/'
%left NEG 

%%

file :   prog        {t=mkfile($1);}
;
                     /*The nodes are constructed and linked*/
prog: /* EMPTY */    {$$= mkprog(NULL,NULL,NULL);}
	|makefun prog    {$$= mkprog(NULL,$1,$2);}
	|int prog        {$$= mkprog($1,NULL,$2);}
;
									/*The functions names are added to the symbol table*/
makefun: VAR '(' ')''{' commands '}'    {s_list_add(fun_list,$1);$$= mkfun($5,$1);}
;

int:     INT VAR ';'                    {m_list_add(var_list,$2);$$= mkint($2);}             
;

while:   WHILE '(' expr ')' '{' commands '}'  {$$= mkwhile($3,$6);} 
;

ifelse:  IF '(' expr ')' '{' commands '}'                    {$$= mkifelse($3,$6,NULL);}
	|IF '(' expr ')' '{' commands '}' ELSE '{' commands '}'  {$$= mkifelse($3,$6,$10);}
;

commands: command                      {$$= mkcommands($1,NULL);}
	 |command commands                 {$$= mkcommands($1,$2);}
;

command:  praxis
	 |while
	 |ifelse
	 |userfun
	 |putstr
 	 |getint
	 |putint
;

putstr:  PUTSTR '(' STR ')' ';'        { $$=mkputstr($3);}
;

putint:  PUTINT '(' expr ')' ';'       { $$=mkputint($3);}
;

getint:  GETINT '(' VAR ')' ';'        { m_search_results(var_list,$3);$$=mkgetint($3);}
;
									/*Searches if the called functions have been declared*/
userfun: VAR '(' ')' ';'               {s_search_results(fun_list,$1);$$= mkuserfun($1);}
;

praxis:  VAR '=' expr ';'              {m_search_results(var_list,$1); $$=mkbinop(mkvariable($1),$3,'=');}
;
                                  /*Searches if the variables have been declared*/
expr:    VAR                      { m_search_results(var_list,$1);$$=mkvariable($1);}
	|NUM                          { $$= mknumber($1);}
	|expr '+' expr                { $$=mkbinop($1,$3,'+');}
	|expr '-' expr                { $$=mkbinop($1,$3,'-');}
	|expr '*' expr                { $$=mkbinop($1,$3,'*');}
	|expr '/' expr                { $$=mkbinop($1,$3,'/');}
	|expr '=' '=' expr	          { $$= mkcmp($1,$4,"==");}
	|expr '!' '=' expr            { $$= mkcmp($1,$4,"!=");}
	|expr '<' expr                { $$=mkcmp($1,$3,"<");} 
	|expr '>' expr                { $$= mkcmp($1,$3,">");}
	|'-' expr  %prec NEG          { $$=mkunop($2,'-');}
	|'(' expr ')'                 { $$=$2;}
;

%%

#include <stdio.h>

struct s_tree *t;
struct list *var_list;
struct list *fun_list;

main()
{
	var_list=var_list_new();
	fun_list=fun_list_new();
	yyparse();
	codegen(t); 
}

void
yyerror (char *s)
{
	printf("%s\n", s);
}

