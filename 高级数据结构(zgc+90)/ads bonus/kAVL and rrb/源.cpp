#include<iostream>
#include<algorithm>
#include<time.h>

//---***��������˵��***---
//������������
#define n 271
//�����ظ�����
#define m 1000

//��¼��ƽ�����
int outnumber;
//��¼ƽ���߶�
int aveheight;
//��¼k����ֵ
int k = 2;

//---***ͨ�ú���˵��***---------------------------------
//��С�ȽϺ���
//���룺��������1����������2
//���أ���������1/2�еĽϴ�ֵ
int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

// ���߼��㺯��(������ڵ�����Ϊ1��
//���룺tree �ġ�����
//���������tree�ĸ߶�
template <typename T>
int nodehigh(T node)
{
	int height;
	//�սڵ㷵�ظ߶�Ϊ0
	if (node == NULL)
		return 0;
	//Ҷ�ӷ��ظ߶�Ϊ1
	else if (node->left == NULL && node->right == NULL)
		return 1;
	else
		//�����������������height
		height = max(nodehigh(node->left), nodehigh(node->right));
	return height + 1;
}

//---***���ݽṹ����***-------------------
//kAVL
//����kAVL tree���ݽṹ��������������
typedef struct treekAVL kAVL;
typedef kAVL* nodekAVL;
//�������ݼ�¼���������������ڵ�
struct treekAVL {
	int number;
	nodekAVL left;
	nodekAVL right;
	nodekAVL pre;
};

//---***����ȫͨ�ú�������***--------
//���� level ���ģ��
//������������ܸ������������stack��ֹͣ
//˫�ر��գ�δ�������Ҳ��ʵ�֣���֧�ֺ���ɾ������������
int exnumber;
typedef struct stack psta;

//���塰�ࡱ�ѽṹ
struct stack {
	//�����Ҫ˳������Ľڵ�
	//���������޸ĵ�һ��������
	//��ǰʹ�õ����ݽṹ
	nodekAVL notree;
	psta* next;
};

//ʹ��ģ�壬ʹ�������������㲻ͬ��������ʹ��
//��ʼ�����ࡱ�ѽṹ
//���룺��Ҫ�� level ��������� root
//�����ָ���ࡱ�ѽṹ��ָ��
template <typename T>
psta* buildstack(T root)
{
	psta* stacktree = (psta*)malloc(sizeof(psta));
	stacktree->notree = root;
	stacktree->next = NULL;
	return stacktree;
}

//���ࡱ���������
//����ֵ�����ࡱ�ѽṹָ��
//		   ���������
//����ֵ�����º�ġ��ࡱ�Ѳ���
psta* print(psta* head, int number)
{
	//������������������ܸ���������� level ���
	if (number >= n)
		return NULL;
	//��Ϊ������������������
	if (head->notree == NULL)
		return head->next;
	//���������޸ĵ����������
	//��ǰʹ�õ����ݽṹ
	nodekAVL next = head->notree;
	psta* temp = head;
	//�������ࡱ�ѽṹ��ĩβ�����������Ԫ��
	while (temp->next != NULL)
		temp = temp->next;
	//�����ӳ�ġ��ࡱ�ѽڵ㣬���ܵ�ǰ���ݽṹ�ڵ����������
	psta* node1 = (psta*)malloc(sizeof(psta));
	psta* node2 = (psta*)malloc(sizeof(psta));
	node1->notree = next->left;
	node2->notree = next->right;
	//����������˳����
	node2->next = NULL;
	temp->next = node1;
	node1->next = node2;
	//�����ǰ���ࡱ�ѽṹ�нڵ���ֵ
	printf("%d ", head->notree->number);
	//�ۼ�������ڵ���
	exnumber++;
	return head->next;
}

