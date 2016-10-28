/*Codegen.c: Functions to produce assembly code*/

#include <stdio.h>
#include <string.h>
#include "tree.h"

/*Function that generates labels*/
char *
new_label()
{
	static int counter=0;
	char c[8];
	sprintf(c,"L00%d",counter++);
	return strdup(c);
}

void codegen_main();
void codegen_num(int Number);
void codegen_int(char *Variable);  
void codegen_putstr(char *String);
void codegen_var(char * Variable);
void codegen_getint(char *Variable);
void codegen_userfun(char *Variable);
void codegen_unop(struct s_tree *exp);
void codegen_putint(struct s_tree *binop);
void codegen_makefun(struct s_tree *commands,char *name);
void codegen_add (struct s_tree *left,struct s_tree *right);
void codegen_sub (struct s_tree *left,struct s_tree *right);
void codegen_mul (struct s_tree *left,struct s_tree *right);
void codegen_div (struct s_tree *left,struct s_tree *right);
void codegen_eq  (struct s_tree *left,struct s_tree *right);
void codegen_less (struct s_tree *left,struct s_tree *right);
void codegen_eq_eq (struct s_tree *left,struct s_tree *right);
void codegen_not_eq (struct s_tree *left,struct s_tree *right);
void codegen_while(struct s_tree *cmp,struct s_tree *commands);
void codegen_greater (struct s_tree *left,struct s_tree *right);
void codegen_ifelse(struct s_tree *cmp,struct s_tree *commands,struct s_tree *else_commands);

/*Trverses the tree and calls the right function according to the node type*/ 
void
codegen(struct s_tree *t)
{
	if (t==NULL)
		return;
	switch(t->nt)
	{
	case MYFILE:                 /*Root of the tree*/
		codegen_main();          /*Calls the main function to produce the code from*/       
		codegen(t->u.Program);   /*where the assembly program begins*/
		break;
	case PROG:           /*A prog node can have either a variable or a function declaration*/
		if(t->u.Prog.variable==NULL)     /*If the next node is a function node*/ 
			codegen(t->u.Prog.makefun);
		else                             /*If the next node is a variable node*/
			codegen(t->u.Prog.variable);
		codegen(t->u.Prog.prog);
		break;
	case MAKEFUN:
		codegen_makefun(t->u.Makefun.commands,t->u.Makefun.name);
		break;
	case INT:
		codegen_int(t->u.Variable);
		break;
	case WHILE:
		codegen_while(t->u.While.cmp,t->u.While.commands);
		break;
	case IFELSE:
		codegen_ifelse(t->u.Ifelse.cmp,t->u.Ifelse.commands,t->u.Ifelse.else_commands);
		break;
	case COMMANDS:    /*To produce code for one or more commands*/
		codegen(t->u.Commands.command);
		codegen(t->u.Commands.commands);
		break;
	case PUTSTR:
		codegen_putstr(t->u.String);		
	 	break;
	case PUTINT:
		codegen_putint(t->u.Putint.binop);
		break;
	 case GETINT:
	 	codegen_getint(t->u.Variable);
	 	break;
	 case USERFUN:
	 	codegen_userfun(t->u.Variable);
	 	break;
	 case BINOP:      /*Call the right function according to the operator*/
	 	if(t->u.Binop.op=='+')
	 		codegen_add(t->u.Binop.left,t->u.Binop.right);
	 	else if(t->u.Binop.op=='-')
	 		codegen_sub(t->u.Binop.left,t->u.Binop.right);
	 	else if(t->u.Binop.op=='*')
	 		codegen_mul(t->u.Binop.left,t->u.Binop.right);
	 	else if(t->u.Binop.op=='/')
	 		codegen_div(t->u.Binop.left,t->u.Binop.right);
	 	else if(t->u.Binop.op=='=')
	 		codegen_eq(t->u.Binop.left,t->u.Binop.right);
	 	break;
	 case CMP:      /*Call the right function according to the comparison operator*/
	 	if(strcmp(t->u.Cmp.op,"==")==0)
	 		codegen_eq_eq(t->u.Cmp.left,t->u.Cmp.right);
	 	else if(strcmp(t->u.Cmp.op,"!=")==0)
	 		codegen_not_eq(t->u.Cmp.left,t->u.Cmp.right);
	 	else if(strcmp(t->u.Cmp.op,"<")==0)
	 		codegen_less(t->u.Cmp.left,t->u.Cmp.right);	
	 	else
	 		codegen_greater(t->u.Cmp.left,t->u.Cmp.right);		
	 	break;
	 case UNOP:
	 	codegen_unop(t->u.Unop.exp);
	 	break;
	 case VAR:
	 	codegen_var(t->u.Variable);
	 	break;
	 case NUM:
	 	codegen_num(t->u.Number);
	 	break;
	  }
}

/*Calls codegen to create code for the comparison node 
 *and the commands nodes, creates code to check if the condition is true. 
 *If it is, execute the commands and jump to  loop label again. 
 *If not, jump to end label
 */
