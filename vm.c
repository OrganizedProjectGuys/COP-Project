/*
 *	Authors...: Christopher Hayden & Zack 
 *	Class.....: COP3402
 *  Assignment: HW1_VM
 *  Due Date..: Jun 4th 11:59pm
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_PAS_LENGTH 500
#define MAX_STACK_LENGTH 350

typedef struct instruction {
    int op; //opcode
    int l; //L
    int m; //M
} instruction;

int sp = 0; //stack pointer
int bp = 1; //base pointer
int pc = 0; //program counter
int halt = 1;  //Flag
instruction ir; //instruction register

int pas[MAX_PAS_LENGTH] = { 0 }; //process address space
int stack[MAX_STACK_LENGTH] = { 0 };  //stack array for printing out contents

int base(int L)
{
  int arb = bp;
  while (L > 0)
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}


int main(int argc, char* argv[])
{
    FILE* fp;
    char* filename;
    int i = 0;
    int j = 0;
    int currInstruction = 0;
    int newBP = 0;

    if (argc < 2)
    {
        printf("Missing file.\n");
        return 0;
    }
    else
        filename = argv[1];

    fp = fopen(filename, "r");

    //Read in the instructions from the file
    while (!feof(fp))
    {
      fscanf(fp, "%d%d%d", &pas[sp], &pas[sp + 1], &pas[sp + 2]);
      //printf("%d %d %d\n", pas[sp], pas[sp+1], pas[sp+2]);
      sp += 3;
    }

    //print out the pas array with formatting
    fclose(fp);

    bp = sp;
    int ogbp = bp;
    sp -= 1;

    printf("                PC  BP  SP  stack\n");
    printf("Initial values: %d   %d  %d\n\n", pc, bp, sp);

    while (halt)
    {
      currInstruction = pc;
      ir.op = pas[pc];
      ir.l = pas[pc + 1];
      ir.m = pas[pc + 2];
      pc += 3;

      switch (ir.op)
      {
        case 1: //LIT
          sp = sp + 1;
          pas[sp] = ir.m;
          break;
        case 2: //OPR
            switch (ir.m)
            {
              case 0: //RTN
                sp = bp - 1;
                bp = pas[sp + 2];
                pc = pas[sp + 3];
                break;
              case 1: //NEG
                pas[sp] = pas[sp] * -1;
                break;
              case 2: //ADD
                sp = sp - 1;
                pas[sp] = pas[sp] + pas[sp + 1];
                break;
              case 3: //SUB
                sp = sp - 1;
                pas[sp] = pas[sp] - pas[sp + 1];
                break;
              case 4: //MUL
                sp = sp - 1;
                pas[sp] = pas[sp] * pas[sp + 1];
                break;
              case 5: //DIV
                sp = sp - 1;
                pas[sp] = pas[sp] / pas[sp + 1];
                break;
              case 6: //ODD
                pas[sp] = pas[sp] % 2;
                break;
              case 7: //MOD
                sp = sp - 1;
                pas[sp] = pas[sp] % pas[sp + 1];
                break;
              case 8: //EQL
                sp = sp - 1;
                pas[sp] = pas[sp] == pas[sp + 1];
                break;
              case 9: //NEQ
                sp = sp - 1;
                pas[sp] = pas[sp] != pas[sp + 1];
                break;
              case 10:  //LSS
                sp = sp - 1;
                pas[sp] = pas[sp] < pas[sp + 1];
                break;
              case 11:  //LEQ
                sp = sp - 1;
                pas[sp] = pas[sp] <= pas[sp + 1];
                break;
              case 12:  //GTR
                sp = sp - 1;
                pas[sp] = pas[sp] > pas[sp + 1];
                break;
              case 13:  //GEQ
                sp = sp - 1;
                pas[sp] = pas[sp] >= pas[sp + 1];
                break;
            }
            break;
        case 3: //LOD
          sp = sp + 1;
          pas[sp] = pas[base(ir.l) + ir.m];
          break;
        case 4: //STO
          pas[base(ir.l) + ir.m] = pas[sp];
          sp = sp - 1;
          break;
        case 5: //CAL
          pas[sp + 1] = base(ir.l);
          pas[sp + 2] = bp;
          pas[sp + 3] = pc;
          bp = sp + 1;
          newBP = bp;
          pc = ir.m;
          break;
        case 6: //INC
          sp = sp + ir.m;
          break;
        case 7: //JMP
          pc = ir.m;
          break;
        case 8: //JPC
          if (pas[sp] == 1)
            pc = ir.m;
          sp = sp - 1;
          break;
        case 9: //SYS
            switch (ir.m)
            {
              case 1: //Peek the top of the stack
                printf("Top of Stack Value: %d\n", pas[sp]);
                sp = sp - 1;
                break;
              case 2: //Read in user input and push it onto the stack
                sp = sp + 1;
                printf("Please Enter an Integer:  ");
                scanf("%d", &pas[sp]);
                break;
              case 3: //End the program
                halt = 0;
                break;
            }
            break;

        default:
          return 0;
    }

    printf("%2d ", currInstruction);
    //I can't figure out how to not print the instruction like this.... but it works:
    switch (ir.op)
    {
      case 1:
        printf("LIT  ");
        break;
      case 2:
        switch(ir.m)
        {
          case 0:
            printf("RTN  ");
            break;
          case 1:
            printf("NEG  ");
            break;
          case 2:
            printf("ADD  ");
            break;
          case 3:
            printf("SUB  ");
            break;
          case 4:
            printf("MUL  ");
            break;
          case 5:
            printf("DIV  ");
            break;
          case 6:
            printf("ODD  ");
            break;
          case 7:
            printf("MOD  ");
            break;
          case 8:
            printf("EQL  ");
            break;
          case 9:
            printf("NEQ  ");
            break;
          case 10:
            printf("LSS  ");
            break;
          case 11:
            printf("LEQ  ");
            break;
          case 12:
            printf("GTR  ");
            break;
          case 13:
            printf("GEQ  ");
            break;
        }
        break;
      case 3:
        printf("LOD  ");
        break;
      case 4:
        printf("STO  ");
        break;
      case 5:
        printf("CAL  ");
        break;
      case 6:
        printf("INC  ");
        break;
      case 7:
        printf("JMP  ");
        break;
      case 8:
        printf("JPC  ");
        break;
      case 9:
        printf("SYS  ");
        break;
    }

    printf("%2d %2d    %2d  %2d  %2d  ", ir.l, ir.m, pc, bp, sp);
    j = sp;
    for (i = ogbp; i <= j; i++)
    {
      if (i == newBP)
        printf("|");
      printf("%2d ", pas[i]);
    }
    printf("\n");
  }

}// end main
