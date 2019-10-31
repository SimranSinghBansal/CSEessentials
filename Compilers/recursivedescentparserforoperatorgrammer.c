/*
Original grammer:
E->E+T|E-T|T
T->T*M|T/M|M
M->id

After removing left recursion:
E->TE'
E'->+TE'|-TE'|€
T->MT'
T'->*MT'|/MT'|€
M->id

After minimizing the grammer:
E->TE'
E'->+TE'|-TE'|€
T->idT'
T'->*idT'|/idT'|€

Test with following examples:
id$
id+id$
id+id*id$
id+id-id/id$
*/

#include<stdio.h>
#define MAX_SIZE 100

char str[MAX_SIZE]; /*Input string*/
char *l=str; /*look ahead symbol*/
int flag=1; /*String validation flag*/

/*Matcher function to match lookahead symbol with grammer symbol*/
void match(char ip)
{
	if(*l==ip)
	{
		l++;
		flag=0;
	}
	else
	{
		flag=1;
	}
}

void Td()
{
	if(*l=='*')
	{
		match('*');
		match('i');
		match('d');
		Td();
	}
	if(*l=='/')
	{
		match('/');
		match('i');
		match('d');
		Td();
	}
}

void T()
{
	if((*l=='i')&&(*(l+1)=='d'))
	{
		match('i');
		match('d');
		Td();		
	}
	else
	{
		flag=1;
	}
}

void Ed()
{
	if(*l=='+')
	{
		match('+');
		T();
		Ed();
	}
	if(*l=='-')
	{
		match('-');
		T();
		Ed();
	}
}

void E()
{
	T();
	if(flag==0)
	Ed();
}

int main()
{
	printf("Enter your string: ");
	scanf("%s", &str);
	E();
	if(*l=='$'&&flag==0)
	{
		printf("String parsed successfully :-)");
	}
	else
	{
		printf("String is not parsed successfully :-(");
	}
	printf("\n===================================================\n");
}