void
codegen_while(struct s_tree *cmp, struct s_tree *commands)
{
	char *loop=new_label();
	char *end=new_label();
	
	printf(".%s:\n",loop);
	codegen(cmp);
	printf("\tpop %%eax\n");
	printf("\tmov $0,%%ebx\n");
	printf("\tcmp %%eax,%%ebx\n");
	printf("\tje .%s\n",end);
	codegen(commands);
	printf("\tjmp .%s\n",loop);
	printf(".%s:\n",end);
}

/*Calls codegen to create code for the comparison node, the commands nodes
 *and, if exists, the else node, creates code to check if the condition is true
 * and if it is, execute the commands and jump to end label. If not, jump 
 *to else label
 */
void
codegen_ifelse(struct s_tree *cmp,struct s_tree *commands,struct s_tree *else_commands)
{
	char *else_loop=new_label();
	char *end=new_label();
	
	codegen(cmp);
	printf("\tpop %%eax\n");
	printf("\tmov $0,%%ebx\n");
	printf("\tcmp %%eax, %%ebx\n");	
	printf("\tje .%s\n",else_loop);
	codegen(commands);
	printf("\tjmp .%s\n",end);
	printf(".%s:\n",else_loop);
	codegen(else_commands);
	printf(".%s:\n",end);
}

/*Creates the apropriate indeses so as the assembler regognises the function,
 *calls codegen to create code for the function and returns to the point 
 *the function was called
 */
void 
codegen_makefun(struct s_tree *commands,char *name)
{
	printf(".globl %s\n",name);
	printf("\t.type\t %s,@function\n",name);
	printf("%s:\n",name);
	codegen(commands);
	printf("\tret\n");
}

/*Creates an index for a new variable */
void
codegen_int(char *Variable)
{
	printf(".comm\t%s,4,4\n",Variable);
}

/*Creates code to push the variables to the stack */
void
codegen_var(char * Variable)
{
	printf("\tpush %s\n",Variable);
}

/*Creates labels so as to store the string ,
 *creates code to call printf to write it and to reinitialise the stack
 */
void
codegen_putstr(char *String)
{

	char *string=new_label();
	char *label=new_label();
	
	printf("\tjmp .%s\n",label);
	printf(".%s: .string ""%s""\n",string,String);
	printf(".%s:\n",label);
	printf("\tpush $.%s\n",string);
	printf("\tcall printf\n");
	printf("\taddl $4,%%esp\n");
}
/*Creates labels to store the string "%d", calls leal to push the address of the 
 *variable to the register %ebx then pushes %ebx and the string to the stack,
 *calls scanf and reinitialises the stack  	
 */

void 
codegen_getint(char *Variable)
{
	char *string=new_label();
	char *label=new_label();
	
	printf("\tjmp .%s\n",label);
	printf(".%s: .string \"%%d\"\n",string);
	printf(".%s:\n",label);
	printf("\tleal %s,%%ebx\n",Variable);
	printf("\tpush %%ebx\n");
	printf("\tpush $.%s\n",string); 
 	printf("\tcall scanf\n",Variable);
 	printf("\taddl $8,%%esp\n");
}

/*Creates labels so as to store "%d", calls codegen to create code 
 *for the binop node, creates code to pushe "%d" to the stack, to call printf
 *and to reinitialise the stack
 */
void
codegen_putint(struct s_tree *binop)
{
	char *string=new_label();
	char *label=new_label();
	
	printf("\tjmp .%s\n",label);
	printf(".%s: .string \"%%d\"\n",string);
	printf(".%s:\n",label);
	codegen(binop);	
	printf("\tpush $.%s\n",string);
	printf("\tcall printf\n");
	printf("\taddl $8,%%esp\n");
}
	
/*Creates code to push numbers to the stack*/
void
codegen_num(int Number)
{
	printf("\tpush $%d\n",Number);
}

/*Creates code to call a user created function*/
void
codegen_userfun(char *Variable)
{
	printf("\tcall %s\n",Variable);
}

/*Calls codegen to create code for the expression and to negate it*/
void
codegen_unop(struct s_tree *exp)
{
	codegen(exp);
	printf("\tpop %%eax\n");
	printf("\tneg %%eax\n");
	printf("\tpush %%eax\n");
}
	

/*Calls codegen twice in order to create code to push in the stack the
 *two operands, and then with the command "add", they are added.
 *The result, registers in register %eax
 */

  void 
codegen_add (struct s_tree *left,struct s_tree *right)
{	
	codegen(left);           
	codegen(right);          
	printf("\tpop %%eax \n ");
	printf("\tpop %%ebx \n" );
	printf("\tadd %%ebx,%%eax \n ");
	printf("\tpush %%eax \n ");
}


/*Calls codegen twice in order to create code to push to the stack
 *the two operands, and then the command "sub" is executed.
 *The result, registers in register %eax
 */

void 
codegen_sub (struct s_tree *left,struct s_tree *right)
{	
	codegen(left);               
	codegen(right);              
	printf("\tpop %%ebx \n ");
	printf("\tpop %%eax \n" );
	printf("\tsub %%ebx,%%eax \n ");
	printf("\tpush %%eax \n ");
}

