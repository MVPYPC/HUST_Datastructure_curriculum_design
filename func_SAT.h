/***********************************
*文件名称：func_SAT.h
*文件描述：关于求解SAT问题的函数。
*日    期：2020.03.01
*作    者：YPC
***********************************/
#include "def.h"

/***********************************
*函数名称：ClearVarNode
*函数功能：删除head指向的文字链表。
*输入参数：表头指针head
*返回参数：OK -> 删除成功
***********************************/
int ClearVarNode(VarNode *&head)
{
    VarNode *pfront, *pbehind;
    pbehind = head;
    while (pbehind)
    {
        pfront = pbehind;
        pbehind = pbehind->next;
        free(pfront);
    }
    head = NULL;
    return OK;
}

/***********************************
*函数名称：ClearClsNode
*函数功能：删除head指向的句子链表。
*输入参数：表头指针head
*返回参数：OK -> 删除成功
***********************************/
int ClearClsNode(ClsNode *&head)
{
    ClsNode *pfront, *pbehind;
    pbehind = head;
    while (pbehind)
    {
        pfront = pbehind;
        pbehind = pbehind->NextNode;
        ClearVarNode(pfront->NextAdj);
        free(pfront);
    }
    head = NULL;
    return OK;
}

/***********************************
*函数名称：TraverseDispose
*函数功能：按照规则，根据当前已确定变元消解化简子句集
*输入参数：子句集Clset
*返回参数：OK -> 消解成功
***********************************/
int TraverseDispose(Clset *cset)
{
    ClsNode *pforcl;
    ClsNode *qforcl;
    ClsNode *del;
    VarNode *pforlit;
    VarNode *qforlit;

    if (cset->Others)
    {
        pforcl = cset->Others;
        qforlit = pforcl->NextAdj;
        if (!pforcl->NextNode)
        {
            if (qforlit->literal == cset->counter[abs(qforlit->literal)][1])
            {
                ClearClsNode(pforcl);
            }
            else if (qforlit->literal == -(cset->counter[abs(qforlit->literal)][1]))
            {
                pforcl->NextAdj = qforlit->next;
                free(qforlit);
                qforlit = pforcl->NextAdj;
                pforcl->length--;
                if (pforcl->length == 1)
                {
                    pforcl->NextNode = cset->One;
                    cset->One = pforcl;
                    cset->Others = NULL;
                }
            }
            else
            {
                pforlit = qforlit->next;
                while (pforlit)
                {
                    if (pforlit->literal == cset->counter[abs(pforlit->literal)][1])
                    {
                        ClearClsNode(pforcl);
                        break;
                    }
                    else if (pforlit->literal == -(cset->counter[abs(pforlit->literal)][1]))
                    {
                        qforlit->next = pforlit->next;
                        free(pforlit);
                        pforlit = qforlit->next;
                        pforcl->length--;
                        if (pforcl->length == 1)
                        {
                            pforcl->NextNode = cset->One;
                            cset->One = pforcl;
                            cset->Others = NULL;
                            break;
                        }
                        continue;
                    }
                    qforlit = qforlit->next;
                    pforlit = pforlit->next;
                }//while
            }//else
        }//if
        else
        {
            qforcl = cset->Others;
            pforcl = qforcl->NextNode;
        label:             //标签：不用递增
            while (pforcl) //第一个先不管
            {
                qforlit = pforcl->NextAdj;
                pforlit = qforlit->next; //q在前p在后

                //第一个文字不管,后续循环
                while (pforlit)
                {
                    if (pforlit->literal == cset->counter[abs(pforlit->literal)][1])
                    {
                        del = pforcl;
                        pforcl = pforcl->NextNode;
                        qforcl->NextNode = pforcl;
                        ClearVarNode(del->NextAdj);
                        free(del);
                        goto label;
                    }
                    else if (pforlit->literal == -(cset->counter[abs(pforlit->literal)][1]))
                    {
                        qforlit->next = pforlit->next;
                        free(pforlit);
                        pforlit = qforlit->next;
                        pforcl->length--;
                        if (pforcl->length == 1)
                        {
                            del = pforcl;
                            pforcl = pforcl->NextNode;
                            qforcl->NextNode = pforcl;
                            del->NextNode = cset->One;
                            cset->One = del;
                            goto label;
                        }
                        continue;
                    }
                    qforlit = qforlit->next;
                    pforlit = pforlit->next;
                } //end of inner while

                //处理第一个文字
                pforlit = pforcl->NextAdj;
                if (pforlit->literal == cset->counter[abs(pforlit->literal)][1])
                {
                    del = pforcl;
                    pforcl = pforcl->NextNode;
                    qforcl->NextNode = pforcl;
                    ClearVarNode(del->NextAdj);
                    free(del);
                    goto label;
                }
                else if (pforlit->literal == -(cset->counter[abs(pforlit->literal)][1]))
                {
                    pforcl->NextAdj = pforlit->next;
                    free(pforlit);
                    pforcl->length--;
                    pforlit = pforcl->NextAdj;
                    if (pforcl->length == 1)
                    {
                        del = pforcl;
                        pforcl = pforcl->NextNode;
                        qforcl->NextNode = pforcl;
                        del->NextNode = cset->One;
                        cset->One = del;
                        goto label;
                    }
                }

                pforcl = pforcl->NextNode;
                qforcl = qforcl->NextNode;
            }//while

        } //end of while

        pforcl = cset->Others; //处理第一个句子
        if (pforcl)
        {
            qforlit = pforcl->NextAdj;
            pforlit = qforlit->next;
            int flagA = 0;
            while (pforlit)
            {
                if (pforlit->literal == cset->counter[abs(pforlit->literal)][1])
                {
                    cset->Others = pforcl->NextNode;
                    ClearVarNode(pforcl->NextAdj);
                    free(pforcl);
                    flagA = 1;
                    break;
                }
                else if (pforlit->literal == -(cset->counter[abs(pforlit->literal)][1]))
                {
                    qforlit->next = pforlit->next;
                    free(pforlit);
                    pforlit = qforlit->next;
                    pforcl->length--;
                    if (pforcl->length == 1)
                    {
                        cset->Others = pforcl->NextNode;
                        pforcl->NextNode = cset->One;
                        cset->One = pforcl;
                        flagA = 1;
                        break;
                    }
                    continue;
                }
                qforlit = qforlit->next;
                pforlit = pforlit->next;
            }
            if (!flagA)
            {
                pforlit = pforcl->NextAdj;
                if (pforlit->literal == cset->counter[abs(pforlit->literal)][1])
                {
                    cset->Others = pforcl->NextNode;
                    ClearVarNode(pforcl->NextAdj);
                    free(pforcl);
                }
                else if (pforlit->literal == -(cset->counter[abs(pforlit->literal)][1]))
                {
                    pforcl->NextAdj = pforlit->next;
                    free(pforlit);
                    pforlit = pforcl->NextAdj;
                    pforcl->length--;
                    if (pforcl->length == 1)
                    {
                        cset->Others = pforcl->NextNode;
                        pforcl->NextNode = cset->One;
                        cset->One = pforcl;
                    }
                }
            }//if
        }//if pforcl
    }//if(cset->Others)

    return OK;
}

