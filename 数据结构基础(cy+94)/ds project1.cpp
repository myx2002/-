#include<stdio.h>
#include<time.h>
#define MAX  500000		/*control the number of repetitions of the function*/

clock_t start, stop;	/*clock_t is a built-in type for processor time(ticks)*/
double duration;		/*records the run time (seconds) of a function*/
double powerthefirst(double,int);	/*declarative the function*/
double powerthesecond(double,int);
double powerthethird(double,int);
int main()
{
	int index;
	int n = 100000;
	double sum;

	/*clock() return the amount of processor time(ticks) that has elapsed since the program began running*/
	start = clock();
	
	for (index = 0;index < MAX; index++)
	{
		//sum = powerthefirst(1.0001, n);			/*Comment out the function------choose the function to use*/
		//sum = powerthesecond(1.0001, n);
		sum = powerthethird(1.0001, n);
	}
		sum=powerthesecond(1.0001, 100000);
	//printf("%lf", powerthesecond(2, 5));			/*Verify the correctness of the algorithm*/
	//printf("%lf", powerthethird(2, 5));
	stop = clock();
	duration = ((double)(stop - start)) / CLK_TCK;
	/*CLK_TCK is a built_in constant = ticks per second*/
	printf("%.6lf", duration);
	return 0;
}
double powerthefirst(double x,int k)				/* with the complexities O(n) , consider the number of the multiply, if k is O(n) */
{
	int index;
	double sum=0;
	for (index = 0;index < k;index++)				/*set k as a index, iterate the circle k times*/
	{
		sum = sum * x;								/*multiple the x for n-1 times*/
	}
	return sum;
}			
double powerthesecond(double x,int k)			/* with the complexities O(log n) // recursion*/
{
	if (k == 0)									/*if k is equal to 0, return 1 */
		return 1;
	else if (k != 0 && k % 2 == 0)				/*if k is even and k is not 0,return power(x^2,k/2)*/
		return powerthesecond(x * x, k / 2);
	else if (k % 2 == 1)
		return powerthesecond(x * x, k / 2) * x;/*if k is a odd ,return power(x^2,n/2)*x */
}
double powerthethird(double x, int k)			/* with the complexities O(n) // iteration */
{
	double record = 1;
	while (k != 0)								
	{
		if (k % 2 == 0)							/*if k is even and k is not 0,we should update the x from single to x^2*/
			x = x * x;
		else if (k % 2 == 1)
			record = record * x;				/*if k is an odd , we should record a x in the final output*/
		k = k / 2;
	}
	record = record * x;
	return record;
}