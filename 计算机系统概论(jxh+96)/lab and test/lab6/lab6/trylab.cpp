#include <stdio.h>
#include <string.h>
#pragma warning (disable: 4996)									

void opfun(char array[300][50], int, int);
void anddfun(char input[300][50], int, int);
void notfun(char input[300][50], int, int);
void jmpfun(char input[300][50], int, int);
void ltdrfun(char input[300][50], int, int);
void trapfun(char input[300][50], int, int);
void jsrrfun(char input[300][50], int, int);
void brfun(char input[300][50], char labeltable[300][50], int, int, int);
void lsdifun(char input[300][50], char labeltable[300][50], int, int, int);
void learfun(char input[300][50], char labeltable[300][50], int, int, int);

int findr(char input[300][50], int, int);
int findadd(char input[300][50], char labeltable[300][50], int, int, int);
void trans(int, int, int);

int main()
{

	char input[300][50] = {};
	int first = 0;				//记录总行数 
	int again;					//用于循环 
	char labeltable[300][50] = {};	//记录存在label的位置
	int second = 0;				//记录当前位数 
	int flag = 1;
	freopen("input.txt", "r", stdin);

	while (1)					//记录input 
	{
		scanf("%c", &input[first][second]);
		if (second == 0 && input[first][second] == ' ')									//消除句首空格 
			continue;
		if (second >= 1 && input[first][second - 1] == ' ' && input[first][second] == ' ' && flag == 1)	//消除多余空格 
			continue;
		if (input[first][second] == '\n' && input[first][0] != '.')						//输入判断结束条件 
		{
			if (second == 0)
				continue;
			input[first][second] = '\0';
			first++;
			second = 0;
			continue;
		}
		else if (input[first][0] == '.' && input[first][second] == '\n')
		{
			if (input[first][1] == 'E' && input[first][2] == 'N' && input[first][3] == 'D')
			{
				input[first][second] = '\0';
				break;
			}
			else
			{
				input[first][second] = '\0';
				first++;
				second = 0;
				continue;
			}
		}
		if (input[first][second] == '"')
			flag = -flag;
		second++;
	}
	again = first;
	for (first = 0;first <= again;first++)											//记录各行是否出现label 
	{
		second = 0;
		if ((input[first][second] == '\0') ||
			(input[first][0] == 'B' && input[first][1] == 'R') ||
			(input[first][0] == 'L' && input[first][1] == 'D' && input[first][2] == ' ') ||
			(input[first][0] == 'S' && input[first][1] == 'T' && input[first][2] == ' ') ||
			(input[first][0] == 'I' && input[first][1] == 'N' && input[first][2] == '\0') ||
			(input[first][0] == 'A' && input[first][1] == 'D' && input[first][2] == 'D' && input[first][3] == ' ') ||
			(input[first][0] == 'A' && input[first][1] == 'N' && input[first][2] == 'D' && input[first][3] == ' ') ||
			(input[first][0] == 'J' && input[first][1] == 'M' && input[first][2] == 'P' && input[first][3] == ' ') ||
			(input[first][0] == 'J' && input[first][1] == 'S' && input[first][2] == 'R' && input[first][3] == ' ') ||
			(input[first][0] == 'L' && input[first][1] == 'D' && input[first][2] == 'R' && input[first][3] == ' ') ||
			(input[first][0] == 'L' && input[first][1] == 'D' && input[first][2] == 'I' && input[first][3] == ' ') ||
			(input[first][0] == 'L' && input[first][1] == 'E' && input[first][2] == 'A' && input[first][3] == ' ') ||
			(input[first][0] == 'N' && input[first][1] == 'O' && input[first][2] == 'T' && input[first][3] == ' ') ||
			(input[first][0] == 'R' && input[first][1] == 'E' && input[first][2] == 'T' && input[first][3] == '\0') ||
			(input[first][0] == 'R' && input[first][1] == 'T' && input[first][2] == 'I' && input[first][3] == '\0') ||
			(input[first][0] == 'S' && input[first][1] == 'T' && input[first][2] == 'I' && input[first][3] == ' ') ||
			(input[first][0] == 'S' && input[first][1] == 'T' && input[first][2] == 'R' && input[first][3] == ' ') ||
			(input[first][0] == 'O' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == '\0') ||
			(input[first][0] == 'G' && input[first][1] == 'E' && input[first][2] == 'T' && input[first][3] == 'C' && input[first][4] == '\0') ||
			(input[first][0] == 'P' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == 'S' && input[first][4] == '\0') ||
			(input[first][0] == 'J' && input[first][1] == 'S' && input[first][2] == 'R' && input[first][3] == 'R' && input[first][4] == ' ') ||
			(input[first][0] == 'T' && input[first][1] == 'R' && input[first][2] == 'A' && input[first][3] == 'P' && input[first][4] == ' ') ||
			(input[first][0] == 'H' && input[first][1] == 'A' && input[first][2] == 'L' && input[first][3] == 'T' && input[first][4] == '\0') ||
			(input[first][0] == '.' && input[first][1] == 'E' && input[first][2] == 'N' && input[first][3] == 'D') ||
			(input[first][0] == 'P' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == 'S' && input[first][4] == 'P' && input[first][5] == '\0') ||
			(input[first][0] == '.' && input[first][1] == 'O' && input[first][2] == 'R' && input[first][3] == 'I') ||
			(input[first][0] == '.' && input[first][1] == 'F' && input[first][2] == 'I' && input[first][3] == 'L') ||
			(input[first][0] == '.' && input[first][1] == 'B' && input[first][2] == 'L' && input[first][3] == 'K') ||
			(input[first][0] == '.' && input[first][1] == 'S' && input[first][2] == 'T' && input[first][3] == 'R' && input[first][4] == 'I'
				&& input[first][5] == 'N' && input[first][6] == 'G' && input[first][7] == 'Z'))
		{
			labeltable[first][0] = '\0';
		}
		else
		{
			while (input[first][second] != ' ')
			{
				labeltable[first][second] = input[first][second];
				second++;
			}
			labeltable[first][second] = '\0';
		}
	}
	for (first = 0;first <= again;first++)
	{
		second = 0;
		if (labeltable[first][second] != '\0')
		{
			while (!(input[first][second] == ' ' && labeltable[first][second] == '\0'))
				second++;
			second++;
		}
		if (input[first][second] == '.')				//识别标签
			opfun(input, first, second);
		else if (input[first][second] == 'B' && input[first][second + 1] == 'R')
			brfun(input, labeltable, first, second, again);
		else if ((input[first][second] == 'A' && input[first][second + 1] == 'D' && input[first][second + 2] == 'D' && input[first][second + 3] == ' ') ||
			(input[first][second] == 'A' && input[first][second + 1] == 'N' && input[first][second + 2] == 'D' && input[first][second + 3] == ' '))
			anddfun(input, first, second);
		else if (input[first][second] == 'N' && input[first][second + 1] == 'O' && input[first][second + 2] == 'T' && input[first][second + 3] == ' ')
			notfun(input, first, second);
		else if ((input[first][second] == 'L' && input[first][second + 1] == 'E' && input[first][second + 2] == 'A' && input[first][second + 3] == ' ') ||
			(input[first][second] == 'J' && input[first][second + 1] == 'S' && input[first][second + 2] == 'R' && input[first][second + 3] == ' '))
			learfun(input, labeltable, first, second, again);
		else if (input[first][second] == 'R' && input[first][second + 1] == 'E' && input[first][second + 2] == 'T' && input[first][second + 3] == '\0')
			printf("1100000111000000\n");
		else if (input[first][second] == 'R' && input[first][second + 1] == 'T' && input[first][second + 2] == 'I' && input[first][second + 3] == '\0')
			printf("1000000000000000\n");
		else if (input[first][second] == 'J' && input[first][second + 1] == 'M' && input[first][second + 2] == 'P' && input[first][second + 3] == ' ')
			jmpfun(input, first, second);
		else if ((input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] == 'R' && input[first][second + 3] == ' ') ||
			(input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] == 'R' && input[first][second + 3] == ' '))
			ltdrfun(input, first, second);
		else if (input[first][second] == 'H' && input[first][second + 1] == 'A' && input[first][second + 2] == 'L' && input[first][second + 3] == 'T' && input[first][second + 4] == '\0')
			printf("1111000000100101\n");
		else if (input[first][second] == 'T' && input[first][second + 1] == 'R' && input[first][second + 2] == 'A' && input[first][second + 3] == 'P' && input[first][second + 4] == ' ')
			trapfun(input, first, second);
		else if (input[first][second] == 'J' && input[first][second + 1] == 'S' && input[first][second + 2] == 'R' && input[first][second + 3] == 'R' && input[first][second + 4] == ' ')
			jsrrfun(input, first, second);
		else if ((input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] == ' ') || (input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] == ' ') ||
			(input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] == 'I' && input[first][second + 3] == ' ') ||
			(input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] == 'I' && input[first][second + 3] == ' '))
			lsdifun(input, labeltable, first, second, again);
		else if (input[first][0] == 'I' && input[first][1] == 'N' && input[first][2] == '\0')
			printf("1111000000100011\n");
		else if (input[first][0] == 'O' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == '\0')
			printf("1111000000100001\n");
		else if (input[first][0] == 'P' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == 'S' && input[first][4] == '\0')
			printf("1111000000100010\n");
		else if (input[first][0] == 'G' && input[first][1] == 'E' && input[first][2] == 'T' && input[first][3] == 'C' && input[first][4] == '\0')
			printf("1111000000100000\n");
		else if (input[first][0] == 'P' && input[first][1] == 'U' && input[first][2] == 'T' && input[first][3] == 'S' && input[first][4] == 'P' && input[first][5] == '\0')
			printf("1111000000100100\n");
	}
	return 0;
}