/***********************************
*函数名称：ReadCset
*函数功能：按照cnf文件内容生成子句集Clset
*输入参数：待写入子句集Clset，cnf文件名filename[100]
*返回参数：OK -> 删除成功
***********************************/
int ReadCset(Clset *cset, char filename[100])
{
    FILE *fp;
    char temp[10];
    int i, lnum;    //i循环
    char a;         //a单个储存
    char line[256]; //解决前面的文字
    int feasible = 1;

    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("open %s failed\n", filename);
        return ERROR;
    }
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == 'c')
            continue;
        if (line[0] == 'p')
        {
            sscanf(line, "p cnf %d %d", &cset->vnum, &lnum);
            break;
        }
    }

    memset(temp, '\0', sizeof(temp));
    int k, l;

    cset->counter = (int **)malloc(sizeof(int *) * (cset->vnum + 1)); //为二维数组分配MAX行
    for (k = 0; k < (cset->vnum + 1); k++)
    { //为每列分配2个大小空间，0记录正负权值，1记录总出现，
        cset->counter[k] = (int *)malloc(sizeof(int) * 2);
    }
    //初始化
    for (k = 0; k < (cset->vnum + 1); k++)
    {
        for (l = 0; l < 2; l++)
        {
            cset->counter[k][l] = 0;
        }
    }
    i = 1;

    ClsNode *cl = (ClsNode *)malloc(sizeof(ClsNode));
    cl->length = 0;
    cl->NextAdj = NULL;
    cl->NextNode = NULL; //给第一个句子准备
    ClsNode *one = NULL;
    ClsNode *other = NULL;
    int SingleRecord[40] = {0}; //记录单个句子的出现次数，处理重复

    while ((fgets(&a, 2, fp)) != NULL)
    {

        if ((a <= '9' && a > '0') || (a == '0' && i != 1) || a == '-'){
            temp[i] = a;
            i++;
        }
        else if (a == ' ')
        {
            int x = atoi(temp + 1);
            int j, flag;
            flag = 1;
            for (j = 0; j < 40; j++)
            {
                if (SingleRecord[j] == x)
                {
                    flag = 0;
                    break;
                }
                else if (SingleRecord[j] == 0)
                {
                    SingleRecord[j] = x;
                    break;
                }
            }
            if (flag == 0)
            {
                i = 1;
                memset(temp, '\0', sizeof(temp));
                continue;
            }

            cl->length++;
            VarNode *lit = (VarNode *)malloc(sizeof(VarNode));
            lit->literal = x;
            lit->next = cl->NextAdj;
            cl->NextAdj = lit;
            i = 1;
            memset(temp, '\0', sizeof(temp));
        } //end if ' '
        else if (a == '0' && i == 1)
        {
            char f;
            fgets(&f, 2, fp);
            if (f != '\n')
                return ERROR;
            else
            {
                if (cl->length == 0)
                {
                    feasible = 0;
                    return UNFEASIBLE; //有空句子
                }
                else if (cl->length == 1)
                {
                    if (one == NULL)
                    {
                        one = cl;
                        cl->NextNode = NULL;
                    }
                    else
                    {
                        cl->NextNode = one;
                        one = cl;
                    }
                }
                else if (cl->length > 1)
                {
                    if (other == NULL)
                    {
                        other = cl;
                        cl->NextNode = NULL;
                    }
                    else
                    {
                        cl->NextNode = other;
                        other = cl;
                    }
                }
                else
                    return ERROR;
            }
            i = 1;
            ClsNode *t = (ClsNode *)malloc(sizeof(ClsNode));
            t->length = 0;
            t->NextAdj = NULL;
            t->NextNode = NULL;
            cl = t;
            memset(temp, '\0', sizeof(temp));
            memset(SingleRecord, 0, sizeof(SingleRecord));
        }
    }
    cset->One = one;
    cset->Others = other;
    fclose(fp);
    return OK;
}

