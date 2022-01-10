/***********************************
*文件名称：func_Sudoku.h
*文件描述：关于求解二进制数独问题的函数。
*日    期：2020.03.01
*作    者：YPC
***********************************/
#include "def.h"

/***********************************
*函数名称：Combination2
*函数功能：把约束三要求的子句按格式写入fp指向的文件位置
*输入参数：处于正确位置的文件指针fp，排列组合元素数组array，结果数组result，排列组合个数size，排列组合递归循环变量index，递归层数deep，变元计数器引用vnumcount
*返回参数：OK -> 成功
***********************************/
int Combination2(FILE *fp, int array[], int result[], int size, int index, int deep) //约束二子句生成，排列组合递归
{
    int i;
    if (deep == size / 2 + 1)
    {
        for (i = 0; i < size / 2 + 1; i++)
        {
            fprintf(fp, "%d ", result[i]);
        }
        fprintf(fp, "0\n");
        for (i = 0; i < size / 2 + 1; i++)
        {
            fprintf(fp, "-%d ", result[i]);
        }
        fprintf(fp, "0\n");
        return OK;
    }
    for (i = index; i < size; i++, index++)
    {
        result[deep] = array[i];
        deep++;
        Combination2(fp, array, result, size, index + 1, deep);
        deep--;
    }
    return OK;
}

/***********************************
*函数名称：CombinationR\L
*函数功能：把约束三要求的子句按格式写入fp指向的文件位置。
*输入参数：处于正确位置的文件指针fp，排列组合元素数组array，结果数组result，排列组合个数size，排列组合递归循环变量index，递归层数deep，变元计数器引用vnumcount
*返回参数：OK -> 成功
***********************************/
int CombinationR(FILE *fp, int result[], int size, int index, int deep, int &vnumcount) //约束三行子句生成，排列组合递归
{
    int i;
    if (deep == 2)
    {
        for (i = 1; i <= size; i++)
        {
            vnumcount++;
            fprintf(fp, "%d -%d 0\n", result[0] * size + i, vnumcount);
            fprintf(fp, "%d -%d 0\n", result[1] * size + i, vnumcount);
            fprintf(fp, "-%d -%d %d 0\n", result[0] * size + i, result[1] * size + i, vnumcount);
            vnumcount++;
            fprintf(fp, "-%d -%d 0\n", result[0] * size + i, vnumcount);
            fprintf(fp, "-%d -%d 0\n", result[1] * size + i, vnumcount);
            fprintf(fp, "%d %d %d 0\n", result[0] * size + i, result[1] * size + i, vnumcount);
            vnumcount++;
            fprintf(fp, "-%d %d 0\n", vnumcount - 2, vnumcount);
            fprintf(fp, "-%d %d 0\n", vnumcount - 1, vnumcount);
            fprintf(fp, "%d %d -%d 0\n", vnumcount - 2, vnumcount - 1, vnumcount);
        }
        vnumcount++;
        fprintf(fp, "-%d ", vnumcount);
        for (i = 0; i < size; i++)
        {
            fprintf(fp, "-%d ", vnumcount - 1 - i * 3);
        }
        fprintf(fp, "0\n");
        for (i = 1; i <= size; i++)
        {
            fprintf(fp, "%d %d 0\n", vnumcount - i, vnumcount);
        }

        return OK;
    }
    for (i = index; i < size; i++, index++)
    {
        result[deep] = i;
        deep++;
        CombinationR(fp, result, size, index + 1, deep, vnumcount);
        deep--;
    }
    return OK;
}

