
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lexer.h"
char non_terminals[100][100];
char t_nt[100][100];
int len = 0;
int count_t_nt= 2;
int count_nt=0;
int count_t;
int count_each_nt[100];
int count_rhs=0;
int count_sym;
int rhs[100];
int changed = 0;
int changed_follow = 0;
int no_of_sym;
int no_of_t = 2; 
int first_set[100][100];
int follow_set[100][100];
char sym_table[100][100];
static char *reservedc[] =
    {   
        "#",
        "$",
    };

int flag_hash = 0;
int compare_strings(char to_compare[], char (*compared_with)[100], int n);
void parse_S();
void parse_R();
void parse_N();
void parse_RL();
void parse_I(int flag);
void parse_RH();
void first_create();
void follow_create();
void add_minus_epsilon(int s1, int s2);
int is_epsilon_in(int s);
void display_first_set();
void display_follow_set();
int is_non_terminal(int s);
int is_terminal(int s);
void follow_add_minus_epsilon(int s1, int s2);
void ff_add_minus_epsilon(int s1, int s2);


struct linked_list
{
    int LHS;
    int RHS[100];
    int size;
    struct linked_list *next;
} ;
typedef struct linked_list NODE;

NODE * getnode();
NODE * insert_at_beg(NODE *head, int list_lhs, int list_rhs[], int list_rhs_count);
NODE * find(NODE *head, char *a);
void display(NODE *head);
NODE *head = NULL;

void process_rules(NODE *head);
void follow_process_rules(NODE *head);
int main (int argc, char* argv[])
{
	int task;
	if (argc < 2) {
          printf("Error: missing argument\n");
		return 1; }
	task = atoi(argv[1]);

	int k;
	int x=0, y=1;
	//Initializing Count of Non Terminals to 0!
	for(k = 0; k< 100; k++)
	{
		count_each_nt[k] = 0;
	}
	strcpy(t_nt[x], reservedc[x]);
	strcpy(t_nt[y], reservedc[y]);
    //Parsing the Grammar 
	parse_S();
	//Displaying Non Terminals with no. of Occurence!
	for(int i =0;i<count_nt; i++){
		//puts(non_terminals[i]);
		//printf("%d\n",count_each_nt[i] );
	}
	//Copying all Terminals and Non Terminals to Symbol Table
	int count_sym=2;
	strcpy(sym_table[x], reservedc[x]);
	strcpy(sym_table[y], reservedc[y]);

	for (int i = count_nt+2; i < count_t_nt; ++i)
	{
		strcpy(sym_table[count_sym++], t_nt[i]);
	}
	for (int i = 0; i < count_nt; ++i)
	{
		strcpy(sym_table[count_sym++], non_terminals[i]);
	}
	//Displaying Symbol Table
	printf("\nSymbol Table\n");
	for(int i =0;i<count_sym; i++){
		puts(sym_table[i]);
		
	}
	//printf("\nT_NT table\n");
	for(int i =0;i<count_t_nt; i++){
		//puts(t_nt[i]);
		
	}
	display(head);
	//First Sets Begins!
	no_of_sym = count_sym;
	no_of_t = count_t;
	first_create();
	follow_create();
	//printf("\nFirst Sets are here\n");
	
	
	switch (task) {
          case 0:
              for (int i = 2; i < count_t+2; ++i)
              {
              	 printf("%s ", sym_table[i]);
              }
              printf("\n");
              for (int i = 0; i < count_nt; ++i)
              {
              	 printf("%s:",non_terminals[i] );
              	 printf("%d\n",count_each_nt[i] );
              }
			break;
          case 1: display_first_set();
              // TODO: Calculate FIRST sets for the input grammar
              // Hint: You better do the calculation in a function and call it here!
              // TODO: Output the FIRST sets in the exact order and format required
              break;
          case 2: display_follow_set();
              // TODO: Calculate FIRST sets for the input grammar
              // TODO: Calculate FOLLOW sets for the input grammar
              // TODO: Output the FOLLOW sets in the exact order and format required
              break;
          default:
              printf("Error: unrecognized task number %d\n", task);
              break;
}
	return 0;
}

