#include<stdio.h>
#include<stdlib.h>

//define the data structure for building the binary tree
typedef struct tree
{
	//the number of every node
	int number;
	//the depth of the node(begin with 0)
	int Degree;
	//the left children
	struct tree* Left;
	//the right children
	struct tree* Right;
}node;

//read in the input 
void read(int* a, int total);
//if the in/pre/post order only contains ont '-',fill it
void fill(int* in, int* a, int total);
//output the result array of in/pre/post order
void output(int* in, int total);
//make a copy of certain input
void copy(int* in, int* inc, int start, int total);
//put the tree into a big array
void preorder(int out[][110], node* p);
//print a number of a certain node
void print(node* p);
//make a copy by certain start location and total copy number
void recopy(int* in, int* inc, int start, int total);
//find the total number of possible fill and remain these number
int deal(int* in, int* re, int* tryfill, int total);
//check whether the first element of preorder is the same as the last element fo the postorder
int checkfirst(int* pre, int* post, int total);
//find the position about a certain number in a array
int find(int* in, int boo,int start, int total);
//fill the inorder from left to right space bit by space bit
int fillinorder(int* in, int* pre, int* post, int* inc, int* prec, int* postc, int start, int total, int* record);
//fill the preorder from left to right space bit by space bit
int fillpreorder(int* in, int* pre, int* post, int* inc, int* prec, int* postc, int start, int total, int* record);
//check the certain combination of in/pre/postorder
int endcheck(int* inc, int* prec, int* postc, int total, int* record);
//use inorder and postorder to build a unique tree
node* bulidtree(int in[], int post[], int total, int degree);

//the details of the functions are in the following

// this function read in a series of number£¬if encount '-',fill the space with 0 and stop by '\n' 
// input: a receive array and the total numebr are supposed to scanf
// output: none
void read(int* a, int total)
{
	int index=0;
	int i;
	char ch;
	//set the array with 0 at first
	for (i = 0;i < total;i++)
		a[i] = 0;
	scanf_s("%c", &ch);
	//scanf the input data
	while (ch != '\n')
	{
		if (ch <= '9' && ch >= '0')
			//if the input is a numebr , record it
			a[index] = 10 * a[index] + (ch - '0');
		else if (ch == ' ')
			//if the input is ' ',waiting for next input in next position
			index++;
		else if (ch == '-')
			// if the input is '-', record 0
			a[index] = 0;
		scanf_s("%c", &ch);
	}
}

//this functinon fill the only space in the in/pre/postorder
//input : the in/pre/postorder ,a array record the series number from 1 to n , the total number n
//output: none
void fill(int* in, int* a, int total)
{
	int i, j;
	int count = 0;
	//set a new array to record the array input
	int* ab = (int*)malloc(sizeof(int) * total);
	for (i = 0;i < total;i++)
		ab[i] = a[i];
	for (i = 0;i < total;i++)
	{
		//caculate the number of 0 in the array
		if (in[i] == 0)
			count++;
		for (j = 0;j < total;j++)
			//if the certain number has been in the array,make a mark to aviod filling it again
			if (in[i] == ab[j])
			{
				ab[j] = -1;
				break;
			}
	}
	//if there is no 0 in the array,do nothing
	if (count == 0);
	//if there only one 0 in the array,fill the only space
	else if (count == 1)
	{
		for (i = 0;i < total;i++)
			//find the position of 0
			if (in[i] == 0)
				break;
		for (j = 0;j < total;j++)
			//find the position the last number stay
			if (ab[j] != -1)
				break;
		//fill the last number in the array
		in[i] = ab[j];
	}
}

//this function is used to output a array in order
//input : the array need to output, the total number of the array
//output: none
void output(int* in, int total)
{
	int i;
	//if it is not the last element, printf it with a space
	for (i = 0;i < total - 1;i++)
		printf("%d ", in[i]);
	//if it is the last element, just output it
	printf("%d\n", in[total - 1]);
}

//this function is used to make a copy form array 1 to array 2 with certain position offset and total copy number
//input : the source array, the target array, the offset position ,the total number
//output: none
void copy(int* in, int* inc, int start, int total)
{
	int i;
	for (i = 0;i < total;i++)
		inc[i] = in[i + start];
}

//this function is used to make a copy form array 1 to array 2 with certain start and total copy number
//input : the source array, the target array, the start position ,the total number
//output: none
void recopy(int* in, int* inc, int start, int total)
{
	int i;
	for (i = 0;i < total;i++)
		in[i + start] = inc[i];
}

