#include<iostream>
#include<algorithm>
#include<time.h>
//--------***��project�Լ�¼����Ϊ��***---------------
//����˵����
//	�����������������ʾ�������
//	�����븺���������ʾɾ������
//---***ע�⣺���������ͬ����������ɾ��δ���ڵ���***----
//---***	  ÿ�ε�������������ظ��������������Ϳ�����Ҫ�ֶ�����***---

//---------***���벿��***-------------------------------

//---***��������˵��***---
//������������
#define n 668
//�����ظ�����
#define m 100
//���ܸ߼�¼����
double aveheight;

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

//BST
//����BST tree���ݽṹ��������������
typedef struct treeBST BST;
typedef BST* nodeBST;
//�������ݼ�¼���������������ڵ�
struct treeBST {
	int number;
	nodeBST left;
	nodeBST right;
	nodeBST pre;
};


//AVL
//����AVL tree���ݽṹ��������������
typedef struct treeAVL AVL;
typedef AVL* nodeAVL;
//�������ݼ�¼���������������ڵ�
struct treeAVL {
	int number;
	nodeAVL left;
	nodeAVL right;
	nodeAVL pre;
};


//SPL
//����splay tree���ݽṹ��������������
typedef struct treeSPL SPL;
typedef SPL* nodeSPL;
//�������ݼ�¼���������������ڵ�
struct treeSPL {
	int number;
	nodeSPL left;
	nodeSPL right;
	nodeSPL pre;
};


//BB
//����BB[��]���ݽṹ��������������
typedef struct treeBB BB;
typedef BB* nodeBB;
//�������ݼ�¼���������������ڵ�
struct treeBB {
	int number;
	nodeBB left;
	nodeBB right;
	nodeBB pre;
};

//---***����ȫͨ�ú�������***--------
// ��������ע�⣺��Ϊ�������ޣ�������Ҫ����������ľ��������޸�����ģ�飬
//				��Ҫ�޸ĵĵط����������ѱ�����������㣬�����			������
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
	nodeAVL notree;
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
	nodeAVL next = head->notree;
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


//---***��ͬ���ݽṹ��tree���ľ��庯�����***--------------
//����AVL/BB tree���쳣����ͳ�Ʊ���
int outnumber;


