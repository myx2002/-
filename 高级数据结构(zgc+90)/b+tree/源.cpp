#include<stdio.h>
#include<stdlib.h>
#define n 3
int flag[20] = { 0 };
int re[20] = { 0 };
int be = 0;
int reb = 0;;
int flagb = 0;
struct tree {
	struct tree* child[n+1];
	int array[n];
	int current;
	struct tree* parent;
	int judge;
};
typedef struct tree* node;
struct stack {
	node tree;
	struct stack* next;
};
typedef struct stack* no;
node insertparent(node* next, int record, node* root);
node buildtree(int record)
{
	node root = (node)malloc(sizeof(struct tree));
	root->judge = 1;
	root->parent = NULL;
	root->current = 1;
	for (int i = 0;i < n;i++)
		root->array[i] = -1;
	for (int i = 0;i < n + 1;i++)
		root->child[i] = NULL;
	root->array[root->current-1]= record;
	return root;
}
int place(node next, int record)
{
	int i;
	for (i = 0;i < next->current;i++)
		if (record <= next->array[i])
			break;
	return i;
}
int check(int a[])
{
	int i;
	for (i = 0;i < 19;i++)
		if (a[i] == 0)
			return i;
	return -1;
}
node splitparent(node* next, int record, node* root)
{
	int find = place(*next, record);
	node subone = (node)malloc(sizeof(struct tree));
	node temp;
	node parent;
	int number;
	//int flag;
	int we = check(flag);
	int ew = check(re);
	if (find == 2)
	{
		subone = buildtree(record);
		number = (*next)->array[1];
		flag[we] = 1;
	}
	else if (find == 1)
	{
		subone = buildtree((*next)->array[1]);
		number = record;
		subone->child[1] = (*next)->child[2];
		flag[we] = 2;
	}
	else if (find == 0)
	{
		subone = buildtree((*next)->array[1]);
		number = (*next)->array[0];
		(*next)->array[find] = record;
		subone->child[1] = (*next)->child[2];
		subone->child[0] = (*next)->child[1];
		flag[we] = 3;
	}
	subone->judge = 0;
	(*next)->array[1] = -1;
	(*next)->current--;
	(*next)->child[2] = NULL;
	if ((*next)->parent == NULL)
	{
		parent = buildtree(number);
		parent->judge = 0;
		parent->child[0] = *next;
		parent->child[1] = subone;
		subone->parent = parent;
		(*next)->parent = parent;
		*root = parent;
		if (flag[we] == 2)
			re[ew] = 1;
	}
	else
	{
		temp = insertparent(&(*next)->parent, number, root);
		find = place(temp, number);
		temp->child[find] = *next;
		temp->child[find + 1] = subone;
		we = check(flag) - 1;
		ew = check(re);
		if (flag[we] == 2)
			re[ew] = find + 1;
	}
	if (flag[we] == 3)
		return *next;
	else if (flag[we] == 1)
		return subone;
	else 
		return *next;
}
node insertparent(node* next, int record, node* root)
{
	int find = place(*next, record);
	int i;
	node parent;
	if ((*next)->current < n - 1)
	{
		(*next)->current++;
		for (i = (*next)->current - 1;i > find;i--)
		{
			(*next)->array[i] = (*next)->array[i - 1];
			(*next)->child[i + 1] = (*next)->child[i];
		}
		(*next)->array[find] = record;
		return *next;
	}
	else
	{
		parent = splitparent(next, record, root);
		int we = check(flag) - 1;
		if (flag[we] != 2)
			flag[we] = 0;
		return parent;
	}
}
void splitnode(node* next, int record, node* root)
{
	node subone = (node)malloc(sizeof(struct tree));
	node parent;
	int number;
	int fin = place(*next, record);
	int we = check(flag)-1;
	int ew = check(re)-1;
	if (fin == n)
	{
		subone = buildtree((*next)->array[2]);
		subone->array[1] = record;
	}
	else if (fin == 2)
	{
		subone = buildtree(record);
		subone->array[1] = (*next)->array[2];
	}
	else if (fin == 1 || fin == 0)
	{
		subone = buildtree((*next)->array[1]);
		subone->array[1] = (*next)->array[2];
		if (fin == 0)
			(*next)->array[1] = (*next)->array[0];
		(*next)->array[fin] = record;
	}
	subone->current += 1;
	(*next)->current--;
	(*next)->array[2] = -1;
	(*next)->child[n] = subone;
	if ((*next)->parent == NULL)
	{
		parent = buildtree(subone->array[0]);
		parent->judge = 0;
		parent->child[0] = *next;
		parent->child[1] = subone;
		subone->parent = parent;
		(*next)->parent = parent;
		*root = parent;
	}
	else
	{
		parent = insertparent(&(*next)->parent, subone->array[0], root);
		int find = place(parent, subone->array[0]);
		we = check(flag) - 1;
		ew = check(re) - 1;
		if (flag[we] == 2)
		{
			parent->child[find] = *next;
			parent->parent->child[re[ew]]->child[0] = subone;//re
			re[ew] = 0;
			flag[we] = 0;
			return;
		}
		parent->child[find] = *next;
		parent->child[find + 1] = subone;
	}
}
void insertkey(node* next, int record, node* root)
{
	int i;
	for(i = 0;i < (*next)->current;i++)
		if (record == (*next)->array[i])
		{
			printf("Key %d is duplicated\n", (*next)->array[i]);
			return;
		}
	if ((*next)->current < n)
	{
		for (i = 0;i < (*next)->current;i++)
		{
			if (record < (*next)->array[i])
			{
				for (int j = n - 1;j > i;j--)
					(*next)->array[j] = (*next)->array[j - 1];
				break;
			}
			else
				continue;
		}
		(*next)->array[i] = record;
		(*next)->current += 1;
	}
	else
		splitnode(next, record, root);
}
void insert(node* root, int record)
{
	int i = 0;
	int j;
	node next = *root;
	node temp = *root;
	if (next->judge == 1)
		insertkey(&next, record, root);
	else
	{
		while (next->judge != 1)
		{
			j = next->current;
			for (i = 0;i < j;i++)
			{
				if (record < next->array[i])
				{
					temp= next->child[i];
					temp->parent = next;
					next = temp;
					break;
				}
			}
			if (i == j)
			{
				temp = next->child[i];
				temp->parent = next;
				next = temp;
			}
		}
		insertkey(&next, record, root);
	}
}
no create(node point)
{
	no record = (no)malloc(sizeof(struct stack));
	record->tree = point;
	record->next = NULL;
	return record;
}
void print(no head)
{
	node next = head->tree;
	no temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	printf("[");
	for (int i = 0;i < next->current;i++)
	{
		if (i == 0)
			printf("%d", next->array[i]);
		else
			printf(",%d", next->array[i]);
	}
	if (flagb == 1)
		be--;
	else if (flagb == -1)
		reb--;
	printf("]");
	if (next->judge == 1)
		return;
	for (int i = 0;i < next->current + 1;i++)
	{
		no point = create(next->child[i]);
		temp->next = point;
		temp = temp->next;
		if (flagb == 1)
			reb++;
		else if (flagb == -1)
			be++;
	}
	if ((flagb == 1 && be == 0) || (flagb == -1 && re == 0))
		printf("\n");
	return;
}
int main()
{
	int number;
	int record;
	struct tree* root = (node)malloc(sizeof(struct tree));
	struct stack* head = (no)malloc(sizeof(struct stack));
	scanf_s("%d", &number);
	for (int i = 0;i < number;i++)
	{
		scanf_s("%d", &record);
		if (i == 0)
			root = buildtree(record);
		else
			insert(&root, record);
	}
	reb = 0;
	flagb = 1;
	be = 1;
	head = create(root);
	while (head != NULL)
	{
		print(head);
		head = head->next;
		if (be == 0 || re == 0)
			flagb = -flagb;
	}
	return 0;
}