//this function is used to find the possible number can be used to fill the 0 in in/preorder
//input : the in/pre order array, all number from 1 to n, the possible number to fill the blank,the total number of n
//output: the number of posssible number
int deal(int* in, int* re,int* tryfill, int total)
{
	int i,j;
	for (i = 0;i < total;i++)
	{
		if (in[i] == 0)
			continue;
		for (j = 0;j < total;j++)
		{
			if (re[j] == -1)
				continue;
			//if the number has been in the in/preoder array,make a mark
			else if (in[i] == re[j])
				re[j] = -1;
		}
	}
	j = 0;
	for (i = 0;i < total;i++)
	{
		if (re[i] == -1)
			continue;
		else
		{
			//if the number dosn't have a mark,save it
			tryfill[j] = re[i];
			j++;
		}
	}
	return j;
}

//this function is used to check whether the first element of predorder is equal to the last element of the post order
//if they are not 0 and they are different from each other,the tryfill is failed,if the last number of postorder if 0,fill
//it with the first element of preorder array
//input : the prerder array, the postorder array,the total number of n
//output: if the pre/postorder number if legal,return 1
//		  if  it is illegal return 0
int checkfirst(int* pre, int* post, int total)
{
	if (pre[0] == 0 && post[total - 1] != 0)
		//if the first element of preorder is 0,fill it with the last element of postorder if the element is not 0
		pre[0] = post[total - 1];
	else if (pre[0] != 0 && post[total - 1] == 0)
		////if the last element of postorder is 0,fill it with the first element of preorder if the element is not 0
		post[total - 1] = pre[0];
	else if (pre[0] != 0 && post[total - 1] != 0 && pre[0] != post[total - 1])
		return 0;
	else if (pre[0] == 0 && post[total - 1] == 0)
		return 0;
	return 1;
}

//this function is used to find the position of a certain number in the array
//input : the array need to traversal,the number need to find ,the start position ,the total number of the array
//output: if find the number,return the position of the number
//		  if can't find the numebr,return -1
int find(int* in, int boo,int start, int total)
{
	int i;
	for (i = start;i < total ;i++)
		if (in[i] == boo)
			return i;
	return -1;
}

//the function is to traversal the inorder array,if ther exists 0 ,fill it with proper number in order
//input : the original in/pre/postorder ,the new in/pre/postorder(convenient for subsequent recursion),the start position of inorder array, 
//        the total number of in/pre/postprder array,the record number
//output: if the combination of fill get only one solution return 1;
//		  else return 0,
int fillinorder(int* in, int* pre, int* post, int* inc, int* prec, int* postc, int start, int total, int* record)
{
	int* incc = (int*)malloc(sizeof(int) * total);
	int* re = (int*)malloc(sizeof(int) * total);
	int* tryfill = (int*)malloc(sizeof(int) * total);
	int count;
	int i, j, flag;
	int receive;
	//if the solution is larger than 1, end the function
	if (*record > 1)
	{
		free(incc);
		free(re);
		free(tryfill);
		return 0;
	}
	copy(inc, incc, 0, total);
	//if the start position is larger than total number, stop
	if (start >= total)
		flag = -1;
	else
		//find thhe first number which is 0
		flag = find(incc, 0, start, total);
	for (i = 0;i < total;i++)
		re[i] = i + 1;
	//if 0 etists in the inorder array
	if (flag != -1)
	{
		count = deal(incc, re, tryfill, total);
		for (j = 0;j < count;j++)
		{
			//fill the 0 position 
			incc[flag] = tryfill[j];
			copy(incc, inc, 0, total);
			//recursion the function until the inorder array have already been filled
			receive = fillinorder(in, pre, post, inc, prec, postc, flag + 1, total, record);
			//if the return is 0, return 0 at once
			if (receive == 0)
			{
				free(re);
				free(incc);
				return 0;
			}
			//(this is important)after each recursion, put the filled number to 0 again, aviod changing the original number
			//in the inordercopy array, this is conducive to subsequent re assignment and call
			inc[flag] = 0;
		}
	}
	// if the number in inorder array have been filled completely
	else
	{
		int* precc = (int*)malloc(sizeof(int) * total);
		copy(prec, precc, 0, total);
		//if the inorder array have been completely filled ,then fill the preorder array
		flag = fillpreorder(in, pre, post, inc, precc, postc, 0, total, record);
		//if the return is 0, return 0 at once
		if (flag == 0)
		{
			free(precc);
			free(re);
			free(incc);
			return 0;
		}
	}
	free(re);
	free(incc);
	return 1;
}