int findadd(char input[300][50], char labeltable[300][50], int first, int second, int again)		//返回偏移量 ,正偏移取+，负偏移取-
{
	int i = 0, j;
	int record;
	int remain = 0;
	int number = 0;
	int result = 0;
	char label[30] = {};
	while (input[first][second] != ' ')
		second++;
	second++;
	while (input[first][second] != '\0')
	{
		label[i] = input[first][second];
		i++;
		second++;
	}
	label[i] = '\0';
	record = i;
	for (j = 0;j <= again;j++)
	{
		result = strcmp(labeltable[j], label);
		if (result == 0)
			break;
	}
	record = j;
	if (record > first)
	{
		for (j = first + 1;j < record;j++)
		{
			i = 0;
			if (labeltable[j][0] == '\0')
			{
				if (input[j][0] == '.' && input[j][1] == 'S')
				{
					i = i + 10;
					while (input[j][i] != '"')
					{
						number++;
						i++;
					}
				}
				else if (input[j][0] == '.' && input[j][1] == 'B')
				{
					i = i + 6;
					remain = 0;
					if (input[j][i] == '#')
						i++;
					while (input[j][i] != '\0')
					{
						remain = remain * 10 + input[j][i] - '0';
						i++;
					}
					number = number + remain - 1;
				}
			}
			else
			{
				while (input[j][i] != ' ')
					i++;
				if (input[j][i + 1] == '.' && input[j][i + 2] == 'S')
				{
					i = i + 11;
					while (input[j][i] != '"')
					{
						number++;
						i++;
					}
				}
				else if (input[j][i + 1] == '.' && input[j][i + 2] == 'B')
				{
					i = i + 7;
					remain = 0;
					if (input[j][i] == '#')
						i++;
					while (input[j][i] != '\0')
					{
						remain = remain * 10 + input[j][i] - '0';
						i++;
					}
					number = number + remain - 1;
				}
			}
			number++;
		}
		return number;
	}
	else if (record < first)
	{
		for (j = record;j <= first;j++)
		{
			i = 0;
			if (labeltable[j][0] == '\0')
			{
				if (input[j][0] == '.' && input[j][1] == 'S')
				{
					i = i + 10;
					while (input[j][i] != '"')
					{
						number++;
						i++;
					}
				}
				else if (input[j][0] == '.' && input[j][1] == 'B')
				{
					i = i + 6;
					remain = 0;
					if (input[j][i] == '#')
						i++;
					while (input[j][i] != '\0')
					{
						remain = remain * 10 + input[j][i] - '0';
						i++;
					}
					number = number + remain - 1;
				}
			}
			else
			{
				while (input[j][i] != ' ')
					i++;
				if (input[j][i + 1] == '.' && input[j][i + 2] == 'S')
				{
					i = i + 11;
					while (input[j][i] != '"')
					{
						number++;
						i++;
					}
				}
				else if (input[j][i + 1] == '.' && input[j][i + 2] == 'B')
				{
					i = i + 7;
					remain = 0;
					if (input[j][i] == '#')
						i++;
					while (input[j][i] != '\0')
					{
						remain = remain * 10 + input[j][i] - '0';
						i++;
					}
					number = number + remain - 1;
				}
			}
			number++;
		}
		return -number;
	}
}