//---***kAVL***---
//kAVL��ʼ������
//����ֵ����Ҫ����/��ʼ������ֵ
//����ֵ����ʼ���Ľڵ�ָ��
nodekAVL buildkAVL(int in_num)
{
	nodekAVL root = new kAVL;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//Ѱ�ҽڵ㺯��
//������ͬ��ֵ�����ظýڵ㣬�������򷵻���ͽڵ�
//����ֵ����ҪѰ���ض���ֵ�ģ��ӣ���root���ض�����
//����ֵ�����и���ֵ�Ľڵ�/BS����͵ĸ��ڵ�
nodekAVL findkAVL(nodekAVL root, int in_num)
{
	nodekAVL temp = root;
	nodekAVL q = root;
	//���ڵ㲻ΪNULL�������Ѱ��
	while (q != NULL)
	{
		temp = q;
		//���ݽڵ�ֵ��Ŀ��ֵ��С��ϵ����Ѱ�ҷ���
		if (in_num > temp->number)
			q = temp->right;
		else if (in_num < temp->number)
			q = temp->left;
		//����ȣ��򷵻ص�ǰ�ڵ�
		else
			return temp;
	}
	return temp;
}

//-*������ת�ຯ��*-
//������ת����
//����ֵ������������kAVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void kAVLLL(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
	//����游��Ϊ���ڵ�
	if (r != root)
	{
		//ֱ�ӽ�preָ�븳ֵ����
		l = r->pre;
		if (l->left == r)
			l->left = q;
		else if (l->right == r)
			l->right = q;
		q->pre = l;
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->left = q->right;
		//�սڵ㲻��ֱ�ӵ���pre
		if (q->right != NULL)
			q->right->pre = r;
		q->right = r;
		r->pre = q;
	}
	//���游�ڵ�Ϊ���ڵ㣬С�Ĵ���preָ��
	else
	{
		q->pre = r->pre;
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->left = q->right;
		//�սڵ㲻��ֱ�ӵ���pre
		if (q->right != NULL)
			q->right->pre = r;
		q->right = r;
		r->pre = q;
	}
	//�����ϴ����ۼ�
	outnumber++;
}

//������ת����
//����ֵ������������kAVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void kAVLRR(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
	//����游��Ϊ���ڵ�
	if (r != root)
	{
		//ֱ�ӽ�preָ�븳ֵ����
		l = r->pre;
		if (l->left == r)
			l->left = q;
		else if (l->right == r)
			l->right = q;
		q->pre = l;
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->right = q->left;
		//�սڵ㲻��ֱ�ӵ���pre
		if (q->left != NULL)
			q->left->pre = r;
		q->left = r;
		r->pre = q;
	}
	//���游�ڵ�Ϊ���ڵ㣬С�Ĵ���preָ��
	else
	{
		q->pre = r->pre;
		//����������ת��������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->right = q->left;
		//�սڵ㲻��ֱ�ӵ���pre
		if (q->left != NULL)
			q->left->pre = r;
		q->left = r;
		r->pre = q;
	}
	//�����ϴ����ۼ�
	outnumber++;
}

//������ת����
//����ֵ������������kAVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void kAVLLR(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
	//����游��Ϊ���ڵ�
	if (r != root)
	{
		//�սڵ㲻��ֱ�ӵ���pre
		l = r->pre;
		if (l->left == r)
			l->left = p;
		else if (l->right == r)
			l->right = p;
		p->pre = l;
		//ֱ�ӽ�preָ�븳ֵ����
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->left = p->right;
		if (p->right != NULL)
			p->right->pre = r;
		p->right = r;
		r->pre = p;
		q->right = p->left;
		if (p->left != NULL)
			p->left->pre = q;//1
		p->left = q;
		q->pre = p;
	}
	//���游�ڵ�Ϊ���ڵ㣬С�Ĵ���preָ��
	else
	{
		p->pre = r->pre;
		//ֱ�ӽ�preָ�븳ֵ����
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		r->left = p->right;
		if (p->right != NULL)
			p->right->pre = r;
		p->right = r;
		r->pre = p;
		q->right = p->left;
		if (p->left != NULL)
			p->left->pre = q;//2
		p->left = q;
		q->pre = p;
	}
	//�����ϴ����ۼ�
	outnumber++;
}

//������ת����
//����ֵ������������kAVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void kAVLRL(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
	//����游��Ϊ���ڵ�
	if (r != root)
	{
		//ֱ�ӽ�preָ�븳ֵ����
		l = r->pre;
		if (l->left == r)
			l->left = p;
		else if (l->right == r)
			l->right = p;
		p->pre = l;
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		//�սڵ㲻��ֱ�ӵ���pre
		r->right = p->left;
		if (p->left != NULL)
			p->left->pre = r;
		p->left = r;
		r->pre = p;
		q->left = p->right;
		if (p->right != NULL)
			p->right->pre = q;//3
		p->right = q;
		q->pre = p;
	}
	//���游�ڵ�Ϊ���ڵ㣬С�Ĵ���preָ��
	else
	{
		p->pre = r->pre;
		//����������ת��������ָ���ָ��
		//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
		//�սڵ㲻��ֱ�ӵ���pre
		r->right = p->left;
		if (p->left != NULL)
			p->left->pre = r;
		p->left = r;
		r->pre = p;
		q->left = p->right;
		if (p->right != NULL)
			p->right->pre = q;//4
		p->right = q;
		q->pre = p;
	}
	//�����ϴ����ۼ�
	outnumber++;
}