//the function is to traversal the preorder array,if ther exists 0 ,fill it with proper number in order
//input : the original in/pre/postorder ,the new in/pre/postorder(convenient for subsequent recursion),the start position of preorder array, 
//        the total number of in/pre/postprder array,the record number
//output: if the combination of fill get only one solution return 1;
//		  else return 0,
int fillpreorder(int* in, int* pre, int* post, int* inc, int* prec, int* postc, int start, int total, int* record)
{
	int* precc = (int*)malloc(sizeof(int) * total);
	int* re = (int*)malloc(sizeof(int) * total);
	int* tryfill = (int*)malloc(sizeof(int) * total);
	int count ;
	int i, j, flag = 1;
	int receive;
	//if the solution is larger than 1, end the function
	if (*record > 1)
	{
		free(precc);
		free(re);
		free(tryfill);
		return 0;
	}
	copy(prec, precc, 0, total);
	//if the start position is larger than total number, stop
	if (start >= total)
		flag = -1;	
	else
		//find thhe first number which is 0
		flag = find(precc, 0, start, total);
	for (i = 0;i < total;i++)
		re[i] = i + 1;
	//if 0 etists in the postorder array
	if (flag != -1)
	{
		count=deal(precc, re,tryfill, total);
		for (j = 0;j < count;j++)
		{
			//fill the 0 position 
			precc[flag] = tryfill[j];
			copy(precc, prec, 0, total);
			//recursion the function until the postorder array have already been filled
			receive = fillpreorder(in, pre, post, inc, prec, postc, flag + 1, total, record);
			//if the return is 0, return 0 at once
			if (receive == 0)
			{
				free(re);
				free(precc);
				return 0;
			}
			//(this is important)after each recursion, put the filled number to 0 again, aviod changing the original number
			//in the postordercopy array, this is conducive to subsequent re assignment and call
			prec[flag] = 0;
		}
	}
	// if the number in inorder array have been filled completely
	else
	{
		int* postcc = (int*)malloc(sizeof(int) * total);
		copy(postc, postcc, 0, total);
		//if the preorder array have been completely filled ,then we can judge the combination of filling is proper or not
		flag = endcheck( inc, prec, postcc, total, record);
		if (flag == 1)
		{
			//printf("1\n");
			*record = *record + 1;
			if (*record == 1)
			{
				//if get one solution at the first time, copy it in the original in/pre/postorder array
				//which are supposed to outuput
				copy(inc, in, 0, total);
				copy(prec, pre, 0, total);
				copy(postcc, post, 0, total);
			}
		}
		//else
			//printf("0\n");
		free(postcc);
	}
	free(re);
	free(precc);
	return 1;
}

//this function is used to judge whether the combination of inorder preorder is proper for the solution 
//input : filled in/preorder array,the copy of a original postorder array,the total number,the record number
//output: if there exists a solution return 1
//		  if the combination can't be a solution return 0
int endcheck(int* inc, int* prec, int* postc, int total, int* record)
{
	int flag,result;
	int right, left;
	//if the new root is not in the inorder array, there can't exist a solution
	result = find(inc, prec[0], 0, total);
	if (result == -1)
		return -1;
	//check whether the first element of the preorder array is equal to the last element of the postorder
	//if not ,there can't be a solution
	flag = checkfirst(prec, postc, total);
	if (flag == 0)
		return 0;
	//if there only one element ,successful(based on result didn't equals to -1)
	else if (flag == 1 && total == 1)
		return 1;
	//divide the problem into two calculations
	//calculate the right part
	//calculate the right part's total number
	right = total - result - 1;
	if (right == 0);
	else
	{
		int* incc = (int*)malloc(sizeof(int) * right);
		int* precc = (int*)malloc(sizeof(int) * right);
		int* postcc = (int*)malloc(sizeof(int) * right);
		//change the size of input 
		copy(inc, incc, result + 1, right);
		copy(prec, precc, result + 1, right);
		copy(postc, postcc, result, right);
		//check the right part
		flag = endcheck(incc, precc, postcc, right, record);
		if (flag == 0)
			return 0;
		//if it isn't  successful, and it doesn't reach more than one solution,return -1
		else if (flag == -1)
			return -1;
		//copy the after changing in/pre/postorder array into the last in/pre/postorder array
		recopy(inc, incc, result + 1, right);
		recopy(prec, precc, result + 1, right);
		recopy(postc, postcc, result, right);
		free(incc);
		free(precc);
		free(postcc);
	}
	//calculate the leftt part
	//calculate the left part's total number
	left = result;
	if (left == 0);
	else
	{
		int* incc = (int*)malloc(sizeof(int) * left);
		int* precc = (int*)malloc(sizeof(int) * left);
		int* postcc = (int*)malloc(sizeof(int) * left);
		copy(inc, incc, 0, left);
		copy(prec, precc, 1, left);
		copy(postc, postcc, 0, left);
		//check the left part
		flag = endcheck(incc, precc, postcc, left, record);
		if (flag == 0)
			return 0;
		//if it isn't  successful, and it doesn't reach more than one solution,return -1
		else if (flag == -1)
			return -1;
		//copy the after changing in/pre/postorder array into the last in/pre/postorder array
		recopy(inc, incc, 0, left);
		recopy(prec, precc, 1, left);
		recopy(postc, postcc, 0, left);
		free(incc);
		free(precc);
		free(postcc);
	}
	return 1;
}

