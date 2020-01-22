#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

FILE* fl_source;  //�������� ����
FILE* fl_corrected; //���� ��� ��������� � ������������

int storage_counter = 7;  //���������� ��������� �������������� � �������������
int type_counter = 9;
int composite_type_counter = 3;
char** storage_spec_modif;      //���������� ��������� �������� ��� ��������� �������������� � �������������
char** type_spec_modif;
char** composite_type;
int function_counter = 0; //���-�� �������

struct FUNCTIONS {
	int var_counter;
	char* function;
	char** types;
	char** identificators;
} *functions;

//���������� �������� ��������� �����
int setTypes() {
	storage_spec_modif = (char**)calloc(storage_counter, sizeof(char*));  //������������� �������� (auto, static, register, extern) � ������������ �������� (const, volatile, restrict)	
	for (int i = 0; i < storage_counter; i++)
		storage_spec_modif[i] = (char*)calloc(10, sizeof(char));
	strcpy(storage_spec_modif[0], "auto");
	strcpy(storage_spec_modif[1], "static");
	strcpy(storage_spec_modif[2], "register");
	strcpy(storage_spec_modif[3], "extern");
	strcpy(storage_spec_modif[4], "const");
	strcpy(storage_spec_modif[5], "volatile");
	strcpy(storage_spec_modif[6], "restrict");

	type_spec_modif = (char**)calloc(type_counter, sizeof(char*));  //������������ � ������������� ����
	for (int i = 0; i < type_counter; i++)
		type_spec_modif[i] = (char*)calloc(10, sizeof(char));
	strcpy(type_spec_modif[0], "int");
	strcpy(type_spec_modif[1], "char");
	strcpy(type_spec_modif[2], "float");
	strcpy(type_spec_modif[3], "double");
	strcpy(type_spec_modif[4], "unsigned");
	strcpy(type_spec_modif[5], "signed");
	strcpy(type_spec_modif[6], "long");
	strcpy(type_spec_modif[7], "short");
	strcpy(type_spec_modif[8], "void");

	composite_type = (char**)calloc(composite_type_counter, sizeof(char*));  //��������� ����
	for (int i = 0; i < composite_type_counter; i++)
		composite_type[i] = (char*)calloc(10, sizeof(char));
	strcpy(composite_type[0], "struct");
	strcpy(composite_type[1], "enum");
	strcpy(composite_type[2], "union");

	return 0;
}

//������� ��������� �������� ����� � ���������� ��������������� � ��������������
int typeCheck(char* buf) {
	for (int i = 0; i < storage_counter; i++) {  //���� ������������� ��������
		if (strcmp(storage_spec_modif[i], buf) == 0) {
			return 1;
		}
	}

	for (int i = 0; i < type_counter; i++) {   //���� ������������ � ������������� ����
		if (strcmp(type_spec_modif[i], buf) == 0) {
			return 2;
		}
	}

	for (int i = 0; i < composite_type_counter; i++) {  //���� ��������� ����
		if (strcmp(composite_type[i], buf) == 0) {
			return 3;
		}
	}

	return 0;
}

