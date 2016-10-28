#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
        char *c;
        struct list *next; 
};

typedef  struct list *lista;
struct list *var_head,*fun_head;
lista tmp;
char *character1;
int search_mark;
 
lista var_list_new(void)
{       lista m=(lista)malloc(sizeof(struct list));
        var_head=NULL;
        return(m);
}

lista fun_list_new(void)
{       lista s=(lista)malloc(sizeof(struct list));
        fun_head=NULL;
        return(s);
}

int m_search_mark(lista m, char *i)
{		search_mark=1;
		for (m=var_head; m; m=m->next)
		{	if (strcmp(m->c,i)==0)
				 search_mark=0;
		}
		return(search_mark);
}

int s_search_mark(lista s, char *i)
{		search_mark=1;
		for (s=fun_head; s; s=s->next)
		{	if (strcmp(s->c,i)==0)
				 search_mark=0;
		}
		return(search_mark);
}

lista m_list_add(lista m, char *i)
{       lista tmp;
		tmp=var_head;
		search_mark=m_search_mark(m,i);
		if (search_mark==0)
			fprintf(stderr," Warning:Redefinition error: two var_names with the same value!!!\n");
		else
		{		
			var_head=tmp;
		
			if(var_head==NULL)    
			{       m->c=i;
				    m->next=var_head;
			        var_head=m;
                    return(m);
			}
            else
			{       m=(lista)malloc(sizeof(struct list));
                    m->c=i;
				    m->next=var_head;
                    var_head=m;
                    return(m);
			}
		}
}

lista s_list_add(lista s, char *i)
{       lista tmp;
		tmp=fun_head;
		search_mark=s_search_mark(s,i);
		if (search_mark==0)
			fprintf(stderr," Warning:Redefinition error: two function_names with the same value!!!\n");
			
		else
		{
			fun_head=tmp;
		
			if(fun_head==NULL)    
			{       s->c=i;
				    s->next=fun_head;
					fun_head=s;
	                return(s);
		    }
		    else
		    {       s=(lista)malloc(sizeof(struct list));
			        s->c=i;
				    s->next=fun_head;
					fun_head=s;
				    return(s);
			}
		}
}

void
m_search_results(lista s, char *i)
{
	if (m_search_mark(s,i)==1)
	{
		fprintf(stderr,"ERROR: Non declared variable %s",i);
		exit(1);
	}
}

void
s_search_results(lista s, char *i)
{
	if (s_search_mark(s,i)==1)
	{
		fprintf(stderr,"ERROR: Non declared function %s",i);
		exit(1);
	}
}	