void trans(int contain, int out, int flag)			//二进制输出 ,out 记录输出个数, flag 记录输出是否是负数  1-- 正数 
{
	char store[17];
	store[out] = '\0';
	int record;
	record = out;
	int remain;
	int carry = 0;
	int i;
	while (contain / 2 != 0)
	{
		out--;
		remain = contain % 2;
		contain = contain / 2;
		store[out] = remain + '0';
	}
	out--;
	store[out] = contain % 2 + '0';
	for (i = 0;i < out;i++)
		store[i] = '0';
	if (flag == 0)
	{
		for (i = 0;i < record;i++)
		{
			if (store[i] == '1')
				store[i] = '0';
			else if (store[i] == '0')
				store[i] = '1';
		}
		store[i - 1] = store[i - 1] + 1;
		for (i = record - 1;i >= 0;i--)
		{
			store[i] = store[i] + carry;
			if (store[i] == '2')
			{
				store[i] = '0';
				carry = 1;
			}
			else
				break;
		}
	}
	printf("%s", store);
}

int findr(char input[300][50], int first, int second)			//寻找寄存器位置并输出代号 
{
	int receive;
	while (input[first][second] != 'R' && input[first][second] != 'r')
		second++;
	second++;
	receive = input[first][second] - '0';
	trans(receive, 3, 1);
	return second;
}

