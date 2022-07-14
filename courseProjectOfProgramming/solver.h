#include"cnfparser.h"
#include<time.h>

int isUnitClause(Clause clause)
{	//判断当前子句是否为单子句，是返回1，否返回0
	LiteralNode* p = NULL;
	int num = 0;	//记录子句中的文字数
	if (clause.flag == 0)
		return 0;
	for (p = clause.NextLit; p != NULL; p = p->next)
	{
		if (p->flag == 1)
			num++;
	}
	if (num == 1)
		return 1;
	else return 0;
}

int unitInCNF(CNF cnf)
{	//判断CNF公式中是否存在单子句，存在则返回单子句序号，不存在返回0
	int i;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		if (cnf.AllClause[i].flag == 0)
			continue;
		else
		{
			if (isUnitClause(cnf.AllClause[i]))
				return i;
		}
	}
	return 0;	//未找到单子句
}

int isClauseEmpty(Clause clause)
{	//判断该子句是否为空子句，是返回1，不是返回0
	LiteralNode* p = NULL;
	for (p = clause.NextLit; p != NULL; p = p->next)
	{
		if (p->flag == 1)
			return 0;
	}
	return 1;
}

int isEmptyClauseExist(CNF cnf)
{	//判断cnf中是否存在空子句，存在返回1，不存在返回0
	int i;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		if (isClauseEmpty(cnf.AllClause[i]) == 1)
			return 1;
	}
	return 0;
}

int isCNFEmpty(CNF cnf)
{	//判断cnf是否为空集，是返回1，不是返回0
	int i;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		if (cnf.AllClause[i].flag == 1)
			return 0;
	}
	return 1;
}

int vChoose(CNF cnf)
{	//选择一个变元，返回变元序号
	int i;
	for (i = 0; i < cnf.LitNum; i++)
	{
		if (cnf.AllLiteral[cnf.vSequence[i]].value == -1)
			return cnf.vSequence[i];
	}
	return 0;	//所有变元已赋值完毕
}

//优化前
//int vChoose(CNF cnf)
//{	//选择一个变元，返回变元序号
//	int i;
//	for (i = 0; i < cnf.LitNum; i++)
//	{
//		if (cnf.AllLiteral[i].value == -1)
//			return i;
//	}
//	return 0;	//所有变元已赋值完毕
//}

void firstBranch(CNF& cnf, int v)
{	//选择变元后对cnf进行处理
	//处理文字邻接表
	cnf.AllLiteral[v].value = 1;
	//处理子句邻接表
	int i;
	LiteralNode* p = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		if (cnf.AllClause[i].flag == 0)
			continue;
		for (p = cnf.AllClause[i].NextLit; p != NULL; p = p->next)
		{
			if (p->flag == 0)
				continue;
			else if (p->literal == v)
				cnf.AllClause[i].flag = 0;
			else if (p->literal + v == 0)
				p->flag = 0;
		}
	}
}

void secondBranch(CNF& cnf, int v)
{	//进入另一分支
	//处理文字邻接表
	cnf.AllLiteral[v].value = 0;
	//处理子句邻接表
	int i;
	LiteralNode* p = NULL;
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		if (cnf.AllClause[i].flag == 0)
			continue;
		for (p = cnf.AllClause[i].NextLit; p != NULL; p = p->next)
		{
			if (p->flag == 0)
				continue;
			else if (p->literal == v)
				p->flag = 0;
			else if (p->literal + v == 0)
				cnf.AllClause[i].flag = 0;
		}
	}
}