void success()
{
	//printf("\nCorrect Grammar\n");
}

void error()
{
	//printf("\nIncorrect Grammar\n");
}


void error2(char a[])
{
	//puts(a);
}

//Grammars Deciphered!
void parse_S()
{
	//printf("\nInside S\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		ungetToken();
		parse_N();
		parse_RL();
		ttype = getToken();
		if(ttype == DOUBLEHASH)
		{
			ttype = getToken();
			if(ttype == EOF)
			success();
			else error();
		}
		else error();
	}
	else error();
}

void parse_N()
{
	//printf("\nInside N\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		ungetToken();
		parse_I(0);

		ttype = getToken();
		if(ttype == HASH){
			success();

			for (int i = 0; i < len; ++i)
			{
				//puts(non_terminals[i]);
				count_nt++;
			}
		}
		else error2("\nNerror-1");
	}
	else error();

}

void parse_I(int flag)
{
	int n,m;
	//printf("\nInside I\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		n = compare_strings(current_token, t_nt, count_t_nt);
		if(n==-1){
			strcpy(t_nt[count_t_nt++], current_token);
		}
		if(flag == 1)
		{
			//printf("\nParsing I called from Rh and inreasing rhs count\n");
			m = compare_strings(current_token, t_nt, count_t_nt);
			//printf("\n%d", count_rhs);
			rhs[count_rhs++] = m;

		}
		strcpy(non_terminals[len++],current_token);
		//printf("\nID found\n");
		//ttype = getToken();
		
		parse_I(flag);
	
	}
	else if(ttype == HASH)
		{
			//printf("\nHASH Found\n");
			ungetToken();
			success();
		}
	else error2("\nError_I-2");
}

void parse_RL()
{

	//printf("\nInside RL\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		ungetToken();
		parse_R();
		//ttype = getToken();
		//if(ttype == ID)
			parse_RL();
		
		//else error2("Error_RL-1");
	}
	else if (ttype == DOUBLEHASH){
			//printf("\nDouble HASH\n");
			ungetToken();
			success();}
	else error2("Error_RL-2");
}

void parse_R()
{
	flag_hash = 0;
	int i;
	int n,t_nt_location;
	int lhs=0;
	//int l_count, r_count;
	for(i=0; i<count_t_nt; i++)
		rhs[i]=0;
	count_rhs = 0;
	//printf("\nInside R\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		n = compare_strings(current_token, non_terminals, count_nt);
		if( n>-1 ){
			count_each_nt[n]++;
		}
		t_nt_location = compare_strings(current_token, t_nt, count_t_nt);
		lhs = t_nt_location; 
		ttype = getToken();
		if(ttype == ARROW)
		{
			ttype = getToken();
			if(ttype == ID)
			{
				flag_hash = 1;
				ungetToken();
				parse_RH();
				head = insert_at_beg(head, lhs, rhs, count_rhs);
				ttype = getToken();
				/*if (ttype == HASH){
					success();
					printf("\nHash in parse_R\n");
				}
				else error();
				*/
			}
			else if (ttype == HASH){

					if(flag_hash == 0)
					{
						rhs[count_rhs++] = 0;
						head = insert_at_beg(head, lhs, rhs, count_rhs);
					}
					success();
					//printf("\nHash in parse_R\n");

				}
			else error();
		}
		else error();	
	}
	else error();
}

void parse_RH()
{
	//rintf("\nInside RH\n");
	token_type ttype = getToken();
	if(ttype == ID)
	{
		ungetToken();
		parse_I(1);
		success();
	}
	else if(ttype == HASH)
	{
		
		ungetToken();
		success();
	}
	else error();

}