/***********************************
*函数名称：ClearVarNode
*函数功能：删除head指向的文字链表。
*输入参数：表头指针head
*返回参数：OK -> 删除成功
***********************************/
int CombinationL(FILE *fp, int result[], int size, int index, int deep, int &vnumcount) //约束三列子句生成，排列组合递归
{
    int i;
    if (deep == 2)
    {
        for (i = 0; i < size; i++)
        {
            vnumcount++;
            fprintf(fp, "%d -%d 0\n", i * size + result[0] + 1, vnumcount);
            fprintf(fp, "%d -%d 0\n", i * size + result[1] + 1, vnumcount);
            fprintf(fp, "-%d -%d %d 0\n", i * size + result[0] + 1, i * size + result[1] + 1, vnumcount);
            vnumcount++;
            fprintf(fp, "-%d -%d 0\n", i * size + result[0] + 1, vnumcount);
            fprintf(fp, "-%d -%d 0\n", i * size + result[1] + 1, vnumcount);
            fprintf(fp, "%d %d %d 0\n", i * size + result[0] + 1, i * size + result[1] + 1, vnumcount);
            vnumcount++;
            fprintf(fp, "-%d %d 0\n", vnumcount - 2, vnumcount);
            fprintf(fp, "-%d %d 0\n", vnumcount - 1, vnumcount);
            fprintf(fp, "%d %d -%d 0\n", vnumcount - 2, vnumcount - 1, vnumcount);
        }
        vnumcount++;
        fprintf(fp, "-%d ", vnumcount);
        for (i = 0; i < size; i++)
        {
            fprintf(fp, "-%d ", vnumcount - 1 - i * 3);
        }
        fprintf(fp, "0\n");
        for (i = 1; i <= size; i++)
        {
            fprintf(fp, "%d %d 0\n", vnumcount - i, vnumcount);
        }

        return OK;
    }
    for (i = index; i < size; i++, index++)
    {
        result[deep] = i;
        deep++;
        CombinationL(fp, result, size, index + 1, deep, vnumcount);
        deep--;
    }
    return OK;
}

/***********************************
*函数名称：PuzzleTransform
*函数功能：把二进制数独题面转换为cnf文件写入filename中。
*输入参数：二维指针puzzle，puzzle大小size，待写入文件名filename[100]
*返回参数：OK -> 成功
***********************************/
int PuzzleTransform(char **puzzle, int size, char filename[100]) //棋局转换为cnf
{
    if (size != 4 && size != 6 && size != 8)
    {
        printf("only 4,6,8 are allowed!");
        return ERROR;
    }
    FILE *fp;
    if ((fp = fopen(filename, "wb")) == NULL)
    {
        printf("save error!\n");
        return ERROR;
    }

    int vnum, lnum;

    if (size == 8)
    {
        vnum = 1464;
        lnum = 4536 + 192 + 1792;
    }
    else if (size == 6)
    {
        vnum = 570 + 36;
        lnum = 1830 + 96 + 360;
    }
    else if (size == 4)
    {
        vnum = 156 + 16;
        lnum = 492 + 32 + 64;
    }
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (puzzle[i][j] == '1' || puzzle[i][j] == '0')
                lnum++;
        }
    }

    fprintf(fp, "p cnf %d %d\n", vnum, lnum);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (puzzle[i][j] == '1')
                fprintf(fp, "%d 0\n", i * size + j + 1);
            else if (puzzle[i][j] == '0')
                fprintf(fp, "-%d 0\n", i * size + j + 1);
        }
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size - 2; j++)
        {
            fprintf(fp, "%d %d %d 0\n", i * size + j + 1, i * size + j + 2, i * size + j + 3);
            fprintf(fp, "-%d -%d -%d 0\n", i * size + j + 1, i * size + j + 2, i * size + j + 3);
        }
    }
    for (i = 0; i < size - 2; i++)
    {
        for (j = 0; j < size; j++)
        {
            fprintf(fp, "%d %d %d 0\n", i * size + j + 1, (i + 1) * size + j + 1, (i + 2) * size + j + 1);
            fprintf(fp, "-%d -%d -%d 0\n", i * size + j + 1, (i + 1) * size + j + 1, (i + 2) * size + j + 1);
        }
    }
    for (i = 0; i < size; i++)
    {
        int array[size];
        int result[size];
        for (j = 0; j < size; j++)
        {
            array[j] = i * size + j + 1;
        }
        Combination2(fp, array, result, size, 0, 0);
    }
    for (i = 0; i < size; i++)
    {
        int array[size];
        int result[size];
        for (j = 0; j < size; j++)
        {
            array[j] = i + size * j + 1;
        }
        Combination2(fp, array, result, size, 0, 0);
    }
    int array[size];
    int result[size];
    for (i = 0; i < size; i++)
    {
        array[i] = i;
    }
    int vnumcount = size * size;
    CombinationR(fp, result, size, 0, 0, vnumcount);
    CombinationL(fp, result, size, 0, 0, vnumcount);

    fclose(fp);
    return OK;
}

