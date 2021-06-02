#include <stdio.h>

typedef struct instruction {
  int op; //opcode
  int l; //L
  int m; //M
} instruction;

int sp = 0; //stack pointer
int bp = 1; //base pointer
int pc = 0; //program counter
instruction ir; //instruction register, not sure how this will work

#define MAX_PAS_LENGTH 500

int pas[MAX_PAS_LENGTH] = {0}; //process address space


int main(int argc, char *argv[])
{
  FILE *fp;
  char *filename;

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
    fscanf(fp, "%d%d%d", &pas[sp], &pas[sp+1], &pas[sp+2]);
    //printf("%d %d %d\n", pas[sp], pas[sp+1], pas[sp+2]);
    sp += 3;
  }

  fclose(fp);

  sp -= 1;
  bp = sp + 1;
}
