#include "def.h"
#include "func_SAT.h"
#include "func_Sudoku.h"
int main(void)
{
    int option;
    printf("                   Final Design\n");
    while (1)
    {
        option = 0;
        printf("-------------------------------------------------\n");
        printf("*                  1 SAT                        *\n");
        printf("*                  2 sudoku                     *\n");
        printf("*                  3 quit                       *\n");
        printf("-------------------------------------------------\n");
        while (option != 1 && option != 2 && option != 3)
        {
            printf("Please enter 1, 2 or 3\n");
            scanf("%d", &option);
        }
        if (option == 1)
            SATsolver();
        else if (option == 2)
            puzzle();
        else
            break;
    }
    return 0;
}