/***********************************
*函数名称：PuzzleInitAuto
*函数功能：随机生成一个有唯一解的size*size的二进制数独。
*输入参数：puzzle大小size
*返回参数：OK -> 成功
***********************************/
char **PuzzleInitAuto(int size) //挖洞自动生成棋局
{
    int x, y, i, j;
    int n = size * size / 8 + 2;
    char filename[100] = "PuzzleInitAuto.cnf";
    char **answer = (char **)malloc(sizeof(char *) * size); //为二维数组分配行
    for (i = 0; i < size; i++)
    {
        answer[i] = (char *)malloc(sizeof(char) * size);
    }
    while (1)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
                answer[i][j] = 'x';
        }
        srand(time(NULL));
        for (i = 0; i < n; i++)
        {
            x = rand() % size;
            y = rand() % size;
            if (i % 2)
                answer[x][y] = '1';
            else
                answer[x][y] = '0';
        }
        PuzzleTransform(answer, size, filename);
        Clset *cset = (Clset *)malloc(sizeof(Clset));
        ReadCset(cset, filename);

        if (DPLL(cset) == OK)
        {
            for (i = 0; i < size; i++)
            {
                for (j = 0; j < size; j++)
                {
                    if (cset->counter[i * size + j + 1][1] > 0)
                        answer[i][j] = '1';
                    else if (cset->counter[i * size + j + 1][1] < 0)
                        answer[i][j] = '0';
                    else
                        return NULL;
                }
            }
            break;
        }
    }

    char **a = (char **)malloc(sizeof(char *) * size); //为二维数组分配行
    for (i = 0; i < size; i++)
    {
        a[i] = (char *)malloc(sizeof(char) * size);
    }

    for (j = 0; j < size; j++)
        a[0][j] = 'x';
    for (j = 0; j < size; j++)
        a[j][0] = 'x';
    for (i = 1; i < size; i++)
    {
        for (j = 1; j < size; j++)
        {
            a[i][j] = answer[i][j];
        }
    }
    char tempfile[100] = "temp.cnf";

    for (i = 1; i < size; i++)
    {
        for (j = 1; j < size; j++)
        {
            if (a[i][j] == '1')
                a[i][j] = '0';
            else if (a[i][j] == '0')
                a[i][j] = '1';

            PuzzleTransform(a, size, tempfile);
            Clset *cset = (Clset *)malloc(sizeof(Clset));
            ReadCset(cset, tempfile);
            int f;
            f = DPLL(cset);
            if (f == OK)
            {
                if (a[i][j] == '1')
                    a[i][j] = '0';
                else if (a[i][j] == '0')
                    a[i][j] = '1';
            }
            else
            {
                a[i][j] = 'x';
            }

            free(cset);
        }
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
            printf("%c ", a[i][j]);
        printf("\n");
    }

    return answer;
}

/***********************************
*函数名称：puzzle
*函数功能：二进制数独游戏，自动生成等待用户输入并根据输入返回结果
*输入参数：五
*返回参数：OK -> 成功
***********************************/
int puzzle(void) //棋局功能
{
    int size, i, j, f = 0;
    printf("---------------------------------------------------------------------\n");
    printf("Sudoku!");
    printf("输入数独大小:\n");
    scanf("%d", &size);
    getchar();
    while (size != 4 && size != 6 && size != 8)
    {
        printf("只能输入4,6，或8.请再次输入：\n");
        scanf("%d", &size);
        getchar();
    }
    printf("\n数独游戏:\n");
    char filename[100] = "PuzzleInitAuto.cnf";
    char **answer = PuzzleInitAuto(size);
    char **a = (char **)malloc(sizeof(char *) * size); //为二维数组分配MAX行
    for (i = 0; i < size; i++)
    {
        a[i] = (char *)malloc(sizeof(char) * size);
    }
    while (1)
    {
        f = 0;
        printf("请输入答案:\n");
        printf("0 0 0 0 1 1 1 1  or 1 1 0 0 1 0 or 1 0 0 1");
        printf("\n");
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                scanf("%c", &a[i][j]);
                getchar();
            }
        }

        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (a[i][j] != answer[i][j])
                {
                    printf("答案错误!\n");
                    while (f != 1 && f != 2)
                    {
                        printf("-再次尝试请输入 1\n-查看答案并退出请输入 2\n");
                        scanf("%d", &f);
                        getchar();
                        printf("\n");
                        break;
                    }
                }
                if (f)
                    break;
            }
            if (f)
                break;
        }
        if (i == size && j == size)
        {
            printf("答案正确!\n标准答案为:\n\n");
            break;
        }

        else if (f == 2)
            break;
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
            printf("%c ", answer[i][j]);
        printf("\n");
    }
    printf("finished! Enter space please!");
    getchar();
    return OK;
}

