#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define OK 1
#define ERROR -1
#define UNFEASIBLE -2
#define TRUE 1
#define FALSE 0
#define MAX 50
#ifndef _TEST_H_
#define _TEST_H_

typedef struct VarNode
{
    int literal;
    struct VarNode *next;
} VarNode; //句子的表结点，文字

typedef struct ClsNode
{
    int length;
    struct ClsNode *NextNode;
    VarNode *NextAdj;
} ClsNode; //句子链表

typedef struct Clset
{
    int **counter; //记录表
    ClsNode *One;    //只有一个文字的句子
    ClsNode *Others; //其他句子
    int vnum;
} Clset;

int DPLL(Clset *&cset);
int ClearVarNode(VarNode *&head);
int ClearClsNode(ClsNode *&head);
int TraverseDispose(Clset *cset); //根据确定变元简化sat
ClsNode *InitClsNode();   //初始化句子结点
int ReadCset(Clset *cset, char filename[100]);  //从cnf文件读入
int DeepCopy(Clset *cset, Clset *&record);   //深拷贝
int ChooseVar(Clset *record);    //搜索最佳值
int InitClsNode(Clset *&cset, int value);    //初始化新策略结点
int PuzzleTransform(char **puzzle, int size, char filename[100]);                        //生成二进制数组的cnf文件
int Combination2(FILE *fp, int array[], int result[], int size, int index, int deep);    //针对第二个条件排列组合构建子句
int CombinationR(FILE *fp, int result[], int size, int index, int deep, int &vnumcount); //针对第三个条件行排列组合构建子句
int CombinationL(FILE *fp, int result[], int size, int index, int deep, int &vnumcount); //针对第三个条件列排列组合构建子句
char **PuzzleInitAuto(int size);        //自动随机生成数组
int puzzle(void);          //数独游戏
int SATsolver(void);                   //SAT求解
int Display(Clset *cset);           //展示子句


#endif