//Looking for known terminals and non terminals!
int compare_strings(char to_compare[], char (*compared_with)[100], int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		if(strcmp(to_compare, compared_with[i])==0)
			return i;

	}
 return -1;
}


//The Linked List related functions!
NODE * getnode()
{
    NODE *create;
    create = (NODE *)(malloc(sizeof(NODE)));
    create->next = NULL;
    return create;
}

NODE * insert_at_beg(NODE *head, int list_lhs, int list_rhs[], int list_rhs_count)
{
    NODE *makenode;
    makenode = getnode();
    int i;

    if(head == NULL)
    {
        makenode->LHS = list_lhs;
        for (i = 0; i < count_rhs; i++)
        {
        	makenode->RHS[i] = list_rhs[i];
        }
        makenode->size = list_rhs_count;
        head = makenode;
        //printf("\n1st insert success\n");
    }
    else 
    {
       	makenode->LHS = list_lhs;
        for (i = 0; i < count_rhs; i++)
        {
        	makenode->RHS[i] = list_rhs[i];
        }
        makenode->size = list_rhs_count;
        makenode->next = head;
        head = makenode;
        //printf("\nFurther insert success\n");
    }
    return head;
        
}


void display(NODE *head)
{
    //char temp[MAX_TOKEN_LENGTH];
    NODE *n;
    n = head;
    count_t = count_t_nt-2 - count_nt;
    char token[100];
    if(n == NULL)
    {
    //  printf("\n List is Empty\n");
        return;
    }
    while(n!=NULL)
	{
		if(n->next == NULL)
		{
			n->LHS = (count_t)+n->LHS;
			printf("LHS %d\nRHS\n",n->LHS);
			for (int i = 0; i < n->size; i++)
			{
				strcpy(token, t_nt[n->RHS[i]]);
				
				if(n->RHS[i] == 0)
				{
					printf("%d\n",n->RHS[i]);
					break;
				}
				else if(compare_strings(token, non_terminals, count_nt) !=-1)
					n->RHS[i] = n->RHS[i] + count_t ;
				else
					n->RHS[i] = n->RHS[i]  - count_nt;
				printf("%d\n",n->RHS[i]);
			}
			//printf("Size - %d\n",n->size );
			
            
			//printf("\n1st Point\n");
			break;
		}


		else
		{
			
			n->LHS = (count_t)+n->LHS;
			printf("LHS %s\nRHS\n",sym_table[n->LHS]);
			for (int i = 0; i < n->size; i++)
			{
				strcpy(token, t_nt[n->RHS[i]]);
				if(n->RHS[i] == 0)
				{
					printf("%s, %d\n",sym_table[n->RHS[i]], n->RHS[i]);
					break;
				}
				else if(compare_strings(token, non_terminals, count_nt) !=-1)
					n->RHS[i] = n->RHS[i] + count_t ;
				else
					n->RHS[i] = n->RHS[i]  - count_nt;
				printf("%s, %d\n",sym_table[n->RHS[i]], n->RHS[i]);
			}
			//printf("Size - %d\n",n->size );
			n = n->next;
			//printf("\n2nd Point");
		}
	}
    
}

//First set Creations!!!
void first_create()
{
	
	for (int i = 0; i < no_of_sym; i++)
	{
		for (int j = 0; j < no_of_t+2; j++)
		{
			first_set[i][j] = 0;
		}
	}
	for(int i=0; i < no_of_t+2; i++)
		first_set[i][i] = 1;

	changed = 1;
	while(changed)
	{
		changed = 0;
		process_rules(head);
	}


}

void add_minus_epsilon(int s1, int s2)
{
	printf("Inside add_m_epsilon for %s and %s\n", sym_table[s1],sym_table[s2] );
	
	for (int i = 2; i < no_of_t+2; ++i)
	{
		if(first_set[s1][i] == 0)
		{
			if(first_set[s2][i] == 1)
			{
				first_set[s1][i] = 1;
				changed = 1;
			}
		}
	}
}

