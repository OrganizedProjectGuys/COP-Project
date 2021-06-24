/*
	This is the lex skeleton for the UCF Summer 2021 Systems Software Project
	Implement the function lexanalyzer, add as many functions and global
	variables as desired, but do not alter printerror or printtokens.
	Include your name (and your partner's name) in this comment in order to
	earn the points for compiling
	Authors: Christopher Hayden & Zachary Mace
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define ALL_KEYWORDS 14

lexeme *list;
int lex_index;
char identifier[MAX_IDENTIFIER_LENGTH];
char number[MAX_NUMBER_LENGTH];
char keywords[ALL_KEYWORDS][MAX_IDENTIFIER_LENGTH] = {"odd", 
                                                      "begin",
                                                      "end",
                                                      "if",
                                                      "then",
                                                      "else",
                                                      "while",
                                                      "do",
                                                      "call",
                                                      "write",
                                                      "read",
                                                      "const",
                                                      "var",
                                                      "procedure"
                                                      };

void printerror(int type);
void printtokens();
int checkKeywords();

int checkKeywords()
{
  int i;
  for (i = 0; i < ALL_KEYWORDS; i++)
  {
    //printf("%s", identifier);
    if (strcmp(identifier, keywords[i]) == 0)
    {
      printf("Success\n");
      switch (i)
      {
        case 0:
          list[lex_index].value = oddsym;
          break;
        case 1:
          list[lex_index].value = beginsym;
          break;
        case 2:
          list[lex_index].value = endsym;
          break;
        case 3:
          list[lex_index].value = ifsym;
          break;
        case 4:
          list[lex_index].value = thensym;
          break;
        case 5:
          list[lex_index].value = elsesym;
          break;
        case 6:
          list[lex_index].value = whilesym;
          break;
        case 7:
          list[lex_index].value = dosym;
          break;
        case 8:
          list[lex_index].value = callsym;
          break;
        case 9:
          list[lex_index].value = writesym;
          break;
        case 10:
          list[lex_index].value = readsym;
          break;
        case 11:
          printf("Setting const symbol");
          list[lex_index].value = constsym;
          break;
        case 12:
          list[lex_index].value = varsym;
          break;
        case 13:
          list[lex_index].value = procsym;
          break;
      }
      return 1;
    }
  }
  return 0;
}

lexeme *lexanalyzer(char *input)
{
	list = malloc(500 * sizeof(lexeme));
	lex_index = 0;
	int input_index, i, len;
	int input_length = strlen(input);
	char c, nextChar;

	printf("%s\n", input);  //Print out the given input. (DELETE LATER).

	for (input_index = 0; input_index < input_length; input_index++)
	{
		c = input[input_index];

    //Test all of the Valid Symbols first.
		switch (c)
		{
      case '=':
        if (input[input_index + 1] == '=')
        {
          list[lex_index].type = eqlsym;
          lex_index++;
          input_index++;
        }
        else
          printerror(1);
        break;
      case '<':
        if (input[input_index + 1] == '>')
        {
          list[lex_index].type = neqsym;
          lex_index++;
          input_index++;
        }
        else if (input[input_index + 1] == '=')
        {
          list[lex_index].type = leqsym;
          lex_index++;
          input_index++;
        }
        else
        {
          list[lex_index].type = lessym;
          lex_index++;
        }
        break;
      case '>':
        if (input[input_index + 1] == '=')
        {
          list[lex_index].type = geqsym;
          lex_index++;
          input_index++;
        }
        else
        {
          list[lex_index].type = gtrsym;
          lex_index++;
        }
        break;
      case '%':
        list[lex_index].type = modsym;
        lex_index++;
        break;
      case '/':
        list[lex_index].type = slashsym;
        lex_index++;
        break;
      case '+':
        list[lex_index].type = plussym;
        lex_index++;
        break;
      case '-':
        list[lex_index].type = minussym;
        lex_index++;
        break;
      case '(':
        list[lex_index].type = lparentsym;
        lex_index++;
        break;
      case ')':
        list[lex_index].type = rparentsym;
        lex_index++;
        break;
      case ',':
        list[lex_index].type = commasym;
        lex_index++;
        break;
      case '.':
        list[lex_index].type = periodsym;
        lex_index++;
        break;
      case ';':
        list[lex_index].type = semicolonsym;
        lex_index++;
        break;
      case ':':
        if (input[input_index + 1] == '=')
        {
          list[lex_index].type = becomessym;
          lex_index++;
          input_index++;
        }
        else
          printerror(1);
        break;
      default:
        break;
		}


    //If our current input character is a letter (a-z) or (A-Z). 
    if (isalpha(c))
    {
      //Loop for all 11 possible letters/numbers to be read in.
      for (i = 0; i < MAX_IDENTIFIER_LENGTH; i++)
      {
 
        //Set c to our current input.
        c = input[input_index];
        identifier[i] = c;
        //Set nextChar to the next place in the input array.
        nextChar = input[input_index + 1];
        printf("%s\n", identifier);

        //If we've reached the max identifier length, and the next character is a letter or digit, return error.
        if (i == 10 && (isalpha(nextChar) || isdigit(nextChar)))
        {
           printerror(4); //Excessive Identifier Length
           exit(0);
        }

        //If the next character is not a letter or digit, we've reached the end of the identifier.
        if (!isalpha(nextChar) && !isdigit(nextChar))
        {
          //If the identifier is NOT a keyword.
          if (!checkKeywords())
            {
              strcpy(list[lex_index].name, identifier);
              list[lex_index].type = identsym;
              lex_index++;
              for (i = 0; i < MAX_IDENTIFIER_LENGTH; i++)
                identifier[i] = '\0';
              break;
            }
          break;
        }

        else if (isalpha(nextChar) || isdigit(nextChar))
        {
          c = nextChar;
          input_index++;
        }

      }
      for (i = 0; i < MAX_IDENTIFIER_LENGTH; i++)
        identifier[i] = '\0';
	  }

    //If our current input character is an integer (0-9).
    if (isdigit(c))
    {
      //Loop for all 5 possible numbers to be read in.
      for (i = 0; i < MAX_NUMBER_LENGTH; i++)
      {
        c = input[input_index];
        nextChar = input[input_index + 1];

        //If a letter is the next character to be read in, return error.
        if (isalpha(nextChar) && i != 4)
        {
          printerror(2); //Invalid Identifier
          exit(0);
        }

        //If there are an excess amount of numbers, return error.
        if (isdigit(nextChar) && i == 4)
        {
          printerror(3); //Excessive Number Length
          exit(0);
        }

        //If the next character is NOT a digit, break out of the loop.
        if (!isdigit(nextChar))
        {
          number[i] = c;
          break;
        }

        //If there are no errors, add the current number and increment the index.
        else
        {
          number[i] = c;
          input_index++;
        }
      }
      list[lex_index].value = atoi(number);
      list[lex_index].type = numbersym;
      lex_index++;
      *number = '\0';
    }
  }
  printtokens();
	return list;
}


void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "<>", neqsym);
				break;
			case lessym:
				printf("%11s\t%d", "<", lessym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case slashsym:
				printf("%11s\t%d", "/", slashsym);
				break;
			case plussym:
				printf("%11s\t%d", "+", plussym);
				break;
			case minussym:
				printf("%11s\t%d", "-", minussym);
				break;
			case lparentsym:
				printf("%11s\t%d", "(", lparentsym);
				break;
			case rparentsym:
				printf("%11s\t%d", ")", rparentsym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case becomessym:
				printf("%11s\t%d", ":=", becomessym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else if (type == 5)
		printf("Lexical Analyzer Error: Neverending Comment\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");

	free(list);
	return;
}

int main(int argc, char **argv)
{
	FILE *ifp;
	char *inputfile;
	char c;
	lexeme *list;
	int i;

	if (argc < 2)
	{
		printf("Error : please include the file name");
		return 0;
	}

	ifp = fopen(argv[1], "r");
	inputfile = malloc(500 * sizeof(char));
	i = 0;

	c = fgetc(ifp);
	while (1)
	{
		inputfile[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	inputfile[i] = '\0';

	list = lexanalyzer(inputfile);
	if (list == NULL)
	{
		free(inputfile);
		return 0;
	}

	free(list);
	free(inputfile);
	return 0;
}