void createCopy(CNF& copy, CNF cnf)
{	//制作副本
	//初始化
	copy.ClaNum = cnf.ClaNum;
	copy.LitNum = cnf.LitNum;
	copy.AllClause = (ClauseArray)malloc(sizeof(Clause) * (cnf.ClaNum + 5));
	copy.AllLiteral = (LiteralArray)malloc(sizeof(Literal) * (cnf.LitNum + 5));
	ClauseNode* pCNode = NULL, *p = NULL;
	LiteralNode* pLNode = NULL, *q = NULL;
	int i;
	//处理子句邻接表
	for (i = 1; i <= cnf.ClaNum; i++)
	{
		copy.AllClause[i].flag = cnf.AllClause[i].flag;
		copy.AllClause[i].NextLit = NULL;
		for (q = cnf.AllClause[i].NextLit; q != NULL; q = q->next)
		{
			pLNode = (LiteralNode*)malloc(sizeof(LiteralNode));
			pLNode->flag = q->flag;
			pLNode->literal = q->literal;
			pLNode->next = copy.AllClause[i].NextLit;
			copy.AllClause[i].NextLit = pLNode;
		}
	}
	//处理文字邻接表
	for (i = 1; i <= cnf.LitNum; i++)
	{
		copy.AllLiteral[i].value = cnf.AllLiteral[i].value;
		copy.AllLiteral[i].NextCla = NULL;
		for (p = cnf.AllLiteral[i].NextCla; p != NULL; p = p->next)
		{
			pCNode = (ClauseNode*)malloc(sizeof(ClauseNode));
			pCNode->clause = p->clause;
			pCNode->flag = p->flag;
			pCNode->next = copy.AllLiteral[i].NextCla;
			copy.AllLiteral[i].NextCla = pCNode;
		}
	}
}

int DPLL(CNF& cnf)
{	//DPLL算法, 成功解决返回1，失败返回0
	while (unitInCNF(cnf))
	{
		int l = unitInCNF(cnf);	//单子句序号
		//找到单子句中文字内容
		LiteralNode* q = NULL;
		int lit;
		for (q = cnf.AllClause[l].NextLit; q != NULL; q = q->next)
		{
			if (q->flag != 0)
			{
				lit = q->literal;
				break;
			}
		}
		//依据单子句规则化简
		//处理文字邻接表
		ClauseNode* p1 = NULL, *p2 = NULL;
		if (lit > 0)
		{
			cnf.AllLiteral[lit].value = 1;
			p1 = cnf.AllLiteral[lit].NextCla;
		}
		else
		{
			cnf.AllLiteral[-lit].value = 0;
			p1 = cnf.AllLiteral[-lit].NextCla;
		}
		//处理子句邻接表
		for (p2 = p1; p2 != NULL; p2 = p2->next)
		{
			for (q = cnf.AllClause[p2->clause].NextLit; q != NULL; q = q->next)
			{
				if (q->literal == lit)
					cnf.AllClause[p2->clause].flag = 0;	//将子句去掉
			}
		}
		if (isCNFEmpty(cnf) == 1)
			return 1;
		else
		{
			for (p2 = p1; p2 != NULL; p2 = p2->next)
			{
				for (q = cnf.AllClause[p2->clause].NextLit; q != NULL; q = q->next)
				{
					if (q->literal == -lit)
						q->flag = 0;
				}
			}
		}
		if (isCNFEmpty(cnf) == 1)
			return 1;
		else if (isEmptyClauseExist(cnf) == 1)
			return 0;
	}
	int v = vChoose(cnf);
	CNF cnf1;	//cnf副本
	createCopy(cnf1, cnf);
	firstBranch(cnf, v);
	if (DPLL(cnf) == 1)
		return 1;
	//回溯到对v执行分支策略的初态
	createCopy(cnf, cnf1);
	secondBranch(cnf, v);
	if (DPLL(cnf) == 1)
		return 1;
	else
	{
		createCopy(cnf, cnf1);
		return 0;
	}
}

void showResult(CNF cnf, char fileName[])
{	//输出cnf结果
	strcat(fileName, ".res");
	FILE *fp = fopen(fileName, "w");
	clock_t start_t, end_t;
	int state;
	start_t = clock();
	state = DPLL(cnf);
	end_t = clock();
	if (state == 0)
	{
		fprintf(fp, "s 0\n");
		return;
	}
	else fprintf(fp, "s 1\n");
	double t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	fprintf(fp, "v ");
	int i;
	for (i = 1; i <= cnf.LitNum; i++)
	{
		if (cnf.AllLiteral[i].value == 1 || cnf.AllLiteral[i].value == -1)
			fprintf(fp, "%d ", i);
		else if (cnf.AllLiteral[i].value == 0)
			fprintf(fp, "-%d ", i);
	}
	fprintf(fp, "\nt %f", t * 1000);
	fclose(fp);
}