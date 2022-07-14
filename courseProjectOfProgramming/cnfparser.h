#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct ClauseNode
{
	int clause;	//�Ӿ����
	int flag;	//��¼���Ӿ����Ƿ������Ӧ���֣�����Ϊ1�����ֱ�ɾȥΪ0
	struct ClauseNode* next;
}ClauseNode;	//�ڽ������ֱ�Ľ�� 

typedef struct Literal
{
	int value;	//ȡֵ��1��ʾT��0��ʾF��-1��ʾδ��ֵ
	ClauseNode* NextCla;	//��¼����������Щ�Ӿ��г��ֹ�
}Literal, * LiteralArray;	//�����ڽӱ��ͷ

typedef struct LiteralNode
{
	int literal;	//�������ݣ�����������ʾ�Ƿ�ȡ���ǡ�
	int flag;	//��¼�������Ƿ񱻴Ӹ��Ӿ���ȥ����0Ϊȥ����1Ϊ����
	struct LiteralNode* next;
}LiteralNode;	//�ڽ����Ӿ��Ľ�� 

typedef struct Clause
{
	int flag;	//��¼���Ӿ��Ƿ�ȥ����0Ϊȥ����1Ϊ����
	LiteralNode* NextLit;
}Clause, * ClauseArray;	//�Ӿ��ڽӱ��ͷ 

typedef struct CNF
{
	int LitNum;
	int ClaNum;
	ClauseArray AllClause;
	LiteralArray AllLiteral;
	int* vSequence;	//��¼��������˳��
}CNF;

//�Ż�ǰ
//typedef struct CNF
//{
//	int LitNum;
//	int ClaNum;
//	ClauseArray AllClause;
//	LiteralArray AllLiteral;
//}CNF;

int clauseLength(Clause clause)
{	//���Ӿ䳤�ȣ������Ӿ䳤��
	LiteralNode* p = NULL;
	int l = 0;
	for (p = clause.NextLit; p != NULL; p = p->next)
		l++;
	return l;
}

void sortV(CNF &cnf)
{	//���Ե����˳��
	struct J
	{
		int lit;	//�������
		double j;	//Jֵ
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
	//ð������
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
{	//����cnf��ʽ���ɹ�����0��ʧ�ܷ���-1
	strcat(fileName, ".cnf");
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
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
	//��ʼ��
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
	//��ȡ�ļ�������CNF
	ClauseNode* pCNode = NULL;
	LiteralNode* pLNode = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		do {
			fscanf(fp, "%d", &l);
			if (l != 0)
			{
				//�������ڽӱ�Ĵ���
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
				//���Ӿ��ڽӱ�Ĵ���
				pLNode = (LiteralNode*)malloc(sizeof(LiteralNode));
				pLNode->literal = l;
				pLNode->flag = 1;
				pLNode->next = cnf.AllClause[i].NextLit;
				cnf.AllClause[i].NextLit = pLNode;
			}
		} while (l != 0);
	}
	printf("CNF��ʽ�Ѵ���\n");
	fclose(fp);
	cnf.vSequence = (int*)malloc(sizeof(int) * (cnf.LitNum + 3));
	sortV(cnf);
	return 0;
}

void showCNF(CNF cnf)
{	//չʾcnf��ʽ����
	printf("�˹�ʽ ��������%d �Ӿ�����%d\n", cnf.LitNum, cnf.ClaNum);
	printf("��ʽ���ݣ�\n");
	int i;
	LiteralNode* p = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		for (p = cnf.AllClause[i].NextLit; p != NULL; p = p->next)
			printf("%d ", p->literal);
		printf("\n");
	}
}