/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b.
 *The "mul" command is used, in order to multiply operands. 
 *The command "mul %ebx", multiplies these two operands, and the result
 *registers in %eax
 */

void 
codegen_mul (struct s_tree *left,struct s_tree *right)
{	
	codegen(left);               
	codegen(right);              
	printf("\tpop %%eax \n ");
	printf("\tpop %%ebx \n" );
	printf("\tmul %%ebx \n ");   
	printf("\tpush %%eax \n ");
}

/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. The "cltd" command is used, in order to convert
 *%eax from a 32-bit word to a  64-bit. After that, it is separated
 *into two pieces, stored into %eax &%edx. After the division,
 *the result is stored in %eax. 
 */

void 
codegen_div (struct s_tree *left,struct s_tree *right)
{	
	codegen(left);                 
	codegen(right);                
	printf("\tpop %%ebx \n ");
	printf("\tpop %%eax \n" );
	printf("\tcltd\n");
	printf("\tdiv %%ebx \n ");   
	printf("\tpush %%eax \n ");
}             

/* Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. In the first part, the content or register %ebx, is
 *moved to register %eax, and after that, the content of %eax is stored to
 *the Variable
 */

void 
codegen_eq (struct s_tree *left,struct s_tree *right)
{	
	codegen(left);                
	codegen(right);               
	printf("\tpop %%ebx \n ");
	printf("\tpop %%eax \n" );
	printf("\tmov %%ebx,%%eax \n "); 
	printf("\tmov %%eax,%s\n",left->u.Variable);
}             

/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. These two, are compared. If they are equal, value $1
 *is pushed to the stack, otherwise a $0 is pushed.
 */
 
void 
codegen_eq_eq (struct s_tree *left,struct s_tree *right)
{	
	char *label1=new_label();
	char *label2=new_label();
	
	codegen(left);      
	codegen(right);     
	printf("\tpop %%eax \n ");
	printf("\tpop %%ebx \n" );
	printf("\tcmp %%ebx,%%eax \n ");  
	printf("\tje  .%s\n",label1);
	printf("\tpush $0 \n ");
	printf("\tjmp  .%s\n",label2);   
	printf(".%s: \n ",label1);
	printf("\tpush $1 \n ");
	printf(".%s: \n",label2);			
}


/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. These two, are compared. If they are equal, value $0
 *is pushed to the stack (that's what we want here), otherwise a $0 is pushed.
 */

void 
codegen_not_eq (struct s_tree *left,struct s_tree *right)
{	
	char *label1=new_label();
    char *label2=new_label();
	
	codegen(left);              
	codegen(right);             
	printf("\tpop %%eax \n ");
	printf("\tpop %%ebx \n" );
	printf("\tcmp %%ebx,%%eax \n ");
	printf("\tje .%s\n",label1);
	printf("\tpush $1 \n ");
	printf("\tjmp  .%s\n",label2);
	printf(".%s: \n ",label1);
	printf("\tpush $0 \n ");
	printf(".%s: \n",label2);
	
}

/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. These two, are compared. If %eax is greater than %ebx,
 *the command jg (Jump (if) Greater) is used, in order to push $1 to 
 *the stack, otherwise $0 is pushed into this
 */

void 
codegen_greater (struct s_tree *left,struct s_tree *right)
{	
	char *label1=new_label();
	char *label2=new_label();
	
	codegen(left);            
	codegen(right);           
	printf("\tpop %%eax \n");
	printf("\tpop %%ebx \n" );
	printf("\tcmp %%eax,%%ebx \n ");
 	printf("\tjg .%s\n",label1);
 	printf("\tpush $0 \n");
 	printf("\tjmp .%s\n",label2);
 	printf(".%s:\n",label1);
 	printf("\tpush $1\n");
 	printf(".%s:\n",label2);
 }


/*Calls codegen twice in order to create code to push to the stack the
 *two operands,a & b. These two, are compared. If %eax is greater than %ebx,
 *the command jl (Jump (if) Less) is used, in order to push $1 to 
 *the stack, otherwise $0 is pushed into this
 */

void 
codegen_less (struct s_tree *left,struct s_tree *right)
{	
    char *label1=new_label();
	char *label2=new_label();
	
	codegen(left);            
	codegen(right);           
	printf("\tpop %%eax \n ");
	printf("\tpop %%ebx \n" );
 	printf("\tcmp %%eax,%%ebx \n ");
 	printf("\tjl .%s\n",label1);
 	printf("\tpush $0 \n");
 	printf("\tjmp .%s\n",label2);
 	printf(".%s:\n",label1);                                                 
 	printf("\tpush $1\n");
 	printf(".%s:\n",label2);  
}

/*Creates index for the main function, from where the assembly code starts
 *execution and creates code to call aegean
 */
void 
codegen_main()
{
	printf("//Aegean C compiler\n//Made by aegean_force(4)\n//Have a lot of fun...\n\n");
	printf(".globl main\n");
	printf("\t.type\t main,@function\n");
	printf("main:\n");
	printf("\tcall aegean\n");
	printf("\tret\n");
}



	
	
	
	
	
	
	
	
	
	
	                                                
	
	