/***********************************
*函数名称：DeepCopy
*函数功能：把当前Clset的内容复制到record，并按照此时状态计算变元偏量
*输入参数：模板子句集Clset，等待复制的空子句集的引用record
*返回参数：OK -> 删除成功
***********************************/
int DeepCopy(Clset *cset, Clset *&record) //深拷贝Cset
{
    //复制cset
    record = (Clset *)malloc(sizeof(Clset));
    record->vnum = cset->vnum;
    record->One = NULL;
    record->Others = NULL;
    int k;

    record->counter = (int **)malloc(sizeof(int *) * (record->vnum + 1)); //为二维数组分配MAX行
    for (k = 0; k < (record->vnum + 1); k++)
    { //为每列分配2个大小空间，0记录出现，1记录答案
        record->counter[k] = (int *)malloc(sizeof(int) * 2);
    }
    //初始化
    for (k = 0; k < (record->vnum + 1); k++)
    {
        record->counter[k][0] = 0;
        record->counter[k][1] = cset->counter[k][1];
    }

    ClsNode *pfrc = cset->Others;
    VarNode *pfrl = pfrc->NextAdj;
    while (pfrc)
    {
        ClsNode *add = (ClsNode *)malloc(sizeof(ClsNode));
        add->length = pfrc->length;
        add->NextAdj = NULL;
        pfrl = pfrc->NextAdj;
        while (pfrl)
        {
            VarNode *ad = (VarNode *)malloc(sizeof(VarNode));
            ad->literal = pfrl->literal;
            if (pfrl->literal > 0)
            {
                record->counter[pfrl->literal][0] = record->counter[pfrl->literal][0] + 1000 / pfrc->length;
            }
            else if (pfrl->literal < 0)
            {
                record->counter[-pfrl->literal][0] = record->counter[-pfrl->literal][0] - 1000 / pfrc->length;
            }
            else
                return ERROR;
            ad->next = add->NextAdj;
            add->NextAdj = ad;
            pfrl = pfrl->next;
        }
        add->NextNode = record->Others;
        record->Others = add;
        pfrc = pfrc->NextNode;
    } //复制结束
    return OK;
}

