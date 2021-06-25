/*
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

//Function that compares all keywords to the current identifier.
int checkKeywords()
{
  int i;

  //Loop through all keywords.
  for (i = 0; i < ALL_KEYWORDS; i++)
  {
    //If we have a correct comparison, stop and set the type.
    if (strcmp(identifier, keywords[i]) == 0)
    {
      switch (i)
      {
        case 0:
          list[lex_index].type = oddsym;
          break;
        case 1:
          list[lex_index].type = beginsym;
          break;
        case 2:
          list[lex_index].type = endsym;
          break;
        case 3:
          list[lex_index].type = ifsym;
          break;
        case 4:
          list[lex_index].type = thensym;
          break;
        case 5:
          list[lex_index].type = elsesym;
          break;
        case 6:
          list[lex_index].type = whilesym;
          break;
        case 7:
          list[lex_index].type = dosym;
          break;
        case 8:
          list[lex_index].type = callsym;
          break;
        case 9:
          list[lex_index].type = writesym;
          break;
        case 10:
          list[lex_index].type = readsym;
          break;
        case 11:
          list[lex_index].type = constsym;
          break;
        case 12:
          list[lex_index].type = varsym;
          break;
        case 13:
          list[lex_index].type = procsym;
          break;
      }
      lex_index++;
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

  //Loop through the input.
	for (input_index = 0; input_index < input_length; input_index++)
	{
		c = input[input_index];
    nextChar = input[input_index + 1];

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
        {
          printerror(1);
          return NULL;
        }
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
      case '*':
        list[lex_index].type = multsym;
        lex_index++;
        break;
      case '/':
        if (input[input_index + 1] == '*')  //Check for comments.
        {
          input_index += 2;
          while ((input[input_index] != '*') || (input[input_index + 1] != '/'))   //Check if the comment ends.
          {
            if (input_index == input_length)
            {
              printerror(5);
              return NULL;
            }
            input_index++;
          }
          input_index ++;
        }
        else
        {
          list[lex_index].type = slashsym;
          lex_index++;
        }
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
        {
          printerror(1);
          return NULL;
        }
        break;
      default:
        if (iscntrl(c) || isspace(c))
          continue;
        if (!isalpha(c) && !(isdigit(c)))
        {
          printerror(1);
          return NULL;
        }
		}


    //If our current input character is a letter (a-z) or (A-Z). 
    if (isalpha(c))
    {
      //Loop for all 11 possible letters/numbers to be read in.
      for (i = 0; i < MAX_IDENTIFIER_LENGTH; i++)
      {
        //Set c to our current input and add it to the identifier string.
        c = input[input_index];
        identifier[i] = c;

        //Set nextChar to the next place in the input array.
        nextChar = input[input_index + 1];

        //If we've reached the max identifier length, and the next character is a letter or digit, return error.
        if (i == 10 && (isalpha(nextChar) || isdigit(nextChar)))
        {
           printerror(4); //Excessive Identifier Length
           return NULL;
        }

        //If the next character is not a letter or digit, we've reached the end of the identifier.
        if (!isalpha(nextChar) && !isdigit(nextChar))
        {
          //Check to see if the identifier is a keyword.
          if (!checkKeywords())
            {
              strcpy(list[lex_index].name, identifier);
              list[lex_index].type = identsym;
              lex_index++;
              //Clear the identifier string.
              for (i = 0; i < MAX_IDENTIFIER_LENGTH; i++)
                identifier[i] = '\0';
              break;
            }
          break;
        }
        //If we are safe to add the next character, index by 1.
        else if (isalpha(nextChar) || isdigit(nextChar))
        {
          c = nextChar;
          input_index++;
        }
      }
      //Clear the identifier string.
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

        //If a letter is to be read in, or the digit is not separated by a whitespace/symbol: print error.
        if ((isalpha(nextChar) && i != 4) || (!isdigit(nextChar) && i== 4))
        {
          printerror(2); //Invalid Identifier
          return NULL;
        }

        //If there are an excess amount of numbers, print error.
        if (isdigit(nextChar) && i == 4)
        {
          printerror(3); //Excessive Number Length
          return NULL;
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

    //The symbol was not found, print Invalid Symbol Error.

  }

  //Print the tokens and return the list.
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
