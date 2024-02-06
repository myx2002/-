#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
//deifne the max number of vertices, ensure the array will not cross the edge
#define m 1001
//define the max and min number,for the comparison of total score and total discount
int max = 0;
int min = 9999999;
//declare the function to find the adjacent vertice of certain vertice
int find(int* array, int* score, int number, int total, int* record,int*check, int ori, int* sum, int* next, int* level, int re);

int main()
{
	//random number seed
	srand((unsigned int)time(NULL));
	
	//define the total number of vertice,and the total number of relational statement
	int total, number;
	int answer;
	int i, j, a, b, c, d, e = 0, f = 0, g = 0;
	//define the output state
	int final = 1;
	//define the sum of total discount
	int* sum = &e;
	//define the current circle(for recording the vertice in an output array)
	int* level = &f;
	//define the sum of total score
	int* next = &g;
	//define the adjacent matrix of score and discout
	int* array = (int*)malloc(sizeof(int) * m * m);
	int* score = (int*)malloc(sizeof(int) * m * m);
	
	//get the total number of vertice and relational statement
	

	//scanf_s("%d %d", &total, &number);
	//generate random number according to the maximum number,and print it on the screen
	total = 1000;
	//control the total number of relational statement
	number = round(rand() % 300+200) + 1;
	printf("%d %d\n", total, number);

	//initialize each element in array and score with -1
	for (i = 0;i < m;i++)
		for (j = 0;j < m;j++)
		{
			array[i * m + j] = -1;
			score[i * m + j] = -1;
		}
	//get the test level , min score and discount of each relational statement
	for (i = 0;i < number;i++)
	{
		//scanf_s("%d %d %d %d", &a, &b, &c, &d);
		//randomly generate two vertices generating relations
		//randomly generate required scores and available discounts,ensure that the generated scores and discounts are within the specified range
		//print it on the screen
		a = round(rand() % total) + 1;
		b = round(rand() % total) + 1;
		while (a == b)
			b = round(rand() % total) + 1;
		c = round(rand() % 100) + 1;
		d = round(rand() % 500) + 1;
		printf("%d %d %d %d\n", a, b, c, d);


		//place the numerical values of score and discount in the corresponding matrix
		array[a * m + b] = d;
		score[a * m + b] = c;
	}
	//get the total number of check vertice

	//scanf_s("%d", &number);
	//randomly generate the total number of check statement
	number = round(rand() % total) / 10 + 1;
	printf("%d\n", number);

	//define the return array of each check vertice
	int flag[m];
	//define the other array to record the check vertice
	int other[m];
	//define a array to record the optimal path, another for checking the correctness of the path
	int* record = (int*)malloc(sizeof(int) * m * m);
	int* check = (int*)malloc(sizeof(int) * m * m);
	//initialize each element in array record and check with -1
	for (i = 0;i < m;i++)
		for (j = 0;j < m;j++)
		{
			record[i * m + j] = -1;
			check[i * m + j] = -1;
		}
	//traverse each check vertice
	for (i = 0;i < number;i++)
	{
		//get the current check number
		//scanf_s("%d", &other[i]);
		//randomly generate the check vertice ensure the vertice is in the range of total
		other[i] = round(rand() % total) + 1;
		printf("%d ", other[i]);


		//reset set the sum of score ,discount and record level
		*sum = -1;
		*next = 0;
		*level = 1;
		//reset the min,max judge
		max = 0;
		min = 9999999;
		//set the first element of array record and check with the check vertice
		record[i * m] = other[i];
		check[i * m] = other[i];
		//find the optimal path and store the return value in the flag array
		flag[i] = find(array, score, other[i], total, record,check, other[i], sum,next, level, i);
	}
	//if there exists a return value -1, that means there at least one circle in the gragh,therefore the whole plan is not consistent
	//input/output split line
	printf("\n");
	printf("----------------------------------------\n");
	for (i = 0;i < number;i++)
		if (flag[i] == -1)
			//set the final to 0  to record this 
			final = 0;
	//if the whole plan is not consistent, print impossible
	if (final == 0)
	{
		printf("Impossible.\n");
		answer = 0;
	}
	else
	//else the whole plan is consistent, print okay
	{
		printf("Okay.\n");
		answer = 1;
	}
	//check evey check vertice output results
	for (i = 0;i < number;i++)
	{
		//if the return is 0, means the in-degree of this vertice is 0
		if (flag[i] == 0)
			printf("You may take test %d directly.\n", other[i]);
		//else the return is 1,means we have find the optimal path to cover the test
		else if (flag[i] == 1 && answer == 1)
			for (j = total - 1;j >= 0;j--)
			{
				//output according to the required format
				if (j == 0)
					printf("%d\n", check[i * m + j]);
				else if (check[i * m + j] >= 0)
					printf("%d->", check[i * m + j]);
			}
		//if the return is -1,means the vertice is in a circle
		else 
			printf("Error.\n");
	}
	//free dynamically allocated memory
	free(array);
	free(score);
	free(record);
	free(check);
	return 0;
}

