#include"solver.h"

void createFirstRow(int a[])
{	//����������һ��
	int i, j;
	srand((unsigned)time(NULL));
	for (i = 0; i < 9; i++)
	{
		a[i] = rand() % 9 + 1;
		j = 0;
		while (j < i)
		{
			if (a[i] == a[j])
			{
				a[i] = rand() % 9 + 1;
				j = 0;
			}
			else j++;
		}
	}
}

int recursion(int **graph, int row, int col)
{	//�ݹ��������µ�������֣��ɹ�����1��ʧ�ܷ���0
	//���������ֽ����޶�
	if (row < 9 && col < 9)
	{
		int i, j;
		int mark[10];
		for (i = 0; i < 10; i++)
			mark[i] = 0;	//0��ʾ��ǰ���ֿ��1��ʾ������
		//������ͬ��
		for (i = 0; i < row; i++)
			mark[graph[i][col]] = 1;
		//������ͬ��
		for (i = 0; i < col; i++)
			mark[graph[row][i]] = 1;
		//������ͬ���
		for (i = row / 3 * 3; i <= row; i++)
		{
			if (i == row)
			{
				for (j = col / 3 * 3; j < col; j++)
					mark[graph[i][j]] = 1;
			}
			else
			{
				for (j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
					mark[graph[i][j]] = 1;
			}
		}
		//���еݹ�����
		int flag = 0;
		for (i = 1; i <= 9 && flag == 0; i++)
		{
			if (mark[i] == 0)
			{
				flag = 1;
				graph[row][col] = i;
				if (col == 8 && row != 8)
				{
					if (recursion(graph, row + 1, 0) == 1)
						return 1;
					else flag = 0;
				}
				else if (col != 8)
				{
					if (recursion(graph, row, col + 1) == 1)
						return 1;
					else flag = 0;
				}
			}
		}
		if (flag == 0)
		{
			graph[row][col] = 0;
			return 0;
		}
	}
	return 1;
}

void createSudoku(int** graph)
{	//��������
	createFirstRow(graph[0]);
	recursion(graph, 1, 0);
}

void createGame(int** gameGraph, int** graph, int hole)
{	//����������Ϸ���ڶ���
	srand((unsigned)time(NULL));
	//��������������������Ϸ
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			gameGraph[i][j] = graph[i][j];
	}
	//������ά���飬���ڼ�¼�Ƿ��ڵ���ͬ�Ķ�
	int** mark = (int**)malloc(sizeof(int*) * hole);
	for (i = 0; i < hole; i++)
	{
		mark[i] = (int*)malloc(sizeof(int) * 2);
		mark[i][0] = mark[i][1] = 0;
	}

	int flag;	//��¼�Ƿ�ɹ��ڶ�
	int k, t;
	for (i = 0; i < hole; i++)
	{
		j = rand() % 9;
		k = rand() % 9;
		flag = 0;
		for (t = 0; t < i; t++)
		{
			if (j == mark[t][0] && k == mark[t][1])
				flag = 1;
		}
		if (flag == 0)
		{
			gameGraph[j][k] = 0;
			mark[i][0] = j;
			mark[i][1] = k;
		}
		else i--;
	}
}

void showSudoku(int** graph)
{	//չʾ����
	int i, j;
	printf("\n");
	for (i = 0; i < 9; i++)
	{
		if (i == 3 || i == 6)
			printf("-----------------------------\n");
		for (j = 0; j < 9; j++)
		{
			if (j == 3 || j == 6)
				printf("|");
			if (graph[i][j] != 0)
				printf(" %d ", graph[i][j]);
			else printf(" / ");
		}
		printf("\n");
	}
	printf("\n");
}

void playSudoku(int** gameGraph, int** answer, int hole)
{	//������Ϸ����
	int row, col, value;
	while (hole != 0)
	{
		showSudoku(gameGraph);
		printf("������������е���ղ��������С��С�ֵ���ÿո������\n");
		printf("���˳�������-1���鿴��������0��\n");
		scanf("%d", &row);
		if (row == -1)
		{
			printf("��Ϸ����\n");
			return;
		}
		else if (row == 0)
		{
			showSudoku(answer);
			continue;
		}
		scanf("%d %d", &col, &value);
		row--;
		col--;
		if (row < 0 || row > 8 || col < 0 || col > 8)
			printf("��Чλ��\n");
		else
		{
			if (gameGraph[row][col] == '/')
				printf("�����޸ĳ�ʼ���\n");
			else
			{
				if (answer[row][col] == value)
				{
					gameGraph[row][col] = value;
					hole--;
					printf("������ȷ\n");
				}
				else
				{
					printf("�������\n");
				}
			}
		}
	}
	printf("�ɹ���\n");
}

void transferToCNF(int** graph, int hole)
{
	FILE* fp = fopen("sudoku.cnf", "w");
	int i, j, k, l;
	fprintf(fp, "p cnf %d %d \n", 999, 8829 + 81 - hole);
	//���������ĵ��Ӿ�
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (graph[i][j] != 0)
				fprintf(fp, "%d %d\n", (i + 1) * 100 + (j + 1) * 10 + graph[i][j], 0);
		}
	}
	//ÿһ�������о�����
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
			for (k = 1; k <= 9; k++)
				fprintf(fp, "%d ", i * 100 + j * 10 + k);
			fprintf(fp, "%d\n", 0);
		}
	}
	//��Լ��
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
			for (k = 1; k <= 8; k++)
			{
				for (l = k + 1; l <= 9; l++)
					fprintf(fp, "%d %d %d\n", 0 - (k * 100 + i * 10 + j), 0 - (l * 100 + i * 10 + j), 0);
			}
		}
	}
	//��Լ��
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
			for (k = 1; k <= 8; k++)
			{
				for (l = k + 1; l <= 9; l++)
					fprintf(fp, "%d %d %d\n", 0 - (i * 100 + k * 10 + j), 0 - (i * 100 + l * 10 + j), 0);
			}
		}
	}
	//����Լ��
	int x, y, z;
	for (z = 1; z <= 9; z++)
	{
		for (i = 0; i <= 2; i++)
		{
			for (j = 0; j <= 2; j++)
			{
				for (x = 1; x <= 3; x++)
				{
					for (y = 1; y <= 3; y++)
					{
						for (k = y + 1; k <= 3; k++)
							fprintf(fp, "%d %d %d\n", 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z), 0 - ((3 * i + x) * 100 + (3 * j + k) * 10 + z), 0);
					}
				}
			}
		}
	}
	for (z = 1; z <= 9; z++)
	{
		for (i = 0; i <= 2; i++)
		{
			for (j = 0; j <= 2; j++)
			{
				for (x = 1; x <= 3; x++)
				{
					for (y = 1; y <= 3; y++)
					{
						for (k = x + 1; k <= 3; k++)
						{
							for (l = 1; l <= 3; l++)
								fprintf(fp, "%d %d %d\n", 0 - ((3 * i + x) * 100 + (3 * j + y) * 10 + z), 0 - ((3 * i + k) * 100 + (3 * j + l) * 10 + z), 0);
						}
					}
				}
			}
		}
	}
	fclose(fp);
}