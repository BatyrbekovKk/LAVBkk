#pragma warning(disable : 4996) // ����� fopen
#include <stdio.h> 
#define MAXLINE 1024  
int main(void)
{  // ��������� �� ��������� ���� FILE ��� ������� � ��������� ������
	int N;
	int a = 0;
	int b;
	printf("N - quantity of symbol in line\n");
	printf("enter N please\n");
	scanf_s("%d", &N);
	char line[MAXLINE]; // ������� ������  
	char *ptr;
	// ������� ���� ��� ������  
	FILE *fpin = fopen("C:\\Users\\����\\source\\repos\\lavb8\\lavb8\test", "r");
	if (fpin == NULL)
	{
		printf("error: file not found\n"); // ���� ���� � ������� �����������, ������� ������
		return;
	}
	// ������� ���� ��� ������
	FILE* fpout = fopen("C:\\Users\\����\\source\\repos\\lavb8\\lavb8\result", "a"); // ������� ���� � �����������

	{
		while (!feof(fpin)) // ���� �� ����� �����  
		{   // ������ ������   
			ptr = fgets(line, MAXLINE, fpin);
			if (ptr == NULL)
				break; // ���� ��������
			while (*ptr != '\0')
			{
				if (a == N)
				{     // ���������� ������������ ����� ������  
					b = *ptr;
					*ptr = '\n';// ������ ������ "����� ������" 
					a = 0;
					fprintf(line[*ptr], fpout);
					ptr == b;

				}
				else
				{
					a++;
				}
				ptr++; // ���������� ��������� �� ������
			}
			fputs(line, fpout); // ������ ������  
		}
		fclose(fpin); // ������� ������� ����  
		fclose(fpout); // ������� �������� ���� 
		return 0;
	}
}