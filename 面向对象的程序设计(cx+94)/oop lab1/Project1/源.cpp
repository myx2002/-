#include<iostream>


template<typename T>
void swap(T func,int abc)
{
	func(abc);
	func(abc);
}
void fun(int abc)
{
	printf("%d\n", abc);
}

int main()
{
	int abc = 1;
	swap(fun,abc);
	return 0;
}