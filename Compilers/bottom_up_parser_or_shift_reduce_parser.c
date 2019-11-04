/*

Bottom up/Shift reduce parser

Grammer
E->E+T		1
E->T		2
T->i		3

Parsing Table
S		+	i	$		E	T
0			S3			1	2
1		S4		AC
2		R2	R2	R2
3		R3	R3	R3
4			S3				5
5		R1	R1	R1

Test Cases:
i$
i+i$
i+i+i$
i+i+i+i$
i+i+i+i+i$

Algorithm followed:
1. Push intial state to stack.
2. if(action[state, input tape symbol]==acc)then
		Parsing is successful and stop #acc stands for accept
2. if(action[state, input tape symbol]==blank)then
		Parsing is unsuccessful and stop.
4. if(action[state, input tape symbol]==Si)then
		perform shift operation and goto step 2.
5. if(action[state, input tape symbol]==ri)then
		let i represents production number A->B
			i) Pop out 2*|B| number of symbols from stack
			ii) Push symbol A onto stack
			iii) if(Sm is state below A in Stack) then
					push(goto(Sm,A)) onto stack.
			iiii) goto step 2.
*/

/*Redefine these values as per size of your input and parsing table*/
#define MAX_SIZE 150
#define ROWS 6
#define COLUMNS 5
#include<stdio.h>
#include<string.h>

char stack[MAX_SIZE*2];						//Stack
int sp=-1;							//Stack pointer
char input[MAX_SIZE];						//Input tape
char *ip = input;						//Input pointer
void push(char ip); 						//Push function stack
void pop();							//Pop function stack
char top();							//Top function stack
int getParsingTableColumnIndex(char ip);		 	//Parsing table column index
void parse();							//Parse function stack
void shift(char *);						//Shift function
void reduce(char *);						//Reduce function
char *getAction(int state, char t);				//Get action table entry
char *getGoto(int state, char v);				//Get goto table entry
char *productionReader(int prodno); 				//Returns RHS of production
char getProductionSymbol(int prodno); 				//Return symbol of production as per production number
int toNum(char i); 						//converts char to num
void status(); 							//Check parser status

/*Grammer data and parsing table*/
char columns[6] = {'+', 'i', '$', 'E', 'T'}; 			//Table columns
char *productions[3] = {"E->E+T", "E->T", "T->i"}; 		//Productions
char *parsingtable[ROWS][COLUMNS]				//Parsing table
		=
		{
			/*
					Action				 	Goto
			+			i			$			E			T
			*/
			{"",			"S3",			"",			"1",			"2"},
			{"S4",			"",			"acc",			"",			""},
			{"r2",			"r2",			"r2",			"",			""},
			{"r3",			"r3",			"r3",			"",			""},
			{"",			"S3",			"",			"",			"5"},
			{"r1",			"r1",			"r1",			"",			""}
		};
							
int toNum(char i)							//Reads a character and returns equivalent int value
{
	return (int)(i-'0');
}

int getParsingTableColumnIndex(char in)					//Reads a column symbol and returns column index
{
	int i=0;
	for(;i<COLUMNS; i++)
	{
		if(columns[i]==in)
		{
			break;
		}
	}
		return i;
}

char *getAction(int state, char t)					//Reads and returns action table entry t€T{Terminal Set}
{
	
	return parsingtable[state][getParsingTableColumnIndex(t)];
}

char *getGoto(int state, char v)					//Reads and returns goto table entry v€V{Variable Set}
{
	
	return parsingtable[state][getParsingTableColumnIndex(v)];
}

void push(char ip)							//Pushes symbol to stack
{
	if(sp==(MAX_SIZE*2)-1)
	{
		puts("Stack overflow");
	}
	else
	{
		++sp;
		stack[sp]=ip;
	}
}

void pop()								//Pop out symbol from stack
{
	if(sp==-1)
	{
		puts("Stack underflow");
	}
	else
	{
		stack[sp]='\0';
		--sp;
	}
}

char top()								//Reads and returns top of the stack
{
	return stack[sp];
}

void parse()								//Parser - Read the algorithm above to understand function
{
	status();
	int state = toNum(top());
	char *tableentry = getAction(state, *ip);
	if(tableentry=="acc")
	{
		puts("\tParsing Success");
	}
	if(tableentry[0]=='S')
	{
		shift(tableentry);
		printf("\t%s\n", tableentry);
		parse();
	}
	if(getAction(state, *ip)[0]=='r')
	{
		reduce(tableentry);
		printf("\t%s\n", tableentry);
		parse();
	}
	if(tableentry=="")
	{
		puts("\tParsing failed");
	}
}

void shift(char *in)							//Shift method
{
	char currentstate=in[1];
	push(*ip);
	push(currentstate);
	ip++;
}

void reduce(char *in)			//Reduce method
{
	char *prod=productionReader(toNum(in[1]));
	int countPoped = 2*strlen(prod);
	int i=0;
	for(;i<countPoped;i++)
	{
		pop();
	}
	push(getProductionSymbol(toNum(in[1])));
	char prevState=stack[sp-1];
	push(getGoto(toNum(prevState),top())[0]);
}

char *productionReader(int prodno)					//Reads and returns right hand side of production
{
	char *P=productions[prodno-1];
	return (P+3);
}

char getProductionSymbol(int prodno)					//Reads and returns left hand side of a production
{
	char *P=productions[prodno-1];
	return P[0];
}

void status()								//Print stack inputstring and action status on screen
{
	printf("%s\t%s", stack, ip);
}

int main()								//Driver method
{
	puts("Enter your input string: ");
	scanf("%s", &input);
	push('0');							//Initialy stack will be empty we need to push initial state at first before start parsing
	printf("Stack\tInput\tAction\n");
	parse();
	printf("=============================================================\n");
}
