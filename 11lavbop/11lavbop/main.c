#pragma warning(disable : 4996)  
#include <stdio.h> 
#include <malloc.h> 
#include <string.h> 
#include <stdlib.h> 
#include <windows.h>
#define YES 
#define NO 0 
#define M 1000 
#include <locale.h>


void HUSCH(char* num);

void main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Rus");

	WORD foregroundColor;  
	WORD textAttribute; 

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

	foregroundColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;  //backgroundColor = BACKGROUND_RED; 

	textAttribute = foregroundColor;//| backgroundColor;// цвет текста 
	SetConsoleTextAttribute(hStdout, textAttribute); 

	FILE* f;
	int i; //  
	int j; //  
	int flag = 0;
	char* str = (char*)calloc(101, 1); //  

	if (argc == 2)
		strcpy(str, argv[1]);

	//f = fopen(local, "rt");
	auto a = "aaa\
abbbb\
";
	while ((f = fopen(str, "rt")) == NULL)
	{
		printf("Cannot open input file.\n");
		printf("Please enter a file address: ");
		gets_s(str, 100);
	}

	while (!feof(f))  

	{
		str = (char*)calloc(101, 1);
		fgets(str, M, f);
		str = (char*)realloc(str, strlen(str) + 1);
		i = 0; 
		
		while (*(str + i) != '\0')
		{
			if (*(str + i) == '/' && *(str + i + 1) == '*') flag = 1;

			if (*(str + i) == '/' && *(str + i + 1) == '/')
			{
				while (*(str + i) != '\n') { printf("%c", str[i++]); }
				break;
			}

			else if (*(str + i) == '"' && flag == 0 && * (str + i + 1) != '\'')

			{
				i++;

				char* num = (char*)calloc(100, 1);
				j = 0;
				while (*(str + i) != '"' || *(str + i - 1) == '\\')
				{
					num[j++] = str[i++];
				}     num = (char*)realloc(num, j + 1);
				i++;
				HUSCH(num);
			}
			else

			{
				printf("%c", str[i++]);
				if (*(str + i) == '*' && *(str + i + 1) == '/')
				{
					flag = 0;
				}
			}
		}
		free(str);
	}
	fclose(f);
	system("pause");
}

void HUSCH(char* num) {

	WORD foregroundColor0;
	WORD foregroundColor1; 
	WORD textAttribute;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	foregroundColor0 = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
	foregroundColor1 = FOREGROUND_RED | COMMON_LVB_UNDERSCORE;
	printf("\"");  textAttribute = foregroundColor1;//| backgroundColor; 
	SetConsoleTextAttribute(hStdout, textAttribute); 
	printf("%s", num); 
	textAttribute = foregroundColor0;//| backgroundColor;  
	SetConsoleTextAttribute(hStdout, textAttribute);
	printf("\""); 
	free(num);
}