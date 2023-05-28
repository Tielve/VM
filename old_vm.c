/*
Jonathan Tielve & Kaitlyn Stagg
VM/p-machine
*/

#include<stdio.h>
#include<stdlib.h>

#define ARRAY_SIZE 500

int base(int bp, int L);
void print();

typedef struct {
    unsigned int opCode;
    unsigned int L;
    unsigned int M;
} instruction;

typedef struct 
{
    char op[3];
} opCodes;

opCodes code[22] = 
{"LIT", "OPR", "RTN", "ADD", "SUB", "MUL", "DIV", 
"EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LOD", 
"STO", "CAL", "INC", "JMP", "JPC", "SIN", "SOU", "EOP"};

int pc = 0;
int sp = 0;
int bp = 0;
int pas[ARRAY_SIZE];
int EOP = 1;
instruction ir;

int main(int argc, char *argv[])
{
    for (int i = 0; i < ARRAY_SIZE; i++) //initialize pas with 0's
    {
        pas[i] = 0;
    }

    FILE *f = fopen(argv[1], "r");
    while(fscanf(f, "%d %d %d", &pas[pc], &pas[pc+1], &pas[pc+2]) == 3)
    {
        pc = pc + 3;
    }
    fclose(f);

    bp = pc;
    sp = bp - 1;
    pc = 0;

    printf("                PC  BP  SP  Stack\n");
    printf("Initial Values: %d  %d  %d\n", pc, bp, sp);

    while(EOP != 0)
    {
        ir.opCode = pas[pc];
        ir.L = pas[pc+1];
        ir.M = pas[pc+2];
        pc = pc + 3;
        switch(ir.opCode)
        {
            case 1: //Lit
                sp = sp - 1;
                pas[sp] = ir.M;
                break;
            case 2: //OPR
                switch(ir.M)
                {
                    case 0: //RTN
                        sp = bp - 1;
                        bp = pas[sp + 2];
                        pc = pas[sp + 3];
                        break;
                    case 1: //ADD
                        pas[sp - 1] = (pas[sp - 1] + pas[sp]);
                        sp = sp - 1;
                        break;
                    case 2: //SUB
                        pas[sp - 1] = (pas[sp - 1] - pas[sp]);
                        sp = sp - 1;
                        break;
                    case 3: //MUL
                        pas[sp - 1] = (pas[sp - 1] * pas[sp]);
                        sp = sp - 1;
                        break;
                    case 4: //DIV
                        pas[sp - 1] = (pas[sp - 1] / pas[sp]);
                        sp = sp - 1;
                        break;
                    case 5: //EQL
                        pas[sp + 1] = (pas[sp + 1] == pas[sp]);
                        sp = sp - 1;
                        break;
                    case 6: //NEQ
                        pas[sp + 1] = (pas[sp + 1] != pas[sp]);
                        sp = sp - 1;
                        break;
                    case 7: //LSS
                        pas[sp + 1] = (pas[sp + 1] < pas[sp]);
                        sp = sp - 1;
                        break;
                    case 8: //LEQ
                        pas[sp + 1] = (pas[sp + 1] <= pas[sp]);
                        sp = sp - 1;
                        break;
                    case 9: //GTR
                        pas[sp + 1] = (pas[sp + 1] > pas[sp]);
                        sp = sp - 1;
                        break;
                    case 10: //GEQ
                        pas[sp + 1] = (pas[sp + 1]>=pas[sp]);
                        sp = sp - 1;
                        break;
                }
                break;
            case 3: //LOD
                sp = sp + 1;
                pas[sp] = pas[base(bp, ir.L) + ir.M];
                break;
            case 4: //STO
                pas[base(bp, ir.L) + ir.M] = pas[sp];
                sp = sp - 1;
                break;
            case 5: //CAL
                pas[sp + 1] = base(bp, ir.L);
                pas[sp + 2] = bp;
                pas[sp + 3] = pc;
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
                        break;
                    case 3: //End of Program
                        EOP = 0;
                        break;
                }
                break;
            default:
                EOP = 0;
                break;
        }
        print();
    }
    return 0;
}

int base( int BP, int L)
{
    int arb = BP; // arb = activation record base
    while (L > 0)     //find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}

void print()
{
    printf("%d %d %d \t%d %d %d \t", ir.opCode, ir.L, ir.M, pc, bp, sp);
    for(int i = 499; i > sp; i--)
    {
        printf("%d ", pas[i]);
    }
    printf("\n");
}