/*Tree.c: Syntax tree constructing functions*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/*construction of the root*/
struct s_tree *
mkfile(struct s_tree *prog)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=MYFILE;
	t->u.Program=prog;
	return(t);
}

/*construction of a program node*/
struct s_tree *
mkprog(struct s_tree *variable,struct s_tree *makefun,struct s_tree *prog)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=PROG;
	t->u.Prog.variable=variable;
	t->u.Prog.makefun=makefun;
	t->u.Prog.prog=prog;
	return(t);
}

/*construction of a user function node*/
struct s_tree *
mkfun(struct s_tree *commands,char *name)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=MAKEFUN;
	t->u.Makefun.commands=commands;
	t->u.Makefun.name=name;
	return(t);
}

/*construction of a variable declaration node*/
struct s_tree *
mkint(char *variable)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=INT;
	t->u.Variable=variable;
	return(t);
}

/*construction of a while command node*/
struct s_tree *
mkwhile(struct s_tree *cmp,struct s_tree *commands)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=WHILE;
	t->u.While.cmp=cmp;
	t->u.While.commands=commands;
	return(t);
}

/*construction of an if and else command node*/
struct s_tree *
mkifelse(struct s_tree *cmp,struct s_tree *commands,struct s_tree *else_commands)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=IFELSE;
	t->u.Ifelse.cmp=cmp;
	t->u.Ifelse.commands=commands;
    t->u.Ifelse.else_commands=else_commands;
	return(t);
}

/*construction of command nodes*/
struct s_tree *
mkcommands(struct s_tree *command,struct s_tree *commands)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=COMMANDS;
	t->u.Commands.command=command;
	t->u.Commands.commands=commands;
	return(t);
}

struct s_tree *
mkputstr(char *string)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=PUTSTR;
	t->u.String=string;
	return(t);
}


struct s_tree *
mkputint(struct s_tree *binop)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=PUTINT;
	t->u.Putint.binop=binop;
	return(t);
}

struct s_tree *
mkgetint(char *variable)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=GETINT;
	t->u.Variable=variable;
	return(t);
}

/*construction of a node which handles calls of user defined functions*/
struct s_tree *
mkuserfun(char *variable)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=USERFUN;
	t->u.Variable=variable;
	return(t);
}

/*construction of a binary operation node*/
struct s_tree *
mkbinop(struct s_tree *left,struct s_tree *right,char op)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=BINOP;
	t->u.Binop.left=left;
	t->u.Binop.right=right;
	t->u.Binop.op=op;
	return(t);
}

/*construction of a comparison node*/
struct s_tree *
mkcmp(struct s_tree *left,struct s_tree *right,char *op)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=CMP;
	t->u.Cmp.left=left;
	t->u.Cmp.right=right;
	t->u.Cmp.op=op;
	return(t);
}

struct s_tree *
mkunop(struct s_tree *exp,char op)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=UNOP;
	t->u.Unop.exp=exp;
	t->u.Unop.op=op;
	return(t);
}

/*construction of a node which carries a variable*/
struct s_tree *
mkvariable(char *variable)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=VAR;
	t->u.Variable=variable;
	return(t);
}

/*construction of a node which carries a nnumber*/
struct s_tree *
mknumber(int number)
{
	struct s_tree *t=(struct s_tree *)malloc(sizeof(struct s_tree));
	t->nt=NUM;
	t->u.Number=number;
	return (t);
}













	








	

		



	

		