// this function is used to build a tree with in/postorder array
//input : in/postreder array,the total numebr,the depth of node
//output: the root of the tree
node* bulidtree(int in[], int post[], int total, int degree)
{
	node* root;
	root = (node*)malloc(sizeof(struct tree));
	//copy the number into the root of the tree
	root->number = post[total - 1];
	root->Degree = degree;
	//define the right subtree and left subtree
	root->Left = NULL;
	root->Right = NULL;
	//if it is a leaf, return the root
	if (total == 1)
		return root;

	int index, i;
	int next;
	node* left;
	node* right;
	left = (node*)malloc(sizeof(struct tree));
	right = (node*)malloc(sizeof(struct tree));
	//calculate the dividing line of the right part and the left part
	for (index = 0;index < total;index++)
		if (in[index] == root->number)
			break;
	next = total - index - 1;
	//copy the right part of the in/postorder array
	int* other = (int*)malloc(sizeof(int) * next);
	for (i = 0;i < next;i++)
		other[i] = in[index + i + 1];
	int* another = (int*)malloc(sizeof(int) * next);
	for (i = 0;i < next;i++)
		another[i] = post[index + i];
	//change the depth of the node(deeper)
	degree++;
	if (next != 0)
		//build the right subtree
		right = bulidtree(other, another, next, degree);
	else
		//when the node didn't have a right children
		right = NULL;
	if (index != 0)
		//build the left subtree
		left = bulidtree(in, post, index, degree);
	else
		//when the node didn't have a left children
		left = NULL;
	root->Left = left;
	root->Right = right;
	return root;
}

//this function is used to print a tree as a array fram
//input : the root of the tree
//output: nont
void print(node* p)
{
	int out[110][110];
	int i, j;
	//empty the array
	for (i = 0;i < 110;i++)
		for (j = 0;j < 110;j++)
			out[i][j] = 0;
	//make copy of the tree into the array
	preorder(out, p);
	//printf the root straightly
	printf("%d", out[0][0]);
	for (i = 1;i < 110;i++)
	{
		for (j = 0;j < 110;j++)
		{
			//printf other number with a space front 
			if (out[i][j] != 0)
				printf(" %d", out[i][j]);
		}
	}
}

//the function is used to copy a tree into a array for level output
//input : the target array,the root of the tree
//output: none
void preorder(int out[][110], node* p)
{
	int i;
	if (p != NULL)
	{
		for (i = 0;i < 110;i++)
		{
			//find the exact position to write in the data
			if (out[p->Degree][i] == 0)
				break;
		}
		//copy the element of each node into the array by its depth
		out[p->Degree][i] = p->number;
		//recursion deal with right subtree and left subtree
		preorder(out, p->Left);
		preorder(out, p->Right);
	}
}

int  main()
{
	int total = 0, i;
	int flag = 1;
	int rema = 0;
	int* remain = &rema;
	char ch;
	//get the totalnumber
	scanf_s("%c", &ch);
	while (ch != '\n')
	{
		//eliminate the interference of wrong input
		if (ch >= '0' && ch <= '9')
			total = total * 10 + (ch - '0');
		scanf_s("%c", &ch);
	}
	/*scanf_s("%d", &total);
	getchar();*/
	int* in = (int*)malloc(sizeof(int) * total);
	int* pre = (int*)malloc(sizeof(int) * total);
	int* post = (int*)malloc(sizeof(int) * total);
	int* record = (int*)malloc(sizeof(int) * total);
	for (i = 0;i < total;i++)
		record[i] = i + 1;
	//read in the in/pre/postorder data
	read(in, total);
	read(pre, total);
	read(post, total);
	//first check the legitimacy
	checkfirst(pre, post, total);
	//divide the input and output
	printf("--------------------------\n");
	//try to fill the in/pre/postorder,if there only exists only one 0
	fill(in, record, total);
	fill(pre, record, total);
	fill(post, record, total);


	int* inc = (int*)malloc(sizeof(int) * total);
	int* prec = (int*)malloc(sizeof(int) * total);
	int* postc = (int*)malloc(sizeof(int) * total);
	copy(in, inc, 0, total);
	copy(pre, prec, 0, total);
	copy(post, postc, 0, total);
	//try to fill the inorder array 
	flag = fillinorder(in, pre, post, inc, prec, postc, 0, total, remain);
	//if there only one solution ,output it
	if (flag == 1 && *remain == 1)
	{
		output(in, total);
		output(pre, total);
		output(post, total);
		//output by the level of the tree 
		node* p;
		p = (node*)malloc(sizeof(struct tree));
		p = bulidtree(in, post, total, 0);
		print(p);
	}
	//other situation printf impossible
	else
		printf("Impossible");
	free(in);
	free(pre);
	free(post);
	free(inc);
	return 0;
}