//�������� ������������ � ��������� �� ������ ������, ������ typedef
int fileProcess() {
	char c, buffer[1000] = { 0 };
	int i = 0, state;

	fl_source = fopen("file_source.txt", "rt");
	if (fl_source == NULL) {
		printf("Error! Can`t open file_source.txt \n");
		getchar();
		exit(1);
	}
	fl_corrected = fopen("file_corrected.txt", "wt");
	if (fl_corrected == NULL) {
		printf("Error! Can`t open file_corrected.out \n");
		getchar();
		exit(1);
	}

	while ((c = getc(fl_source)) != EOF) {
		if (c == '/') {
			c = getc(fl_source);

			if (c == '/')
				while ((c = getc(fl_source)) != '\n');

			else if (c == '*') {
				c = getc(fl_source);
				while (1) {
					if (c == '*') {
						c = getc(fl_source);
						if (c == '/') {
							c = getc(fl_source);
							break;
						}
					}
					else
						c = getc(fl_source);
				}
			}
			else
				fputc(c, fl_corrected);
		}

		else if (c == '\\') {
			c = getc(fl_source);
			c = getc(fl_source);
		}

		else if (c == '"') {
			fputc(c, fl_corrected);
			c = getc(fl_source);
			while (c != '"') {
				if (c == '\\') {
					c = getc(fl_source);
					if (c == '"')
						c = getc(fl_source);
				}
				c = getc(fl_source);
			}
		}
		else if (c == '\'') {
			fputc(c, fl_corrected);
			c = getc(fl_source);
			while (c != '\'') {
				if (c == '\\') {
					c = getc(fl_source);
					if (c == '"')
						c = getc(fl_source);
				}
				c = getc(fl_source);
			}
		}
		else if (c == ' ' || c == '\n' || c == '\t') {
			buffer[i] = '\0';
			if (strcmp(buffer, "typedef") == 0) {    //���� ��� typedef
				fputc(c, fl_corrected);
				for (i = 0; (c = getc(fl_source)) != ' ' && c != '\t'; i++) {   //��������� ��������������� ���
					buffer[i] = c;
					fputc(c, fl_corrected);
				}
				fputc(c, fl_corrected);
				buffer[i] = '\0';
				state = typeCheck(buffer);
				i = 0;
				while (typeCheck(buffer)) {
					for (i = 0; (c = getc(fl_source)) != ' ' && c != '\t' && c != ';'; i++) {   //��������� ��������������� ��� �����, ���� ����
						buffer[i] = c;
						fputc(c, fl_corrected);
					}
					if (c == ' ')
						fputc(c, fl_corrected);
					buffer[i] = '\0';
				}

				if (state == 1) {
					storage_counter++;
					storage_spec_modif = (char**)realloc(storage_spec_modif, storage_counter * sizeof(char*));
					storage_spec_modif[storage_counter - 1] = (char*)calloc(strlen(buffer), sizeof(char));
					strcpy(storage_spec_modif[storage_counter - 1], buffer);
				}
				if (state == 2) {
					type_counter++;
					type_spec_modif = (char**)realloc(type_spec_modif, type_counter * sizeof(char*));
					type_spec_modif[type_counter - 1] = (char*)calloc(strlen(buffer) + 1, sizeof(char));
					strcpy(type_spec_modif[type_counter - 1], buffer);
				}
				if (state == 3) {
					composite_type_counter++;
					composite_type = (char**)realloc(composite_type, composite_type_counter * sizeof(char*));
					composite_type[composite_type_counter - 1] = (char*)calloc(strlen(buffer), sizeof(char));
					strcpy(composite_type[composite_type_counter - 1], buffer);
				}
			}
			i = 0;
		}
		else
			buffer[i++] = c;

		fputc(c, fl_corrected);
	}

	fclose(fl_source);
	fclose(fl_corrected);
	return 0;
}