void brfun(char input[300][50], char labeltable[300][50], int first, int second, int again)	//again总行数，labeltable 标签记录表 
{
	printf("0000");
	int result;
	int record;
	int receive = 0;
	int judge = 0;
	int flag = 1;
	second = second + 2;
	if (input[first][second] == 'n' && input[first][second + 1] == 'z' && input[first][second + 2] == 'p')
		printf("111");
	else if (input[first][second] == 'n' && input[first][second + 1] == 'z')
		printf("110");
	else if (input[first][second] == 'n' && input[first][second + 1] == 'p')
		printf("101");
	else if (input[first][second] == 'z' && input[first][second + 1] == 'p')
		printf("011");
	else if (input[first][second] == 'n')
		printf("100");
	else if (input[first][second] == 'z')
		printf("010");
	else if (input[first][second] == 'p')
		printf("001");
	else
		printf("111");
	record = second;
	while (input[first][record] != '\0')
	{
		if (input[first][record] == '#')
		{
			judge = 1;
			break;
		}
		record++;
	}
	if (judge == 1)
	{
		second = record;
		second++;
		if (input[first][second] == '-')
		{
			flag = 0;
			second++;
		}
		while (input[first][second] != '\0')
		{
			receive = receive * 10 + input[first][second] - '0';
			second++;
		}
		trans(receive, 9, flag);
	}
	else
	{
		result = findadd(input, labeltable, first, second, again);
		if (result < 0)
			trans(-result, 9, 0);
		else
			trans(result, 9, 1);
	}
	printf("\n");
}

void learfun(char input[300][50], char labeltable[300][50], int first, int second, int again)
{
	int judge = 0;
	int record = 0;
	int receive = 0;
	int flag = 1;
	int result = 0;
	int number = 0;
	if (input[first][second] == 'L' && input[first][second + 1] == 'E' && input[first][second + 2] == 'A')
	{
		printf("1110");
		second = findr(input, first, second);
		number = 9;
	}
	else if (input[first][second] == 'J' && input[first][second + 1] == 'S' && input[first][second + 2] == 'R')
	{
		printf("01001");
		number = 11;
	}
	record = second;
	while (input[first][record] != '\0')
	{
		if (input[first][record] == '#')
		{
			judge = 1;
			break;
		}
		record++;
	}
	if (judge == 1)
	{
		second = record;
		second++;
		if (input[first][second] == '-')
		{
			flag = 0;
			second++;
		}
		while (input[first][second] != '\0')
		{
			receive = receive * 10 + input[first][second] - '0';
			second++;
		}
		trans(receive, number, flag);
	}
	else
	{
		result = findadd(input, labeltable, first, second, again);
		if (result < 0)
			trans(-result, number, 0);
		else
			trans(result, number, 1);
	}
	printf("\n");
}