int is_epsilon_in(int s)
{
	return first_set[s][0];
}

void process_rules(NODE *head)
{
	//printf("\nInside Process Rules\n");
	NODE *n;
	n = head; 
	//int i;
		if(n == NULL)
		return;
	while(n!= NULL)
	{

		if(n->next == NULL)
		{
			int flag_ep=0;

			if(n->RHS[0] == 0)
			{
				first_set[n->LHS][0] =1;
			}
			else
			{
				for(int i=0; i < n->size; i++)
				{
					add_minus_epsilon(n->LHS, n->RHS[i]);
					if(!is_epsilon_in(n->RHS[i]))
					break;
				}
				for(int i=0; i < n->size; i++)
				{
					if(is_non_terminal(n->RHS[i]))
					if(first_set[n->RHS[i]][0]==0)
						flag_ep = 1;
				}
				if(flag_ep == 0){
					if(first_set[n->LHS][0] == 0){
					first_set[n->LHS][0] = 1;
					changed =1;
				}
			}	
			
			break;
		}
		else
		{
			int flag_ep=0;

			if(n->RHS[0] == 0)
			{
				first_set[n->LHS][0] =1;
			}
			else
			{
				for(int i=0; i < n->size; i++)
				{
					add_minus_epsilon(n->LHS, n->RHS[i]);
					if(!is_epsilon_in(n->RHS[i]))
					break;
				}
				for(int i=0; i < n->size; i++)
				{
					//if(is_non_terminal(n->RHS[i]))
					if(first_set[n->RHS[i]][0]==0)
						flag_ep = 1;
				}
				if(flag_ep == 0)
				{
					if(first_set[n->LHS][0] == 0){
					first_set[n->LHS][0] = 1;
					printf("Found it\n");
					changed =1;
					}
				}
			}	

			

			n = n->next;
		}
	}
}

void display_first_set()
{
	
	//printf("Inside display 1st set\n");
	for (int i = no_of_t+2; i < no_of_sym; ++i)
	{
		char sorted_symbols[100][100];
		char t[100];
		int k=0;
		printf("FIRST(%s) = {", sym_table[i]);
		for (int j = 0; j < no_of_t+2; ++j)
		{
			if(first_set[i][j] == 1)
				strcpy(sorted_symbols[k++], sym_table[j]);
				//puts(sym_table[j]); 
		}
		for (int i = 1; i < k; i++) {
      		for (int j = 1; j < k; j++) {
         		if (strcmp(sorted_symbols[j - 1], sorted_symbols[j]) > 0) {
            		strcpy(t, sorted_symbols[j - 1]);
            		strcpy(sorted_symbols[j - 1], sorted_symbols[j]);
            		strcpy(sorted_symbols[j], t);
         		}
      		}
   		}
   		for (int i = 0; i < k; ++i)
   		{
   			if(i==0)
   			printf("%s", sorted_symbols[i] );
   			else
   			printf(", %s", sorted_symbols[i]);
   		}
   		printf("}\n" );
	}


}

int is_non_terminal(int s)
{
	char token[100];
	strcpy(token, sym_table[s]);
	if(compare_strings(token, non_terminals, count_nt)!=-1)
		return 1;
	else return 0;
}
int is_terminal(int s)
{
	int i;
	for(i=2; i<no_of_t+2; i++)
	{
		if(strcmp(sym_table[i], sym_table[s])==0)
			return 1;
		else 
			return 0;

	}
}

void follow_add_minus_epsilon(int s1, int s2)
{
	for (int i = 1; i < no_of_t+2; ++i)
	{
		if(follow_set[s1][i] == 0)
		{
			if(follow_set[s2][i] == 1)
			{
				follow_set[s1][i] = 1;
				changed_follow = 1;
			}
		}
	}
}

void ff_add_minus_epsilon(int s1, int s2)
{
	//printf("\nInside FF %d %d \n", s1, s2);
	for (int i = 1; i < no_of_t+2; ++i)
	{
		if(follow_set[s1][i] == 0)
		{
			if(first_set[s2][i] == 1)
			{
				follow_set[s1][i] = 1;
				changed_follow = 1;
			}
		}
	}
}

void follow_create()
{
	for (int i = no_of_t+2; i < no_of_sym; i++)
	{
		for (int j = 0; j < no_of_t+2; j++)
		{
			follow_set[i][j] = 0;
		}
	}
	follow_set[no_of_t+2][1] = 1;

	changed_follow = 1;
	while(changed_follow)
	{
		changed_follow = 0;
		follow_process_rules(head);
	}
}

void follow_process_rules(NODE *head)
{
	NODE *n;
	n = head; 
	int i,k;
	if(n == NULL)
		return;
	while(n!= NULL)
	{
		if(n->next == NULL)
		{
			i = n->size-1;
			while(is_non_terminal(n->RHS[i]) || i!=0 )
			{
				follow_add_minus_epsilon(n->RHS[i], n->LHS);
				if(!is_epsilon_in(n->RHS[i]))
					break;
				i--;
			}
			for (int i = 0; i < n->size; ++i)
			{
				if(is_non_terminal(n->RHS[i]))
				{
					k = i + 1;
					while(k<=n->size-1)
					{
						//if(is_non_terminal(n->RHS[k]))
						ff_add_minus_epsilon(n->RHS[i], n->RHS[k]);
						if(!is_epsilon_in(n->RHS[k]))
							break;
						if(is_terminal(n->RHS[k]))
							break;
						k++;
					}
				}
			}
			break;
		}
		else
		{

			i = n->size-1;
			while(is_non_terminal(n->RHS[i]) || i!=0 )
			{
				follow_add_minus_epsilon(n->RHS[i], n->LHS);
				if(!is_epsilon_in(n->RHS[i]))
					break;
				i--;
			}
			for (int i = 0; i < n->size; ++i)
			{
				if(n->RHS[0]==1)
					break;
				
				if(is_non_terminal(n->RHS[i]))
				{
					//printf("Is non terminal %d\n", n->RHS[i] );
					k = i + 1;
					while(k<=n->size-1)
					{

						//if(is_non_terminal(n->RHS[k]))
						ff_add_minus_epsilon(n->RHS[i], n->RHS[k]);
						if(!is_epsilon_in(n->RHS[k]))
							break;
						if(is_terminal(n->RHS[k]))
							break;
						k++;
					}
				}
			}
			n = n->next;	
		}
	}	 
}

void display_follow_set()
{
	for (int i = no_of_t+2; i < no_of_sym; ++i)
	{
		char sorted_symbols[100][100];
		char t[100];
		int k=0;

		printf("FOLLOW(%s) = {", sym_table[i]);
		//puts(sym_table[i]);
		for (int j = 0; j < no_of_t+2; ++j)
		{
			if(follow_set[i][j] == 1)
				//puts(sym_table[j]);
				strcpy(sorted_symbols[k++], sym_table[j]);
		}
		for (int i = 1; i < k; i++) {
      		for (int j = 1; j < k; j++) {
         		if (strcmp(sorted_symbols[j - 1], sorted_symbols[j]) > 0) {
            		strcpy(t, sorted_symbols[j - 1]);
            		strcpy(sorted_symbols[j - 1], sorted_symbols[j]);
            		strcpy(sorted_symbols[j], t);
         		}
      		}
      	
   		}
   		for (int i = 0; i < k; ++i)
   		{
   			if(i==0)
   			printf("%s", sorted_symbols[i] );
   			else
   			printf(", %s", sorted_symbols[i]);
   		}
   		printf("}\n" );
	}
}

NODE * find(NODE *head, char *a)
{
    return head;
}