//������ ���������� � ���������
int varWrite(char* buf) {
	int state;
	int flag_type = 0;
	int position = 0;
	int var_counter = (functions + function_counter - 1)->var_counter;
	char type_buf[1000] = { 0 }, temp[100] = { 0 }, zvezda[100] = { 0 };
	struct FUNCTIONS* func = (functions + function_counter - 1);

	while (*buf != '\0') {
		position = 0;
		while (*buf == ' ' || *buf == '\t' || *buf == '\n')
			buf++;
		while (*buf != ' ' && *buf != '\t' && *buf != '*' && *buf != '[' && *buf != ',' && *buf != '=' && *buf != '\0')   //������ �������
			temp[position++] = *buf++;
		temp[position] = '\0';
		if (!strcmp(temp, "typedef"))   //���� ��� typedef, �� ��� �� ��������
			return 1;
		if (*buf == ',' && position == 0) {
			buf++;
			continue;
		}
		if (state = typeCheck(temp)) {   //���� ��� ���� �� ��������� �����
			strcat(type_buf, temp);
			strcat(type_buf, " ");
			if (state == 3) {        //���� ��� struct, enum ��� union, �� ��������� ����� ����� � �������� � temp
				while (*buf == ' ' || *buf == '\t')
					buf++;
				position = 0;
				while (*buf != ' ' && *buf != '\t')
					temp[position++] = *buf++;
				temp[position] = '\0';
				while (*buf == ' ' || *buf == '\t')
					buf++;
				strcat(type_buf, temp);
			}
			flag_type = 1;
			continue;
		}

		if (!flag_type)   //���� �� �� ���������� ���������� ����������, �� ������� �� ����� (������ ��� �� ��������)
			return 0;

		else if (*buf == '[' && position == 0) {    //���� '[' ����� �������� �� ��������������
			strcat(func->identificators[var_counter - 1], "[]");
			while (*buf != ']')
				* buf++;
			*buf++;
		}

		else if ((*buf == '=' || *buf == '\0') && position == 0) {
			if (*buf == '\0')
				break;
			else {
				while (*buf != ',' && *buf != '\0')
					buf++;
			}
		}

		else if (*buf == '*') {   //���� ��� ���������, �� ���������� ��� �������� � ������ zvezda
			position = 0;
			while (*buf == '*' || *buf == ' ' || *buf == '\t') {  //���������� ��� �������� � temp
				if (*buf == '*')
					zvezda[position++] = *buf;
				*buf++;
			}
			zvezda[position] = '\0';
		}

		else if (!state) {     //���� ��� �������������, �� ���������� �� � ���������
			func->types = (char**)realloc(func->types, (var_counter + 1) * sizeof(char*));
			func->types[var_counter] = (char*)calloc(strlen(type_buf) + strlen(zvezda) + 1, sizeof(char));    //�������� ����� ��� ��� + ��������

			strcpy(func->types[var_counter], type_buf);
			strcat(func->types[var_counter], zvezda);
			zvezda[0] = '\0';

			func->identificators = (char**)realloc(func->identificators, (var_counter + 1) * sizeof(char*));
			func->identificators[var_counter] = (char*)calloc(strlen(temp) + 3, sizeof(char));      //�������� ����� ��� ������������� + "[]"

			strcat(func->identificators[var_counter], temp);

			if (*buf == '[') {
				strcat(func->identificators[var_counter], "[]");
				while (*buf != ']')
					buf++;
				buf++;
			}
			if (*buf == ',')
				buf++;
			var_counter++;
		}
	}

	func->var_counter = var_counter;

	return 1;
}

//������� ������ ����� � �������
int funcSearch() {
	int flag_function = 0;
	int flag_skobka1 = 0;
	int flag_skobka2 = 0;
	int position = 0;
	char c, buf[100], func_name[100], func_var[100], temp[100];
	char* ptr = buf;

	while ((c = getc(fl_corrected)) != EOF) {      //���������� ����� � �������
		if (c == ';' || c == '>') {
			while ((c = getc(fl_corrected)) == ' ' || c == '\t' || c == '\n');
			position = 0;
			flag_skobka1 = 0;
			flag_skobka2 = 0;
		}
		else if (c == '=') {
			while ((c = getc(fl_corrected)) != ';');
			while ((c = getc(fl_corrected)) == ' ' || c == '\t' || c == '\n');
			position = 0;
		}
		else if (c == '(')
			flag_skobka1 = 1;
		else if (c == ')')
			flag_skobka2 = 1;
		else if (c == '{') {
			if (flag_skobka1 && flag_skobka2) {
				flag_function = 1;
				break;
			}
			else {
				while ((c = getc(fl_corrected)) != ';');
				while ((c = getc(fl_corrected)) == ' ' || c == '\t' || c == '\n');
				position = 0;
				flag_skobka1 = 0;
				flag_skobka2 = 0;
			}
		}
		*(ptr + position) = c;
		position++;
	}

	if (c == EOF && !flag_function)      //���� �� ����� ����, �� ���������� ����
		return 0;

	function_counter++;                //������ � ��������� ����� �������
	functions = (struct FUNCTIONS*)realloc(functions, function_counter * sizeof(struct FUNCTIONS));  //�������� ����� ���������
	(functions + function_counter - 1)->types = (char**)calloc(1, sizeof(char*));
	(functions + function_counter - 1)->identificators = (char**)calloc(1, sizeof(char*));
	(functions + function_counter - 1)->var_counter = 0;
	while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
		ptr++;
	while (1) {
		position = 0;
		while (*ptr != ' ' && *ptr != '\t' && *ptr != '(')
			func_name[position++] = *ptr++;
		func_name[position] = '\0';
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		if (!typeCheck(func_name))
			break;
	}
	ptr++;
	(functions + function_counter - 1)->function = (char*)calloc(position + 1, sizeof(char));
	strcpy((functions + function_counter - 1)->function, func_name);
	while (*ptr != ')') {       //������ � ������ ���������� �� ���������� �������
		position = 0;
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		while (*ptr != ')' && *ptr != ',')
			func_var[position++] = *ptr++;
		if (*ptr == ',')
			ptr++;
		func_var[position] = '\0';
		varWrite(func_var);
	}

	return 1;
}