void lsdifun(char input[300][50], char labeltable[300][50], int first, int second, int again)
{
	if (input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] != 'I')
		printf("0010");
	else if (input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] != 'I')
		printf("0011");
	else if (input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] == 'I')
		printf("1010");
	else if (input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] == 'I')
		printf("1011");
	int result;
	int record;
	int receive = 0;
	int judge = 0;
	int flag = 1;
	second = findr(input, first, second);
	record = second;
	while (input[first][record] != '\0')
	{
		if (input[first][record] == '#')
		{
			judge = 1;
			second = record;
			break;
		}
		record++;
	}
	second++;
	if (judge == 1)
	{
		if (input[first][second] == '-')
		{
			flag = 0;
			second++;
		}
		while (input[first][second] != '\0')
		{
			receive = receive * 10 + input[first][second] - '0';
			second++;
		}
		trans(receive, 9, flag);
	}
	else
	{
		result = findadd(input, labeltable, first, second, again);
		if (result < 0)
			trans(-result, 9, 0);
		else
			trans(result, 9, 1);
	}
	printf("\n");
}

void opfun(char array[300][50], int number, int second)
{
	int total;
	int multi = 10;
	int receive = 0;
	unsigned int recordnum = 0;
	int flag = 1;
	second++;
	if (array[number][second] == 'O' && array[number][second + 1] == 'R' && array[number][second + 2] == 'I' && array[number][second + 3] == 'G')
	{
		total = second + 6;
		while (array[number][total] != '\0')
		{
			if (array[number][total] >= '0' && array[number][total] <= '9')
				receive = array[number][total] - '0';
			else if (array[number][total] <= 'F' && array[number][total] >= 'A')
				receive = array[number][total] - 'A' + 10;
			trans(receive, 4, 1);
			total++;
		}
		printf("\n");
	}
	else if (array[number][second] == 'E' && array[number][second + 1] == 'N' && array[number][second + 2] == 'D')
		return;
	else if (array[number][second] == 'F' && array[number][second + 1] == 'I' && array[number][second + 2] == 'L' && array[number][second + 3] == 'L')
	{
		total = second + 6;
		if (array[number][total - 1] == 'x')
		{

			if (array[number][total] == '-')
			{
				flag = 0;
				total++;
			}
			while (array[number][total] != '\0')
			{
				if (array[number][total] >= '0' && array[number][total] <= '9')
					recordnum = recordnum * 16 + array[number][total] - '0';
				else if (array[number][total] <= 'F' && array[number][total] >= 'A')
					recordnum = recordnum * 16 + array[number][total] - 'A' + 10;
				total++;
			}
			trans(recordnum, 16, flag);
			printf("\n");
		}
		else if (array[number][total - 1] == '#')
		{
			if (array[number][total] == '-')
			{
				flag = 0;
				total++;
			}
			while (array[number][total] != '\0')
			{
				recordnum = array[number][total] - '0' + 10 * recordnum;
				total++;
			}
			trans(recordnum, 16, flag);
			printf("\n");
		}
	}
	else if (array[number][second] == 'B' && array[number][second + 1] == 'L' && array[number][second + 2] == 'K' && array[number][second + 3] == 'W')
	{
		total = second + 5;
		if (array[number][total] == '#')
		{
			multi = 10;
			total++;
		}
		else if (array[number][total] == 'x')
		{
			multi = 16;
			total++;
		}
		while (array[number][total] != '\0')
		{
			if (array[number][total] <= '9' && array[number][total] >= '0')
				recordnum = recordnum * multi + array[number][total] - '0';
			else if (multi == 16 && array[number][total] <= 'F' && array[number][total] >= 'A')
				recordnum = recordnum * multi + array[number][total] - 'A' + 10;
			total++;
		}
		int i, j;
		receive = 7;
		for (i = 1;i <= recordnum;i++)
		{
			for (j = 1;j <= 4;j++)
				trans(receive, 4, 1);
			printf("\n");
		}
	}
	else
	{
		total = second + 9;
		while (array[number][total] != '"')
		{
			if (array[number][total] <= 'Z' && array[number][total] >= 'A')
				receive = array[number][total] - 'A' + 65;
			else if (array[number][total] >= 'a' && array[number][total] <= 'z')
				receive = array[number][total] - 'a' + 97;
			else if (array[number][total] == ' ')
				receive = 32;
			else
				receive = (int)array[number][total];
			trans(receive, 16, 1);
			printf("\n");
			total++;
		}
		printf("0000000000000000\n");
	}
}

