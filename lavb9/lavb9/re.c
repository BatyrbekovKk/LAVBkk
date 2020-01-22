#define _CRT_SECURE_NO_WARNINGS // Егор ТРетьяков 9 лаба
#include <stdio.h>
#include <locale.h>
#define LEN 1000
#define NUM_OF_STRINGS 256
struct SRT
{
	int length; //Äëèííà ñòðîêè
	char *adr; //Àäðåññ ñòðîêè
};
int main()
{
	setlocale(LC_ALL, "Russian");
	struct SRT lines[NUM_OF_STRINGS]; //ìàññèâ ñòðóêòóð
	struct SRT tmp;  //âðåìåííàÿ ñòðóêòóðà
	int i;
	int j;
	int n; //ñ÷åò÷èê ñòðîê
	char str[NUM_OF_STRINGS][LEN]; //áóôåð òåêñòà
	char *ptr;
	int counter;
	//íà÷àëüíûå îïðåäåëåíèÿ ïåðåìåííûõ è ÷òåíèå èç ôàéëà
	i = 0;
	j = 0;
	n = 0;
	counter = 0;
	FILE *fpin;
	fpin = fopen("input.txt", "r");
	if (fpin == NULL) //ïðîâåðêà êîððåêòíîñòè îòêðûòèÿ ôàéëà
		return 1;
	while (ptr = fgets(str[i], sizeof(str[i]), fpin)) //àíàëèç ñòðîê â ôàéëå
	{
		sscanf(str[i], "%[^\n]", str[i]);
		while (*ptr)
		{
			counter++;
			ptr++;
		}
		n++;
		// ñ÷èòàåì êîëè÷åñòâî ñòðîê â ôàéëå
			// çàïèñü èíôîðìàöèè â ñîîòâåòñâóþùèå ïîëÿ ñòðóêòóðû
		lines[i].adr = str[i];
		lines[i].length = counter;
		//âûâîäèì ñòàòèñòèêó ïî äëèíå ñòðîê
		printf("In string %d symbols %d\n", i, lines[i++].length);
		counter = 0;
	}
	for (i = 0; i < n - 1; i++) //сортировка массива структур по длине строк файла методом "пузырька"
		for (j = i + 1; j < n; j++)
		{
			if (lines[i].length > lines[j].length)
			{
				tmp = lines[i];
				lines[i] = lines[j];
				lines[j] = tmp;
			}
		}
	printf("\n");
	//âûâîä ñòðîê â ïîðÿäêå âîçðàñòàíèÿ èõ äëèí 
	for (i = 0; i < n; i++)
	{
		puts(lines[i].adr);
	}
	fclose(fpin);
	return 0;
}