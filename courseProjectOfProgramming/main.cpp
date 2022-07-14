#include"sudoku.h"

int main()
{
	int option, i, hole, state;
	char fileName[50], s[50];
	CNF cnf;
	int** graph = (int**)malloc(sizeof(int*) * 10);
	for (i = 0; i < 10; i++)
		graph[i] = (int*)malloc(sizeof(int) * 10);
	int** gameGraph = (int**)malloc(sizeof(int*) * 10);
	for (i = 0; i < 10; i++)
		gameGraph[i] = (int*)malloc(sizeof(int) * 10);
	while (1)
	{	//һ���˵�
		iny:
		printf("--------------------Menu1-----------------------\n");
		printf("     1.SAT����                   2.��������\n");
		printf("     0.�˳�\n");
		printf("------------------------------------------------\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			printf("�������ļ�����");
			scanf("%s", fileName);
			if (createCNF(fileName, cnf) == 0)
				printf("CNF��ʽ�Ѵ������Ƿ�չʾ��ʽ���ݣ���������yes��no��");
			else
			{
				printf("δ�ҵ��ļ�\n");
				break;
			}
			while(1)
			{
				scanf("%s", s);
				if (strcmp(s, "yes") == 0)
				{
					showCNF(cnf);
					break;
				}
				else if (strcmp(s, "no") == 0)
					break;
				else printf("��Ч����\n");
			}
			printf("SAT�����ѽ��\n");
			showResult(cnf, fileName);
			break;
		case 2:
			createSudoku(graph);
			srand((unsigned)time(NULL));
			do {
				hole = rand() % 65;
			} while (hole < 20);
			createGame(gameGraph, graph, hole);
			printf("�������������\n");
			goto inx;
		case 0:
			printf("��лʹ�ã�\n");
			return 0;
		default:
			printf("��Чָ��\n");
		}
	}
	while (1)
	{	//�����Ķ����˵�
	inx:
		printf("--------------------Menu2-----------------------\n");
		printf("1.���Ϊcnf�ļ�              2.������Ϸ\n");
		printf("0.������һ��\n");
		printf("------------------------------------------------\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			transferToCNF(gameGraph, hole);
			printf("������cnf�ļ�\n");
			break;
		case 0:
			goto iny;
		case 2:
			playSudoku(gameGraph, graph, hole);
			goto iny;
			break;
		default:
			printf("��Чָ��\n");
		}
	}
	return 0;
}