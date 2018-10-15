#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include"sudoku.h"

int findPossibleNum(int(*sudoku)[LENGTH], int x, int y, int *check);

void writeBoard(int(*sudoku)[LENGTH]);

void printBoard(int(*sudoku)[LENGTH]);

int solve(int(*sudoku)[LENGTH], int x, int y);

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textColor(boolean oldOrNew);
int solveNum = 0, findTimes=0;

int main()
{
	int sudoku[LENGTH][LENGTH];
	int i, j, num = 0;
	FILE *input = fopen("question-c.txt", "r");

	for (i = 0; i<LENGTH; i++)
	{
		for (j = 0; j<LENGTH; j++)
		{
			if (LENGTH < 10) fscanf(input, "%01d", &sudoku[i][j]);
			else fscanf(input, "%d", &sudoku[i][j]);
		}
	}
	setcursortype(NOCURSOR);

	printBoard(sudoku);
	solve(sudoku, 0, 0);
	gotoxy(0, LENGTH + LENGTH_BOX+1);
	printf("%d번 탐색\n", findTimes);
	if (solveNum<=0) printf("불가능\n");
	else printf("%d개의 해\n", solveNum);
	fclose(input);

	return 0;
}

int findPossibleNum(int(*sudoku)[LENGTH], int x, int y, int *check)
{
	int i, j, possibleSum=0;

	for (i = 0; i<LENGTH; i++)
	{
		if (sudoku[x][i] != 0) check[sudoku[x][i] - 1] = 1;

		if (sudoku[i][y] != 0) check[sudoku[i][y] - 1] = 1;
	}

	for (i = (x / LENGTH_BOX) * LENGTH_BOX; i - ((x / LENGTH_BOX)*LENGTH_BOX) < LENGTH_BOX; i++)
	{
		for (j = (y / LENGTH_BOX) * LENGTH_BOX; j - ((y / LENGTH_BOX)*LENGTH_BOX) < LENGTH_BOX; j++)
		{
			if (sudoku[i][j] == 0) continue;

			check[sudoku[i][j] - 1] = 1;
		}
	}

	for (i = 0; i < LENGTH; i++) {
		possibleSum += check[i];
	}

	return possibleSum;
}

void writeBoard(int(*sudoku)[LENGTH])
{
	int i, j;

	FILE *output = fopen("answer-c.txt", "w");

	for (i = 0; i<LENGTH; i++)
	{
		for (j = 0; j<LENGTH; j++)
		{
			if(LENGTH<10)fprintf(output, "%d", sudoku[i][j]);
			else fprintf(output, "%02d ", sudoku[i][j]);
			if ((j+1) % LENGTH_BOX == 0) fprintf(output, " ");
		}

		if ((i+1) % LENGTH_BOX == 0) fprintf(output, "\n");

		fprintf(output, "\n");
	}

	fclose(output);
}

void printBoard(int(*sudoku)[LENGTH])
{
	int i, j;
	gotoxy(0, 0);
	for (i = 0; i<LENGTH; i++)
	{
		if (i % LENGTH_BOX == 0)
		{
			printf("+");

			for (j = 0; j < LENGTH; j++)
			{
				printf("-");
				if (LENGTH >= 10)printf("--");
				if ((j+1) % LENGTH_BOX == 0)printf("+");
			}

			printf("\n");
		}

		printf("|");

		for (j = 0; j<LENGTH; j++)
		{
			if (sudoku[i][j] == 0) {
				printf(" ");
				if (LENGTH >= 10)printf("  ");
			}
			else
			{
				//textColor(sudoku[i][j] > 0);
				if(LENGTH<10)printf("%d", sudoku[i][j]);
				else printf("%02d ", sudoku[i][j]);
				//textColor(1);
			}
			if ((j+1) % LENGTH_BOX == 0) printf("|");
		}

		printf("\n");
	}

	printf("+");

	for (j = 0; j < LENGTH; j++)
	{
		printf("-");
		if (LENGTH >= 10)printf("--");
		if ((j+1) % LENGTH_BOX == 0) printf("+");
	}

	printf("\n");
}

int solve(int(*sudoku)[LENGTH], int x, int y)
{
	int i, possibleSum=0;
	int *check=(int *)malloc(sizeof(int)*LENGTH);
	for (i = 0; i < LENGTH; i++)
		check[i] = 0;
	if (sudoku[x][y] > 0)
	{
		free(check);
		if (x == LENGTH - 1 && y == LENGTH - 1) {
			solveNum++;
			writeBoard(sudoku);
			
			gotoxy(0, 1 + LENGTH + LENGTH_BOX);
			system("pause");
			gotoxy(0, 1 + LENGTH + LENGTH_BOX);
			printf("                                     ");
			return 1;
		}
		else return solve(sudoku, (y == LENGTH - 1) ? x + 1 : x, (y == LENGTH - 1) ? 0 : y + 1);
	}

	possibleSum=findPossibleNum(sudoku, x, y, check);

	do
	{
		findTimes++;
		for (i = sudoku[x][y]; check[i] != 0 && i < LENGTH; i++) {}

		if (i == LENGTH)
		{
			sudoku[x][y] = 0;
			if (LENGTH < 10) {
				gotoxy(y + 1 + y / LENGTH_BOX, 1 + x + x / LENGTH_BOX);
				printf(" ");
			}
			else {
				gotoxy(1 + 3*y + y / LENGTH_BOX, 1 + x + x / LENGTH_BOX);
				printf("  ");
			}
			free(check);
			return 0;
		}

		sudoku[x][y] = i + 1;
		possibleSum--;
		
		if(LENGTH<10)gotoxy(y + 1 + y / LENGTH_BOX, 1 + x + x / LENGTH_BOX);
		else gotoxy(1 +3* y + y / LENGTH_BOX, 1 + x + x / LENGTH_BOX);
		textColor(0);
		if(LENGTH<10)printf("%d", i + 1);
		else printf("%02d", i + 1);
		textColor(1);
		if (x == LENGTH - 1 && y == LENGTH - 1 ) {
			writeBoard(sudoku);
			free(check);
			solveNum++;
			
			gotoxy(0, 1 + LENGTH + LENGTH_BOX);
			system("pause");
			gotoxy(0, 1 + LENGTH + LENGTH_BOX);
			printf("                                    ");
			if (LENGTH < 10) {
				gotoxy(y + 1 + y / LENGTH_BOX, x + 1 + x / LENGTH_BOX);
				printf(" ");
			}
			else {
				gotoxy(3*y + 1 + y / LENGTH_BOX, x + 1 + x / LENGTH_BOX);
				printf("  ");
			}
			sudoku[x][y] = 0;
			return possibleSum<=0;
		}
	} while (!solve(sudoku, (y == LENGTH - 1) ? x + 1 : x, (y == LENGTH - 1) ? 0 : y + 1)||possibleSum>0);
	free(check);
	return 1;
}

void textColor(boolean oldOrNew)
{
	int color = oldOrNew ? 15 : 7; //15 is old(white), 7 is new(grey)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