//���뺯��
//����ֵ����ǰkAVL����root��������ֵ
//���ֵ�����º�����root
nodekAVL insertkAVL(nodekAVL root, int in_num)
{
	nodekAVL temp = new kAVL;
	nodekAVL q, p;
	int result;
	//����Ϊ�գ���������
	if (root == NULL)
		root = buildkAVL(in_num);
	//�����ǿգ���Ѱ��Ӧ�ò���λ�õĸ��ڵ�
	else
	{
		q = findkAVL(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//���ݴ�С�жϲ������λ��
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
		//�����У���Ƿ�����ƽ������
		while (q != root)
		{
			p = q->pre;
			//�������������߶Ȳ�ֵ
			result = abs(nodehigh(p->left) - nodehigh(p->right));
			//������������ת��������ʵ��
			if (result > k && p->left == q && q->left == temp)
			{
				kAVLLL(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = q;
				break;
			}
			//������������ת��������ʵ��
			else if (result > k && p->left == q && q->right == temp)
			{
				kAVLLR(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = temp;
				break;
			}
			//������������ת��������ʵ��
			else if (result > k && p->right == q && q->left == temp)
			{
				kAVLRL(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = temp;
				break;
			}
			//������������ת��������ʵ��
			else if (result > k && p->right == q && q->right == temp)
			{
				kAVLRR(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = q;
				break;
			}
			//���¸��ڵ�
			temp = q;
			q = temp->pre;
		}
	}
	root->pre = NULL;
	return root;
}

//-*ɾ����ת�ຯ��*-
//������������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ڵ㣬�ӽڵ�
//����ֵ�����º�ڵ�ָ��
nodekAVL kAVLsl(nodekAVL p, nodekAVL q)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	q->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = q;
	else
		p->pre->right = q;
	//���ݵ���������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	p->right = q->left;
	//�սڵ㲻��ֱ�ӵ���pre
	if (q->left != NULL)
		q->left->pre = p;
	q->left = p;
	p->pre = q;
	//�����ϴ����ۼ�
	outnumber++;
	return q;
}

//������������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ڵ㣬�ӽڵ�
//����ֵ�����º�ڵ�ָ��
nodekAVL kAVLsr(nodekAVL p, nodekAVL q)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	q->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = q;
	else
		p->pre->right = q;
	//���ݵ���������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	p->left = q->right;
	//�սڵ㲻��ֱ�ӵ���pre
	if (q->right != NULL)
		q->right->pre = p;
	q->right = p;
	p->pre = q;
	//�����ϴ����ۼ�
	outnumber++;
	return q;
}

//˫����������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ڵ㣬�ӽڵ㣬��ڵ�
//����ֵ�����º�ڵ�ָ��
nodekAVL kAVLdl(nodekAVL p, nodekAVL q, nodekAVL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	r->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = r;
	else
		p->pre->right = r;
	//����˫��������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	p->right = r->left;
	if (r->left != NULL)
		r->left->pre = p;
	r->left = p;
	p->pre = r;
	q->left = r->right;
	if (r->right != NULL)
		r->right->pre = q;
	r->right = q;
	q->pre = r;
	//�����ϴ����ۼ�
	outnumber++;
	return r;
}

//˫����������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ڵ㣬�ӽڵ㣬��ڵ�
//����ֵ�����º�ڵ�ָ��
nodekAVL kAVLdr(nodekAVL p, nodekAVL q, nodekAVL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	r->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = r;
	else
		p->pre->right = r;
	//����˫��������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	p->left = r->right;
	if (r->right != NULL)
		r->right->pre = p;
	r->right = p;
	p->pre = r;
	q->right = r->left;
	if (r->left != NULL)
		r->left->pre = q;
	r->left = q;
	q->pre = r;
	//�����ϴ����ۼ�
	outnumber++;
	return r;
}

//ɾ������
//����ֵ����ǰAVL����root����Ҫɾ������ֵ
//����ֵ�����º�����root
nodekAVL deletionkAVL(nodekAVL root, int in_num)
{
	nodekAVL q = new kAVL;
	nodekAVL temp, p;
	int record, result, check;
	//Ѱ����Ҫɾ����ֵ�Ľڵ�λ��
	q = findkAVL(root, in_num);
	temp = q->pre;
	//�������ڸ���ֵ
	if (q->number != in_num)
	{
		//�������ɾ�����
		//printf("delete %d is a bad choose\n",in_num);
		return root;
	}
	//��ΪҶ�ڵ㣬��ֱ��ɾ������
	else if (q->left == NULL && q->right == NULL)
	{
		//���ݾ���λ�ý����ڵ����/��������ΪNULL
		if (temp->left == q)
			temp->left = NULL;
		else
			temp->right = NULL;
		q = NULL;
		//���ɾ�������Ƿ����ƽ��
		while (temp != NULL)
		{
			//�������������߶Ȳ�ֵ
			result = abs(nodehigh(temp->left) - nodehigh(temp->right));
			if (result <= k);
			else
			{
				//���ݵ�ǰ�ڵ�Э��ָ��ָ��ڵ�
				if (temp->left == q)
					p = temp->right;
				else
					p = temp->left;
				//���ӽڵ����
				if (p == temp->right)
				{
					//�Ƚ���һ֧�����ĸ߶Ȳ�ֵ
					check = nodehigh(p->left) - nodehigh(p->right);
					//���������������������������������Ƚϣ�
					if (check < 0)
						//���е�������
						temp = kAVLsl(temp, p);
					else
						//����˫������
						temp = kAVLdl(temp, p, p->left);
				}
				//���ӽڵ����
				else
				{
					//�Ƚ���һ֧�����ĸ߶Ȳ�ֵ
					check = nodehigh(p->right) - nodehigh(p->left);
					//���������������������������������Ƚϣ�
					if (check < 0)
						//���е�������
						temp = kAVLsr(temp, p);
					else
						//����˫������
						temp = kAVLdr(temp, p, p->right);
				}
				if (temp->pre == NULL);
					return temp;
			}
			q = temp;
			temp = temp->pre;
		}
	}
	//���ýڵ㲻ΪҶ�ӽڵ㣬��û������������Ѱ����������Сֵ
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//����ִ��ɾ������
		p = deletionkAVL(p, p->number);
		q->number = record;
	}
	//��������������Ѱ�����������ֵ
	else
	{
		p = q->left;
		while (p->right != NULL)
			p = p->right;
		record = p->number;
		//����ִ��ɾ������
		p = deletionkAVL(p, p->number);
		q->number = record;
	}
	return root;
}

int main()
{
	//��������������鲢�������������
	int* record = (int*)malloc(sizeof(int) * n);
	for (int i = 0;i < n;i++)
		scanf_s("%d", &record[i]);

	//������Ҫ������������ÿ��һ�ã�
	nodekAVL root = NULL;

	//������һ����node�����ڿ��ƱȽ���ͬ���룩
	nodekAVL receive = NULL;

	//��0��ƽ�����������
	outnumber = 0;
	//��0�ۼ�������λ
	aveheight = 0;

	//����ʱ���¼����
	clock_t start, end;
	//��ʼ��¼ʱ��
	start = clock();

	//��������record����ʼִ�в����ɾ������
	//��ѭ��Ϊ�ظ�����
	for (int i = 0;i < m;i++)
	{
		//ÿ������root��ʱ�俪���ɺ��Բ���
		root = NULL;
		//��ѭ��������õĲ�����ɾ��
		for (int j = 0;j < n;j++)
		{
			//���ݴ���0��ʾ����
			if (record[j] > 0)
			{
				//kAVLtree
				receive = findkAVL(root, record[j]);
				if (receive == NULL)
					root = insertkAVL(root, record[j]);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertkAVL(root, record[j]);
			}
			//����С��0��ʾɾ��
			else
			{
				//��rootΪ�գ����ɾ��������Ϣ
				if (root == NULL)
				{
					//printf("bad choose, the root is empty\n");
					continue;
				}
				root = deletionkAVL(root, -record[j]);
			}
		}
	}

	//ֹͣʱ���¼
	end = clock();

	//���������ʱ��
	printf("total time = %f\n", (double)(end - start) / CLOCKS_PER_SEC);
	//���������ƽ������Ĵ���
	printf("error time = %d\n", outnumber / m);

	//Ϊ�˷��㣬����һ�֣��������ƽ�����ߣ���Ӱ��ʱ��ļ��㣩
	root = NULL;
	for (int j = 0;j < n;j++)
	{
		if (record[j] > 0)
		{
			//kAVLtree
			receive = findkAVL(root, record[j]);
			if (receive == NULL)
				root = insertkAVL(root, record[j]);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertkAVL(root, record[j]);
		}
		//����С��0��ʾɾ��
		else
		{
			//��rootΪ�գ����ɾ��������Ϣ
			if (root == NULL)
			{
				//printf("bad choose, the root is empty\n");
				continue;
			}
			root = deletionkAVL(root, -record[j]);
		}
		//�ۼӲ���/ɾ��������������
		aveheight += nodehigh(root);
	}

	//����ա��ࡱ�Ѳ��������ࡱ��
	psta* stacktree = NULL;
	stacktree = buildstack(root);
	//��0�����node�ĸ���
	exnumber = 0;
	//��level������ĸ����ڵ�
	while (stacktree != NULL)
		stacktree = print(stacktree, exnumber);
	printf("\n");

	//������������ƽ���߶�
	printf("average height = %lf\n", 1.0 * aveheight / n );

	return 0;
}

