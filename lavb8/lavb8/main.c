#pragma warning(disable : 4996) // чиним fopen
#include <stdio.h> 
#define MAXLINE 1024  
int main(void)
{  // указатели на структуру типа FILE для входной и выходного файлов
	int N;
	int a = 0;
	int b;
	printf("N - quantity of symbol in line\n");
	printf("enter N please\n");
	scanf_s("%d", &N);
	char line[MAXLINE]; // текущая строка  
	char *ptr;
	// открыть файл для чтения  
	FILE *fpin = fopen("C:\\Users\\Коля\\source\\repos\\lavb8\\lavb8\test", "r");
	if (fpin == NULL)
	{
		printf("error: file not found\n"); // если файл с данными отсутствует, выводим ошибку
		return;
	}
	// открыть файл для записи
	FILE* fpout = fopen("C:\\Users\\Коля\\source\\repos\\lavb8\\lavb8\result", "a"); // создаем файл с результатом

	{
		while (!feof(fpin)) // цикл до конца файла  
		{   // чтение строки   
			ptr = fgets(line, MAXLINE, fpin);
			if (ptr == NULL)
				break; // файл исчерпан
			while (*ptr != '\0')
			{
				if (a == N)
				{     // достигнута максимальная длина строки  
					b = *ptr;
					*ptr = '\n';// ставим символ "конец строки" 
					a = 0;
					fprintf(line[*ptr], fpout);
					ptr == b;

				}
				else
				{
					a++;
				}
				ptr++; // продвигаем указатель по строке
			}
			fputs(line, fpout); // запись строки  
		}
		fclose(fpin); // закрыть входной файл  
		fclose(fpout); // закрыть выходной файл 
		return 0;
	}
}
