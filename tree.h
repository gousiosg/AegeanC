/*tree.h :syntax tree definitions*/

/*All the possible types of the tree nodes*/
enum nodetypes
{
	MYFILE,/*name of the root of the tree*/
	PROG,   
	MAKEFUN,/*a user function definition node*/
	INT,    /*a variable definition node */
	WHILE,
	IFELSE,
	COMMANDS,/*a node with pointers to all possible commands*/ 
	PUTSTR,
	PUTINT,
	GETINT,
	USERFUN,
	BINOP, /*a node for all binary operations*/
	CMP,   /*a node for comparisons*/
	UNOP,
	VAR,
	NUM
};

struct s_tree
{
	enum nodetypes nt;
	union
	{
		struct s_tree *Program;/*points to the rest of the program*/
		char *Variable;/*used for constructing a variable node*/
		char *String;  /*used for constucting a putstring node*/
		int Number;    /*used for constructing a getint node*/
		struct s_prog  
		{
			struct s_tree *variable;
			struct s_tree *makefun;
			struct s_tree *prog;/*to contruct more PROG nodes*/ 
		}Prog;
		struct s_makefun    /*User function definition node*/
		{
			struct s_tree *commands;
			char *name;
		}Makefun;
		struct s_while
		{
			struct s_tree *cmp;
			struct s_tree *commands;
		}While;
		struct s_ifelse
		{
			struct s_tree *cmp;
			struct s_tree *commands;
			struct s_tree *else_commands;
		}Ifelse;
		struct s_commands   
		{
			struct s_tree *command;
			struct s_tree *commands;
		}Commands;
		struct s_putint
		{
			struct s_tree *binop;
		}Putint;
		struct s_binop
		{
			struct s_tree *left;
			struct s_tree *right;
			char op;
		}Binop;
		struct s_cmp
		{
			struct s_tree *left;
			struct s_tree *right;
			char *op;
		}Cmp;
		struct s_unop
		{
			struct s_tree *exp;
			char op;
		}Unop;
	}u;
};