#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int total_number;
FILE* p;
void slice(int, int);
int main()
{
	int total_width;
	int total_height;
	srand((unsigned int)time(NULL));
	fopen_s(&p,"C:\\Users\\ma\\Desktop\\optimal_dataset\\test_5.txt", "w");
	scanf_s("%d %d",&total_width,&total_height);
	fprintf(p,"%d\n",total_width);
	total_number = 0;
	slice(total_width, total_height);
	fprintf(p,"%d\n", total_number);
	fclose(p);

	return 0;
}

void slice(int width, int height)
{
	if ((1 < width && width <= 10) && (1 < height && height <= 10))
	{
		fprintf(p,"%d %d\n",width,height);
		total_number++;
		return;
	}
	else if (width == 1 || height == 1)
	{
		fprintf(p,"%d %d\n", width, height);
		total_number++;
		return;
	}
	int flag = 0;
	if (width < height)
		flag = 1;
	int sli;
	if (flag == 0)
	{
		sli = rand() % width + 1;
		if (sli != 0)
			slice(sli, height);
		if (sli != width)
			slice(width - sli, height);
	}
	else
	{
		sli = rand() % height + 1;
		if (sli != 0)
			slice(width, sli);
		if (sli != height)
			slice(width, height - sli);
	}
}