/***********************************
*函数名称：ChooseVar
*函数功能：给出一个猜测的变元值
*输入参数：当前消解完的子句集record
*返回参数：决策int（含正负）
***********************************/
int ChooseVar(Clset *record) //做出下一个假设变量的策略
{
    int i;

    for (i = 1; i < (record->vnum + 1); i++)
    {
        if (!record->counter[i][1])
            break;
    }

    if (record->counter[i][0] < 0)
        i = -i;
    return i;
}

/***********************************
*函数名称：InitClsNode
*函数功能：把只有value的单子句添加到Clset->One字句链表中
*输入参数：待添加结点的子句集引用Clset，决策完待添加的变元值value
*返回参数：OK -> 添加成功
***********************************/
int InitClsNode(Clset *&cset, int value)
{
    ClsNode *add = (ClsNode *)malloc(sizeof(ClsNode));
    add->NextAdj = (VarNode *)malloc(sizeof(VarNode));
    add->length = 1;
    add->NextAdj->next = NULL;
    add->NextAdj->literal = value;
    add->NextNode = cset->One;
    cset->One = add;
    return OK;
}

/***********************************
*函数名称：Display
*函数功能：展示读入成功的子句集中的所有子句
*输入参数：子句集Clset
*返回参数：OK -> 输出成功
***********************************/
int Display(Clset *cset) //展示子句
{
    int i;
    int counter = 1;
    printf("display the Clauses? -Yes enter 1  -No enter 0\n");
    scanf("%d", &i);
    getchar();
    if (!i)
        return OK;

    printf("----------------------------------------------------\n");
    printf("number of variety:%d\n", cset->vnum);
    ClsNode *p = cset->One;
    while (p)
    {
        printf("%d clause: %d\n", counter, p->NextAdj->literal);
        counter++;
        p = p->NextNode;
    }
    p = cset->Others;
    while (p)
    {
        printf("%d clause: ", counter);
        VarNode *pa = p->NextAdj;
        while (pa)
        {
            printf("%d ", pa->literal);
            pa = pa->next;
        }
        printf("\n");
        counter++;
        p = p->NextNode;
    }

    printf("display finished!\n");
    printf("----------------------------------------------------\n");
    return OK;
}