//---***BST***---
//BST��ʼ������
//����ֵ����Ҫ����/��ʼ������ֵ
//����ֵ����ʼ���Ľڵ�ָ��
nodeBST buildBST(int in_num)
{
	nodeBST root = new BST;
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
nodeBST findBST(nodeBST root, int in_num)
{
	nodeBST temp = root;
	nodeBST q = root;
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

//���뺯��
//����ֵ����ǰAVL����root��������ֵ
//���ֵ�����º�����root
nodeBST insertBST(nodeBST root, int in_num)
{
	nodeBST temp = new BST;
	nodeBST q;
	//����Ϊ�գ���������
	if (root == NULL)
		root = buildBST(in_num);
	else
	{
		//�����ǿգ���Ѱ��Ӧ�ò���λ�õĸ��ڵ�
		q = findBST(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//���ݴ�С�жϲ������λ��
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		//���¸��ڵ�
		temp->pre = q;
	}
	return root;
}

//ɾ������
//����ֵ����ǰAVL����root����Ҫɾ������ֵ
//����ֵ�����º�����root
nodeBST deletionBST(nodeBST root, int in_num)
{
	nodeBST q = new BST;
	nodeBST temp, p;
	int record;
	//Ѱ����Ҫɾ����ֵ�Ľڵ�λ��
	q = findBST(root, in_num);
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
	}
	//���ýڵ㲻ΪҶ�ӽڵ㣬��û������������Ѱ����������Сֵ
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//����ִ��ɾ������
		p = deletionBST(p, p->number);
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
		p = deletionBST(p, p->number);
		q->number = record;
	}
	return root;
}


//---***AVL***---
//AVL��ʼ������
//����ֵ����Ҫ����/��ʼ������ֵ
//����ֵ����ʼ���Ľڵ�ָ��
nodeAVL buildAVL(int in_num)
{
	nodeAVL root = new AVL;
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
nodeAVL findAVL(nodeAVL root, int in_num)
{
	nodeAVL temp = root;
	nodeAVL q = root;
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
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void AVLLL(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
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
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void AVLRR(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
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
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void AVLLR(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
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
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ㣻���ڵ�
//����ֵ����
void AVLRL(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
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
//����ֵ����ǰAVL����root��������ֵ
//���ֵ�����º�����root
nodeAVL insertAVL(nodeAVL root, int in_num)
{
	nodeAVL temp = new AVL;
	nodeAVL q,p;
	int result;
	//����Ϊ�գ���������
	if (root == NULL)
		root = buildAVL(in_num);
	//�����ǿգ���Ѱ��Ӧ�ò���λ�õĸ��ڵ�
	else
	{
		q = findAVL(root, in_num);
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
			if (result > 1 && p->left == q && q->left == temp)
			{
				AVLLL(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = q;
				break;
			}
			//������������ת��������ʵ��
			else if (result > 1 && p->left == q && q->right == temp)
			{
				AVLLR(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = temp;
				break;
			}
			//������������ת��������ʵ��
			else if (result > 1 && p->right == q && q->left == temp)
			{
				AVLRL(temp, q, p, root);
				//����Ϊ���ڵ㣬������ѭ��
				if (p == root)
					root = temp;
				break;
			}
			//������������ת��������ʵ��
			else if (result > 1 && p->right == q && q->right == temp)
			{
				AVLRR(temp, q, p, root);
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
nodeAVL AVLsl(nodeAVL p, nodeAVL q)
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
nodeAVL AVLsr(nodeAVL p, nodeAVL q)
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
nodeAVL AVLdl(nodeAVL p, nodeAVL q, nodeAVL r)
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
nodeAVL AVLdr(nodeAVL p, nodeAVL q, nodeAVL r)
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
nodeAVL deletionAVL(nodeAVL root, int in_num)
{
	nodeAVL q = new AVL;
	nodeAVL temp, p;
	int record, result, check;
	//Ѱ����Ҫɾ����ֵ�Ľڵ�λ��
	q = findAVL(root, in_num);
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
			if (result <= 1);
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
					if ((check == 0 && nodehigh(p->left) == nodehigh(q) + 1)|| (check == -1 && nodehigh(p->left) == nodehigh(q)))
						//���е�������
						temp = AVLsl(temp, p);
					else
						//����˫������
						temp = AVLdl(temp, p, p->left);
				}
				//���ӽڵ����
				else
				{
					//�Ƚ���һ֧�����ĸ߶Ȳ�ֵ
					check = nodehigh(p->right) - nodehigh(p->left);
					//���������������������������������Ƚϣ�
					if ((check == 0 && nodehigh(p->right) == nodehigh(q) + 1)|| (check == -1 && nodehigh(p->right) == nodehigh(q)))
						//���е�������
						temp = AVLsr(temp, p);
					else
						//����˫������
						temp = AVLdr(temp, p, p->right);
				}
				if (temp->pre == NULL)
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
		p = deletionAVL(p, p->number);
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
		p = deletionAVL(p, p->number);
		q->number = record;
	}
	return root;
}


//---***SPL***---
//SPL��ʼ������
//����ֵ����Ҫ����/��ʼ������ֵ
//����ֵ����ʼ���Ľڵ�ָ��
//splay ÿ�β���󼴲�ѯ
nodeSPL buildSPL(int in_num)
{
	nodeSPL root = new SPL;
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
nodeSPL findSPL(nodeSPL root, int in_num)
{
	nodeSPL temp = root;
	nodeSPL q = root;
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

//-*������ת����*-
//zigzig��������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ�
//����ֵ�����º�ڵ�ָ��
nodeSPL zzLL(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//����zigzig������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	q->left = p->right;
	if (p->right != NULL)
		p->right->pre = q;
	r->left = q->right;
	if (q->right != NULL)
		q->right->pre = r;
	p->right = q;
	q->pre = p;
	q->right = r;
	r->pre = q;
	return p;
}

//zigzig��������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ�
//����ֵ�����º�ڵ�ָ��
nodeSPL zzRR(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//����zigzig������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	q->right = p->left;
	if (p->left != NULL)
		p->left->pre = q;
	r->right = q->left;
	if (q->left != NULL)
		q->left->pre = r;
	p->left = q;
	q->pre = p;
	q->left = r;
	r->pre = q;
	return p;
}

//zigzag����������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ�
//����ֵ�����º�ڵ�ָ��
nodeSPL zzRL(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//����zigzag��������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	r->right = p->left;
	if (p->left != NULL)
		p->left->pre = r;
	q->left = p->right;
	if (p->right != NULL)
		p->right->pre = q;
	p->left = r;
	q->pre = p;
	p->right = q;
	r->pre = p;
	return p;
}

//zigzag����������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣬�游�ڵ�
//����ֵ�����º�ڵ�ָ��
nodeSPL zzLR(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//����pre�Ƿ�Ϊ���ڵ��������
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//����zigzag��������������ָ���ָ��
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	r->left = p->right;
	if (p->right != NULL)
		p->right->pre = r;
	q->right = p->left;
	if (p->left != NULL)
		p->left->pre = q;
	p->right = r;
	q->pre = p;
	p->left = q;
	r->pre = p;
	return p;
}

//���㵥������ת����
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣨root��
//����ֵ�����º�ڵ�ָ��
nodeSPL zL(nodeSPL p, nodeSPL q)
{
	//���ݶ���������������ָ���ָ��
	p->pre = NULL;
	q->left = p->right;
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	if (p->right != NULL)
		p->right->pre = q;
	p->right = q;
	q->pre = p;
	return p;
}

//���㵥����������
//����ֵ������������AVL�ڵ�ָ�룬�ֱ�Ϊ��ǰ�ӽڵ㣬���ڵ㣨root��
//����ֵ�����º�ڵ�ָ��
nodeSPL zR(nodeSPL p, nodeSPL q)
{
	//���ݶ���������������ָ���ָ��
	p->pre = NULL;
	q->right = p->left;
	//ע��pre���ӽڵ�ָ��ı�ĳɶԳ���
	//�սڵ㲻��ֱ�ӵ���pre
	if (p->left != NULL)
		p->left->pre = q;
	p->left = q;
	q->pre = p;
	return p;
}

//���뺯��
//ÿ�β������һ�Ρ�ð��Ѱ�ҡ�
//����ֵ����ǰAVL����root��������ֵ
//���ֵ�����º�����root
nodeSPL insertSPL(nodeSPL root, int in_num)
{
	nodeSPL temp = new SPL;
	nodeSPL q, p;
	//����Ϊ�գ���������
	if (root == NULL)
		return  buildSPL(in_num);
	else
	{
		//�����ǿգ���Ѱ��Ӧ�ò���λ�õĸ��ڵ�
		q = findSPL(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//���ݴ�С�жϲ������λ��
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
		//�����½ڵ㡰ð�ݡ����½ڵ�
		while (q != root)
		{
			p = q->pre;
			//���������zigzig�ṹ����������ת
			if (q->left == temp && p->left == q)
				temp = zzLL(temp, q, p);
			//�������Ҳ�zigzig�ṹ����������ת
			else if (q->right == temp && p->right == q)
				temp = zzRR(temp, q, p);
			//�������Ҳ�zigzag�ṹ����������ת
			else if (q->left == temp && p->right == q)
				temp = zzRL(temp, q, p);
			//���������zigzag�ṹ����������ת
			else if (q->right == temp && p->left == q)
				temp = zzLR(temp, q, p);
			//���¸��ڵ�
			q = temp->pre;
			//����Ϊ���ڵ㣬��ֹͣ��ð�ݡ�����
			if (q == NULL)
				return temp;
		}
		//����ʣ��������ʵ�е�������������
		if (q->left == temp)
			temp = zL(temp, q);
		else
			temp = zR(temp, q);
	}
	return temp;
}

//ɾ������
//����ֵ����ǰAVL����root����Ҫɾ������ֵ
//����ֵ�����º�����root
nodeSPL deletionSPL(nodeSPL root, int in_num)
{
	nodeSPL q = new SPL;
	nodeSPL temp, p;
	int record;
	//Ѱ����Ҫɾ����ֵ�Ľڵ�λ��
	q = findSPL(root, in_num);
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
	}
	//���ýڵ㲻ΪҶ�ӽڵ㣬��û������������Ѱ����������Сֵ
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//����ִ��ɾ������
		p = deletionSPL(p, p->number);
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
		p = deletionSPL(p, p->number);
		q->number = record;
	}
	return root;
}


//---***BB***---
//�����¼���������������
int innumber;

//����С���㺯��
//����ֵ��BB����root
//����ֵ����rootΪ����BB���İ���node����
int sizeBB(nodeBB root)
{
	int size = 0;
	//����Ϊ�գ������0��node
	if (root == NULL)
		return 0;
	//��ΪҶ�ӽڵ㣬�����1��
	else if (root->left == NULL && root->right == NULL)
		return 1;
	//����������еݹ����
	else
		size = sizeBB(root->left) + sizeBB(root->right) + 1;
	return size;
}

//�����¼����
//����ֵ����Ҫ�����¼��BB����root����¼�������е�����ָ��
//����ֵ����
void inorderBB(nodeBB root, int array[])
{
	//��BB��Ϊ�գ���ղ���
	if (root == NULL);
	//����ΪҶ�ӽڵ㣬���¼�������ӳλ��
	else if (root->left == NULL && root->right == NULL)
		array[innumber++] = root->number;
	//�������������ú���
	else
	{
		//��������������
		inorderBB(root->left, array);
		//��������ֵ��¼��������
		array[innumber++] = root->number;
		//��������������
		inorderBB(root->right, array);
	}
}

//BB��ʼ������
//����ֵ����Ҫ����/��ʼ������ֵ
//����ֵ����ʼ���Ľڵ�ָ��
nodeBB buildBB(int in_num)
{
	nodeBB root = new BB;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//�������������ؽ�BBtree����
//����ֵ��ԭBBtree�ĸ��������������飬��ǰ��������������ʼλ�ã���ǰ�����������н�ֹλ��
//����ֵ���½�BBtree��root
nodeBB newBB(nodeBB root, int* array, int start, int end)
{
	nodeBB temp, q;
	//����ʼֵ������ֵֹ��ֱ�ӷ���
	if (start > end)
		return NULL;
	//����ֵ��ȣ������ڵ�󷵻�
	else if (start == end)
	{
		root = buildBB(array[start]);
		return root;
	}
	//��������������м�ֵ��������
	root = buildBB(array[(end + 1 + start) / 2]);
	//����������������
	temp = newBB(root->left, array, start, (end + 1 + start) / 2 - 1);
	q = newBB(root->right, array, (end + 1 + start) / 2 + 1, end);
	root->left = temp;
	//�սڵ㲻��ֱ�ӵ���pre
	if (temp != NULL)
		temp->pre = root;
	root->right = q;
	if (q != NULL)
		q->pre = root;
	return root;
}

//BBtreeƽ�����ú���
//����ֵ����Ҫƽ���BBtree��root��BBtree�Ĵ�С
//����ֵ�����ú��BBtree
nodeBB balanceBB(nodeBB root, int sizepre)
{
	//�����������н�������
	int* array = (int*)malloc(sizeof(int) * sizepre);
	innumber = 0;
	//����BBtree����������
	inorderBB(root, array);
	//�ؽ�BBtree
	nodeBB temp= newBB(NULL, array, 0, sizepre - 1);
	//�ۼ�ƽ��ʧЧ����
	outnumber++;
	return temp;
}

//Ѱ�ҽڵ㺯��
//������ͬ��ֵ�����ظýڵ㣬�������򷵻���ͽڵ�
//����ֵ����ҪѰ���ض���ֵ�ģ��ӣ���root���ض�����
//����ֵ�����и���ֵ�Ľڵ�/BS����͵ĸ��ڵ�
nodeBB findBB(nodeBB root, int in_num)
{
	nodeBB temp = root;
	nodeBB q = root;
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

//���뺯��
//����ֵ����ǰAVL����root��������ֵ
//���ֵ�����º�����root
nodeBB insertBB(nodeBB root, int in_num, double alfa)
{
	nodeBB temp = new BB;
	nodeBB q, p, result;
	int sizepre;
	int sizechl, sizechr;
	//����Ϊ�գ���������
	if (root == NULL)
	{
		root = buildBB(in_num);
		return root;
	}
	else
	{
		//�����ǿգ���Ѱ��Ӧ�ò���λ�õĸ��ڵ�
		q = findBB(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//���ݴ�С�жϲ������λ��
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
	}
	//ÿ�β�����ж��Ƿ����BB������
	while (q != NULL)
	{
		p = q->pre;
		//������븸�ڵ������������С�������㵱ǰ������С
		sizechl = sizeBB(q->left);
		sizechr = sizeBB(q->right);
		sizepre = sizechl + sizechr + 1;
		//�ж��Ƿ�����BBtree����
		if (sizepre * alfa < sizechl || sizepre * alfa < sizechr)
		{
			//�������㣬�����ø���������ϸ�������Ƶ�
			result = balanceBB(q, sizepre);
			//�����������������ã��򷵻�
			if (q == root)
				return  result;
			//������������ԭ�ڵ�ֲ��ض���preֵ
			else if (p->left == q)
			{
				p->left = result;
				result->pre = p;
			}
			else
			{
				p->right = result;
				result->pre = p;
			}
		}
		//���½ڵ�
		q = p;
	}
	return root;
}

//ɾ������
//����ֵ����ǰAVL����root����Ҫɾ������ֵ
//����ֵ�����º�����root
nodeBB deletionBB(nodeBB root, int in_num, double alfa,nodeBB truroot)
{
	nodeBB q = new BB;
	nodeBB temp, p;
	int record;
	int sizepre;
	int sizechl, sizechr;
	//Ѱ����Ҫɾ����ֵ�Ľڵ�λ��
	q = findBB(root, in_num);
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
		//ÿ��ɾ�����ж��Ƿ�����BBtree����
		while (temp != NULL)
		{
			q = temp->pre;
			//������븸�ڵ������������С�������㵱ǰ������С
			sizechl = sizeBB(temp->left);
			sizechr = sizeBB(temp->right);
			sizepre = sizechl + sizechr + 1;
			//�ж��Ƿ�����BBtree����
			if (sizepre * alfa < sizechl || sizepre * alfa < sizechr)
			{
				//�������㣬�����ø�����
				p = balanceBB(temp, sizepre);
				if (temp->pre == NULL)
					return p;
				//������������ԭ�ڵ�ֲ��ض���preֵ
				else if (q->left == temp)
				{
					q->left = p;
					p->pre = q;
				}
				else
				{
					q->right = p;
					p->pre = q;
				}
			}
			//���½ڵ�
			temp = q;
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
		p = deletionBB(p, p->number, alfa,truroot);
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
		p = deletionBB(p, p->number, alfa, truroot);
		q->number = record;
	}
	return root;
}

//------------------***����������***-----------------
int main()
{
	//��������������鲢�������������
	int* record = (int*)malloc(sizeof(int) * n);
	for (int i = 0;i < n;i++)
		scanf_s("%d", &record[i]);

	//������Ҫ������������ÿ��һ�ã�
	nodeAVL root = NULL;
	//nodeBST root = NULL;
	//nodeBB root = NULL;
	//nodeSPL root = NULL;

	//������һ����node�����ڿ��ƱȽ���ͬ���룩
	nodeAVL receive = NULL;
	//nodeBB receive = NULL;
	//nodeBST receive = NULL;
	//nodeSPL receive = NULL;

	//����BB tree ��[��]ϵ��
	double alfa = 0.5;

	//��0��ƽ�������������������AVL tree��BB tree��
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
				//����ѡ����Ӧ�����ݽṹ�Ϳ�

				// BST
				/*receive = findBST(root, record[j]);
				if (receive == NULL)
					root = insertBST(root, record[j]);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertBST(root, record[j]);*/

				//BB
				/*receive = findBB(root, record[j]);
				if (receive == NULL)
					root = insertBB(root, record[j], alfa);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertBB(root, record[j], alfa);*/


				//AVLtree
				receive = findAVL(root, record[j]);
				if (receive == NULL)
					root = insertAVL(root, record[j]);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertAVL(root, record[j]);

				//SPLtree
				/*receive = findSPL(root, record[j]);
				if (receive == NULL)
					root = insertSPL(root, record[j]);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertSPL(root, record[j]);*/
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
				root = deletionAVL(root, -record[j]);
				//root = deletionBST(root, -record[j]);
				//root = deletionBB(root, -record[j], alfa ,root);
				//root = deletionSPL(root, -record[j]);
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
			//����ѡ����Ӧ�����ݽṹ�Ϳ�

			// BST
			/*receive = findBST(root, record[j]);
			if (receive == NULL)
				root = insertBST(root, record[j]);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertBST(root, record[j]);*/

			//BB
			/*receive = findBB(root, record[j]);
			if (receive == NULL)
				root = insertBB(root, record[j], alfa);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertBB(root, record[j], alfa);*/


			//AVLtree
			receive = findAVL(root, record[j]);
			if (receive == NULL)
				root = insertAVL(root, record[j]);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertAVL(root, record[j]);

			//SPLtree
			/*receive = findSPL(root, record[j]);
			if (receive == NULL)
				root = insertSPL(root, record[j]);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertSPL(root, record[j]);*/
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
			//root = deletionAVL(root, -record[j]);
			//root = deletionBST(root, -record[j]);
			//root = deletionBB(root, -record[j], alfa, root);
			//root = deletionSPL(root, -record[j]);
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
	printf("average height = %lf\n", 1.0 * aveheight / n);

	return 0;
}