//this function is used to find the optimal path of a certain vertice with score goes first and discount goes second with recursion
//input:1.the array record the discount of each pair of vertice
//		2.the array record the score of each piar of vertice
//		3.the certain vertice we need to find
//		4.the total number of vertice
//		5.the array to check the path
//		6.the array to record the optimal path
//		7.the origin vertice we need to find
//		8.the sum about discount of current path
//		9.the sum about score of current path
//		10.the current level of record array
//		11.the place we need to record the current optimal path of certain vertice
//output:the result of the find optimal path
//		if the vertice is in-degree 0 vertice ,then return 0
//		if the vertice is in a circle ,then return -1
//		if the we find the optimal path ,return 1
int find(int* array,int* score, int number,int total,int* record,int* check,int ori,int* sum,int* next,int* level,int re)
{
	int i, j, k;
	//define the exceptional case index
	int index = 0;
	//define the state vary
	int fa;
	//if there exists a circle, return -1
	if (number == ori && *sum != -1)
		return -1;
	for (j = 0;j < total;j++)
		if (array[j * m + number] != -1)
			index = 1;
	//if the vertice is a in-degree 0 vertice ,return 0
	if (index == 0)
		return 0;
	for (i = 0;i < total;i++)
	{
		if (i == number && array[i * m + number] != -1)
			return -1;
		//find the non -1 element in the adjacent matrix of a certain column
		if (array[i * m + number] != -1)
		{
			//add the total discount
			*sum = *sum + array[i * m + number];
			//add the total score require
			*next = *next + score[i * m + number];
			//add the level to input the vertice
			*level = *level + 1;
			//record the vertice in the current place
			record[re * m + *level - 1] = i;
			//use recursion to find the local optimal path
			fa = find(array, score, i, total, record,check, ori, sum, next, level, re);
			//if there exist a circle ,return -1 directly
			if (fa == -1)
				return -1;
			//if the score require is smaller than the current optimal,record it 
			if (*next < min && fa == 0)
			{
				//record the current optimal total score and discount
				min = *next;
				max = *sum;
				//fill the current place
				record[re * m + *level - 1] = i;
				//reset the element in the recrod array, prevent error multi output
				for (k = *level;k < m;k++)
					record[re * m + k] = -1;
				k = 0;
				//copy the current path into final output array
				while (record[re * m + k] != -1)
				{
					check[re * m + k] = record[re * m + k];
					k++;
				}
				for (k = *level;k < m;k++)
					check[re * m + k] = -1;
			}
			//if the score is equal to min path, compare the total number of discount
			else if (*next == min && fa == 0 && *sum >= max)
			{

				max = *sum;
				//fill the current place
				record[re * m + *level - 1] = i;
				//reset the element in the recrod array, prevent error multi output
				for (k = *level;k < m;k++)
					record[re * m + k] = -1;
				k = 0;
				//copy the current path into final output array
				while (record[re * m + k] != -1)
				{
					check[re * m + k] = record[re * m + k];
					k++;
				}
				for (k = *level;k < m;k++)
					check[re * m + k] = -1;
			}
			//reset the level
			*level = *level - 1;
			//reset the toal number of score require and discount
			*next = *next - score[i * m + number];
			* sum = *sum - array[i * m + number];
		}
	}
	return 1;
}