/***********************************
*函数名称：DPLL
*函数功能：DPLL循环求解
*输入参数：子句集的引用Clset
*返回参数：int（按文档中规定输出状态）
***********************************/
int DPLL(Clset *&cset)
{
    Clset *stack[200];
    int top = 0;
    stack[top] = cset;
    int flag = 0;
    while (1)
    {
        while (stack[top]->One)
        {
            flag = 0;
            ClsNode *pforcl = stack[top]->One;
            int literal, f;
            while (pforcl)
            {
                literal = pforcl->NextAdj->literal;
                if (stack[top]->counter[abs(literal)][1] == literal || stack[top]->counter[abs(literal)][1] == 0)
                    stack[top]->counter[abs(literal)][1] = literal;
                else if (stack[top]->counter[abs(literal)][1] == -literal)
                {
                    flag = 1;
                    break;
                }
                else
                    return ERROR;
                pforcl = pforcl->NextNode;
            }
            if (flag == 1)
                break;
            ClearClsNode(stack[top]->One);
            stack[top]->One = NULL;
            f = TraverseDispose(stack[top]); //所有含这个文字的句子处理
            if (f == UNFEASIBLE)
                return UNFEASIBLE;
        }

        if (flag == 1)
        {
            stack[top]->vnum = 0;
            ClearClsNode(stack[top]->Others);
            ClearClsNode(stack[top]->One);
            top--;
            if (top < 0)
                return UNFEASIBLE;
        }

        else if (!stack[top]->Others)
        {
            cset->counter = stack[top]->counter;
            return OK;
        }

        else
        {
            DeepCopy(stack[top], stack[top + 1]);
            top++;
            int value = ChooseVar(stack[top]);
            InitClsNode(stack[top], value);
            InitClsNode(stack[top - 1], -value);
        }
    }
}

/***********************************
*函数名称：SATsolver
*函数功能：简单的用户界面，以及输出结果
*输入参数：无
*返回参数：OK -> 输出成功
***********************************/
int SATsolver(void) //SAT求解器
{
    char question[100];
    Clset *cset = (Clset *)malloc(sizeof(Clset));
    printf("Enter the question filename:\n");
    scanf("%s", question);
    int f = ReadCset(cset, question);
    if (f == ERROR)
    {
        printf("error");
        getchar();
        return ERROR;
    }
    Display(cset);
    char answerfilename[100];
    int i;
    for (i = 0; i < 96; i++)
    {
        answerfilename[i] = question[i];
        if (answerfilename[i] == '.')
        {
            answerfilename[i + 1] = 'r';
            answerfilename[i + 2] = 'e';
            answerfilename[i + 3] = 's';
            answerfilename[i + 4] = '\0';
            break;
        }
    }
    if (i == 96)
    {
        printf("ERROR!");
        return ERROR;
    }

    if (f == UNFEASIBLE)
    {
        FILE *fp;
        if ((fp = fopen(answerfilename, "wb")) == NULL)
        {
            printf("save error!\n");
            return ERROR;
        }
        fprintf(fp, "s 0\n");
        fprintf(fp, "t 0\n");
        getchar();
    }
    else
    {
        clock_t start, finish;
        double totaltime;
        start = clock();
        f = DPLL(cset);
        finish = clock();
        totaltime = (double)(finish - start);
        FILE *fp;
        if ((fp = fopen(answerfilename, "wb")) == NULL)
        {
            printf("save error!\n");
            return ERROR;
        }
        if (f == OK)
        {
            fprintf(fp, "s 1\n");
            fprintf(fp, "v ");
            int i;
            for (i = 1; i <= cset->vnum; i++)
            {
                fprintf(fp, "%d ", cset->counter[i][1]);
            }
            fprintf(fp, "\n");
            fprintf(fp, "t %f", totaltime);
        }
        else
        {
            fprintf(fp, "s 0\n");
            fprintf(fp, "t %f", totaltime);
        }
        fclose(fp);
    }
    printf("finished! Enter spcae please!");
    getchar();
    return OK;
}