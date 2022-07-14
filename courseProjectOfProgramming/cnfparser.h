#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct ClauseNode
{
	int clause;	//子句序号
	int flag;	//记录该子句中是否包含对应文字，包含为1，文字被删去为0
	struct ClauseNode* next;
}ClauseNode;	//邻接于文字表的结点 

typedef struct Literal
{
	int value;	//取值，1表示T，0表示F，-1表示未赋值
	ClauseNode* NextCla;	//记录该文字在哪些子句中出现过
}Literal, * LiteralArray;	//文字邻接表表头

typedef struct LiteralNode
{
	int literal;	//文字内容，其中正负表示是否取“非”
	int flag;	//记录该文字是否被从该子句中去掉，0为去掉，1为存在
	struct LiteralNode* next;
}LiteralNode;	//邻接于子句表的结点 

typedef struct Clause
{
	int flag;	//记录该子句是否被去掉，0为去掉，1为存在
	LiteralNode* NextLit;
}Clause, * ClauseArray;	//子句邻接表表头 

typedef struct CNF
{
	int LitNum;
	int ClaNum;
	ClauseArray AllClause;
	LiteralArray AllLiteral;
	int* vSequence;	//记录变量决策顺序
}CNF;

//优化前
//typedef struct CNF
//{
//	int LitNum;
//	int ClaNum;
//	ClauseArray AllClause;
//	LiteralArray AllLiteral;
//}CNF;

int clauseLength(Clause clause)
{	//求子句长度，返回子句长度
	LiteralNode* p = NULL;
	int l = 0;
	for (p = clause.NextLit; p != NULL; p = p->next)
		l++;
	return l;
}

void sortV(CNF &cnf)
{	//求边缘决策顺序
	struct J
	{
		int lit;	//文字序号
		double j;	//J值
	};
	struct J* litJ = (struct J*)malloc(sizeof(struct J) * (cnf.LitNum + 3));
	struct J t;
	int i, j, n;
	ClauseNode* p = NULL;
	for (i = 1; i <= cnf.LitNum; i++)
	{
		litJ[i].lit = i;
		litJ[i].j = 0;
		for (p = cnf.AllLiteral[i].NextCla; p != NULL; p = p->next)
		{
			n = clauseLength(cnf.AllClause[p->clause]);
			litJ[i].j += pow(2, -n);
		}
	}
	//冒泡排序
	for (i = 1; i < cnf.LitNum; i++)
	{
		for (j = i; j < cnf.LitNum; j++)
		{
			if (litJ[j].j < litJ[j + 1].j)
			{
				t = litJ[j];
				litJ[j] = litJ[j + 1];
				litJ[j + 1] = t;
			}
		}
	}
	for (i = 1; i <= cnf.LitNum; i++)
		cnf.vSequence[i - 1] = litJ[i].lit;
}

int createCNF(char fileName[], CNF &cnf)
{	//创建cnf公式，成功返回0，失败返回-1
	strcat(fileName, ".cnf");
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return -1;
	}
	char s[50];
	int i;
	while (fscanf(fp, "%s", s) != EOF)
	{
		if (strcmp(s, "p") == 0)
			break;
	}
	while (fscanf(fp, "%s", s) != EOF)
	{
		if (strcmp(s, "cnf") == 0)
		{
			fscanf(fp, "%d", &cnf.LitNum);
			fscanf(fp, "%d", &cnf.ClaNum);
			break;
		}
	}
	//初始化
	cnf.AllLiteral = (LiteralArray)malloc(sizeof(Literal) * (cnf.LitNum + 5));
	cnf.AllClause = (ClauseArray)malloc(sizeof(Clause) * (cnf.ClaNum + 5));
	int l;
	for (i = 1; i <= cnf.LitNum; i++)
	{
		cnf.AllLiteral[i].NextCla = NULL;
		cnf.AllLiteral[i].value = -1;
	}
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		cnf.AllClause[i].NextLit = NULL;
		cnf.AllClause[i].flag = 1;
	}
	//读取文件，建立CNF
	ClauseNode* pCNode = NULL;
	LiteralNode* pLNode = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		do {
			fscanf(fp, "%d", &l);
			if (l != 0)
			{
				//对文字邻接表的处理
				pCNode = (ClauseNode*)malloc(sizeof(ClauseNode));
				pCNode->clause = i;
				pCNode->flag = 1;
				if (l > 0)
				{
					pCNode->next = cnf.AllLiteral[l].NextCla;
					cnf.AllLiteral[l].NextCla = pCNode;
				}
				else
				{
					pCNode->next = cnf.AllLiteral[-l].NextCla;
					cnf.AllLiteral[-l].NextCla = pCNode;
				}
				//对子句邻接表的处理
				pLNode = (LiteralNode*)malloc(sizeof(LiteralNode));
				pLNode->literal = l;
				pLNode->flag = 1;
				pLNode->next = cnf.AllClause[i].NextLit;
				cnf.AllClause[i].NextLit = pLNode;
			}
		} while (l != 0);
	}
	printf("CNF公式已创建\n");
	fclose(fp);
	cnf.vSequence = (int*)malloc(sizeof(int) * (cnf.LitNum + 3));
	sortV(cnf);
	return 0;
}

void showCNF(CNF cnf)
{	//展示cnf公式内容
	printf("此公式 文字数：%d 子句数：%d\n", cnf.LitNum, cnf.ClaNum);
	printf("公式内容：\n");
	int i;
	LiteralNode* p = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		for (p = cnf.AllClause[i].NextLit; p != NULL; p = p->next)
			printf("%d ", p->literal);
		printf("\n");
	}
}