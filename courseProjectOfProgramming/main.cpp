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
	{	//一级菜单
		iny:
		printf("--------------------Menu1-----------------------\n");
		printf("     1.SAT问题                   2.数独问题\n");
		printf("     0.退出\n");
		printf("------------------------------------------------\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			printf("请输入文件名：");
			scanf("%s", fileName);
			if (createCNF(fileName, cnf) == 0)
				printf("CNF公式已创建，是否展示公式内容？（请输入yes或no）");
			else
			{
				printf("未找到文件\n");
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
				else printf("无效命令\n");
			}
			printf("SAT问题已解决\n");
			showResult(cnf, fileName);
			break;
		case 2:
			createSudoku(graph);
			srand((unsigned)time(NULL));
			do {
				hole = rand() % 65;
			} while (hole < 20);
			createGame(gameGraph, graph, hole);
			printf("已生成数独格局\n");
			goto inx;
		case 0:
			printf("感谢使用！\n");
			return 0;
		default:
			printf("无效指令\n");
		}
	}
	while (1)
	{	//数独的二级菜单
	inx:
		printf("--------------------Menu2-----------------------\n");
		printf("1.输出为cnf文件              2.进行游戏\n");
		printf("0.返回上一级\n");
		printf("------------------------------------------------\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			transferToCNF(gameGraph, hole);
			printf("已生成cnf文件\n");
			break;
		case 0:
			goto iny;
		case 2:
			playSudoku(gameGraph, graph, hole);
			goto iny;
			break;
		default:
			printf("无效指令\n");
		}
	}
	return 0;
}