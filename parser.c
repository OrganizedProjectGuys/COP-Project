/*
 *	Authors...: Christopher Hayden & Zach Mace
 *	Class.....: COP3402
 *  Assignment: HW_3 (Parser-Code Generator)
 *  Due Date..: Jun 15th 11:59pm
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define CONSTANT 1
#define VARIABLE 2
#define PROCEDURE 3
#define FACTOR 4

symbol *table;
int sym_index, input_index, inputLength;
int error;
int lexlevel;

lexeme *globallist;
lexeme token;

void printtable();
void errorend(int x);

void program();
void block();
void constantDeclaration();
void variableDeclaration();
void procedureDeclaration();
void statement();
void condition();
void relop();
void mathop();
void expression();
void term();
void factor();
void get();

void checkSymbols();
void addSymbol(int kind, char *name, int level, int addr);
void validSymbol(int kind);


symbol *parse(lexeme *input)
{
	table = malloc(1000 * sizeof(symbol));
	sym_index = 0;
	input_index = 0;
	error = 0;
  inputLength = 0;
	lexlevel = 0;
  int i = 0;

	globallist = input;

  while (input[i].type != '\0')
  {
    inputLength++;
    i++;
  }
	inputLength-=2;

	addSymbol(PROCEDURE, "main", 0, 0);	//Add main to the symbol table.
	table[sym_index].val = 0;
	sym_index++;

  program(inputLength);	//Call Program.

	if (error)
	{
		free(table);
		return NULL;
	}
	else
	{
		printtable();
		return table;
	}
}

//Get the next token in the input list.
void get()
{
	token = globallist[input_index];
	input_index++;
}

void program(int inputLength)
{
	block();
	if (token.type != periodsym)	//Make sure the program ends with a '.'
		errorend(3);
}

void block()
{
	constantDeclaration();
	variableDeclaration();
	procedureDeclaration();
	statement();
}

void constantDeclaration()
{
	get();
	if (token.type == constsym)
	{
		do
		{
			get();
			if (token.type != identsym)	//Make sure the constant is given an identifier.
				errorend(4);

			checkSymbols();	//Check conflicting symbols.
			addSymbol(CONSTANT, token.name, lexlevel, 0);	//Add the constant to the symbol table.

			get();
			if (token.type != becomessym)	//Make sure the constant is given a value.
				errorend(5);

			get();
			if (token.type != numbersym)	//Make sure the value is present.
				errorend(5);

			table[sym_index].val = token.value;	//Assign the constant its value.
			sym_index++;

			get();
		}	while (token.type == commasym);

	if (token.type != semicolonsym)	//Make sure the declaration ends in a semicolon.
		errorend(6);
	get();
	}
}

void variableDeclaration()
{
	if (token.type == varsym)
	{
		int numvars = 0;
		do
		{
			get();
			if (token.type != identsym)	//Make sure the constant is given an identifier.
				errorend(4);

			checkSymbols();	//Check conflicting symbols.
			addSymbol(VARIABLE, token.name, lexlevel, numvars + 3);		//Add the variable to the symbol table.
			numvars++;	//Increment the number of variables.
			table[sym_index].val = 0;	//Initialize the variable.
			sym_index++;

			get();
		} while(token.type == commasym);

	if (token.type != semicolonsym)	//Make sure the declaration ends in a semicolon.
		errorend(6);
	get();
	}
}

void procedureDeclaration()
{
	if (token.type == procsym)
	{
			get();
			if (token.type != identsym)	//Make sure the procedure is given an identifier.
				errorend(4);

			checkSymbols();	//Check conflicting symbols.
			addSymbol(3, token.name, lexlevel, 0);	//Add the procedure to the symbol table.
			table[sym_index].val = 0;
			sym_index++;

			get();
			if (token.type != semicolonsym)	//Make sure the procedure declaration ends in a semicolon.
				errorend(6);

			lexlevel++;	//Increment the lex level and call block.
			block();

			if (token.type != semicolonsym)	//Make sure the procedure ends in a semicolon.
				errorend(6);

			get();
			procedureDeclaration();

	}
}

void statement()
{
	if (token.type == identsym)
	{
		validSymbol(VARIABLE);	//See if its in symbol table and is a var.

		get();
		if (token.type != becomessym)	//Make sure the variable is becoming something.
			errorend(2);

		get();
		expression();
		get();
		return;
	}

	if (token.type == callsym)
	{
		get();
		if (token.type != identsym)	//Make sure an identifier is being called.
			errorend(14);

		validSymbol(PROCEDURE);	//See if its in symbol table and is a procedure.

		get();
		return;
	}

	if (token.type == beginsym)
	{
		do
		{
			get();
			statement();
		}	while (token.type == semicolonsym);

		while (token.type == semicolonsym)	//Buffer if there are additional semicolons.
			get();
		statement();	//Recall statement with the new variable.

		if (token.type != endsym)	//Make sure begin has an end.
			errorend(10);

		get();
		return;
	}

	if (token.type == ifsym)
	{
		get();
		condition();

		if (token.type != thensym)	//Make sure if has a then.
			errorend(9);

		get();
		statement();
		if (token.type == elsesym)	//Check for else.
			statement();
		return;
	}

	if (token.type == whilesym)
	{
		get();
		condition();

		if (token.type != dosym)	//Make sure while has a do.
			errorend(8);

		get();
		statement();
		return;
	}

	if (token.type == readsym)
	{
		get();
		if (token.type != identsym)	//Make sure read is followed by an identifier.
			errorend(14);

		validSymbol(VARIABLE);	//Check if the identifier is in the symbol table.

		get();
		return;
	}

	if (token.type == writesym)
	{
		get();
		expression();
		return;
	}

}

void condition()
{
	if (token.type == oddsym)
	{
		get();
		expression();
	}
	else
	{
		expression();
		relop();
		get();
		expression();
	}
}

void relop()	//Function that checks all of the relational operators.
{
	if (token.type != eqlsym && token.type != neqsym && token.type != lessym &&
		 token.type != leqsym && token.type != gtrsym && token.type != geqsym)
		 errorend(12);
}

void mathop()	//Function that checks if a number is followed by a valid symbol.
{
	if (token.type != plussym && token.type != minussym && token.type != multsym &&
			token.type != slashsym && token.type != modsym && token.type != semicolonsym &&
			token.type != identsym)
			errorend(2);
}

void expression()
{
	if (token.type == plussym || token.type == minussym)
		get();

	term();

	while (token.type == plussym || token.type == minussym)
	{
		get();
		term();
	}
}

void term()
{
	factor();
	while (token.type == multsym || token.type == slashsym || token.type == modsym)
	{
		get();
		factor();
	}
}

void factor()
{
	if (token.type == identsym)
	{
		validSymbol(FACTOR);
		get();
	}

	else if (token.type == numbersym)
	{
		get();
		mathop();	//Make sure numbersym is followed by a valid symbol.
	}

	else if (token.type == lparentsym)
	{
		get();
		expression();
		if (token.type != rparentsym)	//Make sure the lparent has an rparent to follow.
			errorend(13);
		get();
	}

	else	//Called factor without a condition.
		errorend(11);
}

void checkSymbols()	//Check if we can add the symbol to the table without conflict.
{
	int i;

	for (i = 0; i < sym_index; i++)
	{
		if (!strcmp(token.name, table[i].name) && (table[i].level >= lexlevel))
			errorend(1);
	}
}

void addSymbol(int kind, char *name, int level, int addr)	//Add symbol to the table.
{
	table[sym_index].kind = kind;
	strcpy(table[sym_index].name, name);
	table[sym_index].level = level;
	table[sym_index].addr = addr;
}

void validSymbol(int kind)	//Check if the symbol being called exists in the table.
{
	int i;

	for (i = 0; i < sym_index; i++)
	{
		if (!strcmp(token.name, table[i].name) && kind == table[i].kind)
			return;
		if (kind == FACTOR)
		{
			if (!strcmp(token.name, table[i].name) &&
			(VARIABLE == table[i].kind || CONSTANT == table[i].kind))
				return;
		}
	}

	errorend(7);
}

void errorend(int x)
{
	switch (x)
	{
		case 1:
			printf("Parser Error: Competing Symbol Declarations\n");
			break;
		case 2:
			printf("Parser Error: Unrecognized Statement Form\n");
			break;
		case 3:
			printf("Parser Error: Programs Must Close with a Period\n");
			break;
		case 4:
			printf("Parser Error: Symbols Must Be Declared with an Identifier\n");
			break;
		case 5:
			printf("Parser Error: Constants Must Be Assigned a Value at Declaration\n");
			break;
		case 6:
			printf("Parser Error: Symbol Declarations Must Be Followed By a Semicolon\n");
			break;
		case 7:
			printf("Parser Error: Undeclared Symbol\n");
			break;
		case 8:
			printf("Parser Error: while Must Be Followed By do\n");
			break;
		case 9:
			printf("Parser Error: if Must Be Followed By then\n");
			break;
		case 10:
			printf("Parser Error: begin Must Be Followed By end\n");
			break;
		case 11:
			printf("Parser Error: while and if Statements Must Contain Conditions\n");
			break;
		case 12:
			printf("Parser Error: Conditions Must Contain a Relational-Operator\n");
			break;
		case 13:
			printf("Parser Error: ( Must Be Followed By )\n");
			break;
		case 14:
			printf("Parser Error: call and read Must Be Followed By an Identifier\n");
			break;
		default:
			printf("Implementation Error: Unrecognized Error Code\n");
			break;
	}
	exit(0);
}

void printtable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address\n");
	printf("--------------------------------------------\n");
	for (i = 0; i < sym_index; i++)
		printf("%4d | %11s | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr);
}
