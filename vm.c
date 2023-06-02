/*
*   pl/0 virtual machine
*   by Jonathan Tielve
*/

#include<stdio.h>
#include<stdlib.h>

#define ARRAY_SIZE 500

int base(int bp, int L);

typedef struct {
    unsigned int OP;
    unsigned int L;
    unsigned int M;
} instruction;

int pc = 0;
int bp = 0;
int sp = 0;
int EOP = 1;
int pas[ARRAY_SIZE];
int ar[ARRAY_SIZE];
instruction ir;

void main(int argc, char *argv[])
{
    //Initialize the PAS "stack" with 0
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        pas[i] = 0;
    }

    //Read in all the instructions from the input file
    FILE *input = fopen("input.txt", "r");
    while(fscanf(input, "%d %d %d", &pas[pc], &pas[pc+1], &pas[pc+2]) == 3)
    {
        pc = pc + 3;
    }
    fclose(input);

    //Initialize BP, SP and PC
    //with their respective values based on size of input
    int arCounter = 0;
    bp = pc;
    sp = bp - 1;
    pc = 0;
    ir.OP = 0;
    ir.L = 0;
    ir.M = 0;

    //Print BP, SP and PC values
    printf("                PC  BP  SP  Stack\n");
    printf("Initial Values: %d  %d  %d\n", pc, bp, sp);

    while(EOP != 0)
    {
        ir.OP = pas[pc];
        ir.L = pas[pc+1];
        ir.M = pas[pc+2];
        pc = pc + 3;
        switch(ir.OP)
        {
            case 1: //LIT
                sp = sp + 1;
                pas[sp] = ir.M;
                ar[sp] = ir.M;
                arCounter++;
                break;
            case 2: //OPR
                switch(ir.M)
                {
                    case 0: //RTN
                        sp = bp - 1;
                        bp = pas[sp + 2];
                        pc = pas[sp + 3];
                        break;
                    case 1: //ADD + 
                        pas[sp - 1] = (pas[sp - 1] + pas[sp]);
                        sp = sp - 1;
                        break;
                    case 2: //SUB -
                        pas[sp - 1] = (pas[sp - 1] - pas[sp]);
                        sp = sp - 1;
                        break;
                    case 3: //MUL *
                        pas[sp - 1] = (pas[sp - 1] * pas[sp]);
                        sp = sp - 1;
                        break;
                    case 4: //DIV /
                        pas[sp - 1] = (pas[sp - 1] / pas[sp]);
                        sp = sp - 1;
                        break;
                    case 5: //EQL ==
                        pas[sp - 1] = (pas[sp - 1] == pas[sp]);
                        sp = sp - 1;
                        break;
                    case 6: //NEQ !=
                        pas[sp - 1] = (pas[sp - 1] != pas[sp]);
                        sp = sp - 1;
                        break;
                    case 7: //LSS <
                        pas[sp - 1] = (pas[sp - 1] < pas[sp]);
                        sp = sp - 1;
                        break;
                    case 8: //LEQ <=
                        pas[sp - 1] = (pas[sp - 1] <= pas[sp]);
                        sp = sp - 1;
                        break;
                    case 9: //GTR >
                        pas[sp - 1] = (pas[sp - 1] > pas[sp]);
                        sp = sp - 1;
                        break;
                    case 10: //GEQ >=
                        pas[sp - 1] = (pas[sp - 1] >= pas[sp]);
                        sp = sp - 1;
                        break;
                }
                break;
            case 3: //LOD
                sp = sp + 1;
                pas[sp] = pas[base(bp, ir.L) + ir.M];
                ar[sp] = pas[base(bp, ir.L) + ir.M];
                arCounter++;
                break;
            case 4: //STO
                pas[base(bp, ir.L) + ir.M] = pas[sp];
                ar[base(bp, ir.L) + ir.M] = pas[sp];
                arCounter++;
                sp = sp - 1;
                break;
            case 5: //CAL
                pas[sp + 1] = base(bp, ir.L); //Static Link
                pas[sp + 2] = bp;   //Dynamic Link
                pas[sp + 3] = pc;   //Return Address
                
                //Save SL, DL, and RA values for Activation Record
                ar[sp + 1] = -1;
                ar[sp + 2] = base(bp, ir.L);    
                ar[sp + 3] = bp;
                ar[sp + 4] = pc;
                arCounter += 4;

                bp = sp + 1;
                pc = ir.M;
                break;
            case 6: //INC
                sp = sp + ir.M;
                break;
            case 7: //JMP
                pc = ir.M;
                break;
            case 8: //JPC
                if(pas[sp] == 0)
                {
                    pc = ir.M;
                }
                sp = sp - 1;
                break;
            case 9: //SYS
                switch(ir.M)
                {
                    case 1: //Write 
                        printf("Output result is: %d\n", pas[sp]);
                        sp = sp - 1;
                        break;
                    case 2: //Read and Store
                        sp = sp + 1;
                        int num = 0;
                        printf("Please input an Integer:");
                        scanf("%d", &num);
                        pas[sp] = num;
                        ar[sp] = num;
                        arCounter++;                       
                        break;
                    case 3: //Halt
                        EOP = 0;
                        break;
                }
                break;             
            default: //If instruction is not recognized
                EOP = 0;
                break;
        }
        printf("%d %d %d \t\t%d %d %d    ", ir.OP, ir.L, ir.M, pc, bp, sp);
        for(int i = sp - arCounter; i <= sp; i++)
        {
            if(ar[i] == -1)
                printf("| ");
            else
                printf("%d ", ar[i]);
        }
        printf("\n");
    }
    return;
}

int base( int BP, int L)
{
    int arb = BP;
    while (L > 0)
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}