void anddfun(char input[300][50], int first, int second)
{
	int receive = 0;
	int flag = 1;
	int multi = 0;
	int i;
	if (input[first][second + 1] == 'D')
		printf("0001");
	else if (input[first][second + 1 == 'N'])
		printf("0101");
	for (i = 1;i <= 2;i++)
		second = findr(input, first, second);
	while (!(input[first][second] == 'R' || input[first][second] == '#' || input[first][second] == 'x'))
		second++;
	if (input[first][second] == 'R')
	{
		second++;
		receive = input[first][second] - '0';
		trans(receive, 6, 1);
	}
	else
	{
		printf("1");
		if (input[first][second] == '#')
			multi = 10;
		else if (input[first][second] == 'x')
			multi = 16;
		second++;
		if (input[first][second] == '-')
		{
			flag = 0;
			second++;
		}
		while (input[first][second] != '\0')
		{
			if (input[first][second] <= '9' && input[first][second] >= '0')
				receive = receive * multi + input[first][second] - '0';
			else if (multi == 16 && input[first][second] <= 'F' && input[first][second] >= 'A')
				receive = receive * 16 + input[first][second] - 'A' + 10;
			second++;
		}
		trans(receive, 5, flag);
	}
	printf("\n");
}

void notfun(char input[300][50], int first, int second)
{
	int receive = 0;
	int i;
	printf("1001");
	for (i = 1;i <= 2;i++)
		second = findr(input, first, second);
	printf("111111\n");
}

void jmpfun(char input[300][50], int first, int second)
{
	int receive;
	printf("1100000");
	findr(input, first, second);
	printf("000000\n");
}

void ltdrfun(char input[300][50], int first, int second)				//str可以合并		 
{
	int receive = 0;
	int multi = 0;
	int flag = 1;
	int i;
	if (input[first][second] == 'L' && input[first][second + 1] == 'D' && input[first][second + 2] == 'R')
		printf("0110");
	else if (input[first][second] == 'S' && input[first][second + 1] == 'T' && input[first][second + 2] == 'R')
		printf("0111");
	second = second + 3;
	for (i = 1;i <= 2;i++)
		second = findr(input, first, second);
	while (!(input[first][second] == '#' || input[first][second] == 'x'))
		second++;
	if (input[first][second] == '#')
		multi = 10;
	else if (input[first][second] == 'x')
		multi = 16;
	second++;
	if (input[first][second] == '-')
	{
		flag = 0;
		second++;
	}
	while (input[first][second] != '\0')
	{

		if (input[first][second] <= '9' && input[first][second] >= '0')
			receive = receive * multi + input[first][second] - '0';
		else if (multi == 16 && input[first][second] <= 'F' && input[first][second] >= 'A')
			receive = receive * 16 + input[first][second] - 'A' + 10;
		second++;
	}
	trans(receive, 6, flag);
	printf("\n");
}

void trapfun(char input[300][50], int first, int second)
{
	int total = 0;
	printf("11110000");
	while (input[first][second] != 'x')
		second++;
	second++;
	while (input[first][second] != '\0')
	{
		if (input[first][second] <= '9' && input[first][second] >= '0')
			total = total * 16 + input[first][second] - '0';
		else if (input[first][second] <= 'F' && input[first][second] >= 'A')
			total = total * 16 + input[first][second] - 'A' + 10;
		second++;
	}
	trans(total, 8, 1);
	printf("\n");
}

void jsrrfun(char input[300][50], int first, int second)
{
	printf("0100000");
	second = second + 4;
	findr(input, first, second);
	printf("000000\n");
}