//������������ ������
int freeMemory() {
	for (int i = 0; i < function_counter; i++) {      //������������ ������ � ���������
		for (int j = 0; j < (functions + i)->var_counter; j++) {
			free((functions + i)->types[j]);
			free((functions + i)->identificators[j]);
		}
		free((functions + i)->types);
		free((functions + i)->identificators);
		free((functions + i)->function);
	}
	free(functions);

	for (int i = 0; i < storage_counter; i++) {  //������������ ������ �������������� ��������
		free(storage_spec_modif[i]);
	}
	free(storage_spec_modif);

	for (int i = 0; i < type_counter; i++) {   //������������ ������ �������������� � ������������� ����
		free(type_spec_modif[i]);
	}
	free(type_spec_modif);

	for (int i = 0; i < composite_type_counter; i++) {    //������������ ������ ��������� �����
		free(composite_type[i]);
	}
	free(composite_type);

	return 0;
}

//����� ������� � ���������� �� ���������
int printVar() {
	printf("�������                   ���� ����������                     ��������������                      \n");   //����� ���������� �� �����
	printf("-----------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < function_counter; i++) {
		printf("%-25s ", (functions + i)->function);
		if ((functions + i)->var_counter == 0)
			printf("\n");
		for (int j = 0; j < (functions + i)->var_counter; j++) {
			if (j != 0)
				printf("                          ");
			printf("%-35s ", (functions + i)->types[j]);
			printf("%-35s", (functions + i)->identificators[j]);
			printf("\n");
		}
		printf("-----------------------------------------------------------------------------------------------\n");
	}

	return 0;
}

//������� �������
int main() {
	char c, buf[1000];
	int position;
	int Block_cnt = 0;

	setlocale(LC_ALL, "");

	setTypes();  //���������� �������� ��������� �����

	fileProcess();     //������� ������������ � ��������� � ����� (���������� �����)

	fl_corrected = fopen("file_corrected.txt", "rt");     //�������� ��������������� �����

	while (1) {          //������� ����
		if (!funcSearch())   //���� �� ����� ������� (�.�. �� ������ ��� � �����), �� ������� �� �����
			break;
		Block_cnt = 1;
		position = 0;
		while ((c = getc(fl_corrected)) == ' ' || c == '\t' || c == '\n');    //������� �� ����� � ��������
		while (c != '}' && c != EOF) {        //������ � ����� �� ';'
			if (c == '{') {       //���� ����� ���� � �������, �� ��� struct, enum ��� union, ������� ���������� ��, ��� ��������� � ����� (��� ��� �� ����������)
				Block_cnt++;
				while (Block_cnt != 1) {   //���� �� � �����
					c = getc(fl_corrected);
					if (c == '{')
						Block_cnt++;
					else if (c == '}')
						Block_cnt--;
				}
				Block_cnt = 1;

			}
			else if (c == ';') {
				buf[position] = '\0';
				if (!varWrite(buf)) {        // ���� ����������� ���������� ����������, �� ������� �� �������(���� ������ varWrite(buf);)
					while (Block_cnt) {  //���� �� � �����
						if (c == '{')
							Block_cnt++;
						else if (c == '}')
							Block_cnt--;
						c = getc(fl_corrected);
					}
					c = getc(fl_corrected);
					break;
				}
				position = 0;
			}
			else {
				buf[position++] = c;
			}
			c = getc(fl_corrected);
		}
	}

	printVar();    //����� ����������, ������� �� ����� � ����
	freeMemory();  //������������ ���� ���������� ������ � ���� ������ ���������
	fclose(fl_corrected);
	system("pause");
	return 0;
}