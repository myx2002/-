#include<iostream>
#include<algorithm>
#include<time.h>
//--------***本project以记录整数为例***---------------
//输入说明：
//	若输入正整数，则表示插入操作
//	若输入负整数，则表示删除操作
//---***注意：允许插入相同的数，允许删除未存在的数***----
//---***	  每次调换输入个数、重复次数及树的类型可能需要手动调整***---

//---------***代码部分***-------------------------------

//---***基础变量说明***---
//定义总输入量
#define n 668
//定义重复次数
#define m 100
//树总高记录参数
double aveheight;

//---***通用函数说明***---------------------------------

//大小比较函数
//输入：整型输入1，整型输入2
//返回：整型输入1/2中的较大值
int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


// 树高计算函数(假设根节点树高为1）
//输入：tree 的”顶点
//输出：输入tree的高度
template <typename T>
int nodehigh(T node)
{
	int height;
	//空节点返回高度为0
	if (node == NULL)
		return 0;
	//叶子返回高度为1
	else if (node->left == NULL && node->right == NULL)
		return 1;
	else
	//返回左右子树的最高height
		height = max(nodehigh(node->left), nodehigh(node->right));
	return height + 1;
}


//---***数据结构定义***-------------------

//BST
//定义BST tree数据结构，并进行重命名
typedef struct treeBST BST;
typedef BST* nodeBST;
//包含数据记录，左右子树及父节点
struct treeBST {
	int number;
	nodeBST left;
	nodeBST right;
	nodeBST pre;
};


//AVL
//定义AVL tree数据结构，并进行重命名
typedef struct treeAVL AVL;
typedef AVL* nodeAVL;
//包含数据记录，左右子树及父节点
struct treeAVL {
	int number;
	nodeAVL left;
	nodeAVL right;
	nodeAVL pre;
};


//SPL
//定义splay tree数据结构，并进行重命名
typedef struct treeSPL SPL;
typedef SPL* nodeSPL;
//包含数据记录，左右子树及父节点
struct treeSPL {
	int number;
	nodeSPL left;
	nodeSPL right;
	nodeSPL pre;
};


//BB
//定义BB[α]数据结构，并进行重命名
typedef struct treeBB BB;
typedef BB* nodeBB;
//包含数据记录，左右子树及父节点
struct treeBB {
	int number;
	nodeBB left;
	nodeBB right;
	nodeBB pre;
};

//---***非完全通用函数定义***--------
// ！！！！注意：因为能力有限，可能需要根据输出树的具体类型修改下列模块，
//				需要修改的地方共两处且已标出，带来不便，请见谅			！！！
//树按 level 输出模块
//定义已输出的总个数，方便控制stack的停止
//双重保险，未添加限制也可实现，如支持含有删除操作的输入
int exnumber;
typedef struct stack psta;

//定义“类”堆结构
struct stack {
	//存放需要顺序输出的节点
	//！！！！修改点一！！！！
	//当前使用的数据结构
	nodeAVL notree;
	psta* next;
};

//使用模板，使函数尽可能满足不同类型树的使用
//初始化“类”堆结构
//输入：需要按 level 输出的树的 root
//输出：指向“类”堆结构的指针
template <typename T>
psta* buildstack(T root)
{
	psta* stacktree = (psta*)malloc(sizeof(psta));
	stacktree->notree = root;
	stacktree->next = NULL;
	return stacktree;
}

//“类”堆输出函数
//输入值：“类”堆结构指针
//		   已输出个数
//返回值：更新后的“类”堆操作
psta* print(psta* head, int number)
{
	//若输出个数大于输入总个数，则结束 level 输出
	if (number >= n)
		return NULL;
	//若为空子树，则跳过此项
	if (head->notree == NULL)
		return head->next;
	//！！！！修改点二！！！！
	//当前使用的数据结构
	nodeAVL next = head->notree;
	psta* temp = head;
	//遍历“类”堆结构的末尾，方便添加新元素
	while (temp->next != NULL)
		temp = temp->next;
	//定义对映的“类”堆节点，接受当前数据结构节点的左右子树
	psta* node1 = (psta*)malloc(sizeof(psta));
	psta* node2 = (psta*)malloc(sizeof(psta));
	node1->notree = next->left;
	node2->notree = next->right;
	//按从左至右顺序存放
	node2->next = NULL;
	temp->next = node1;
	node1->next = node2;
	//输出当前“类”堆结构中节点数值
	printf("%d ", head->notree->number);
	//累计已输出节点数
	exnumber++;
	return head->next;
}


//---***不同数据结构（tree）的具体函数设计***--------------
//定义AVL/BB tree的异常次数统计变量
int outnumber;


//---***BST***---
//BST初始化函数
//输入值：需要插入/初始化的数值
//返回值：初始化的节点指针
nodeBST buildBST(int in_num)
{
	nodeBST root = new BST;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//寻找节点函数
//存在相同数值即返回该节点，不存在则返回最低节点
//输入值：需要寻找特定数值的（子）树root，特定整数
//返回值：含有该数值的节点/BS后最低的父节点
nodeBST findBST(nodeBST root, int in_num)
{
	nodeBST temp = root;
	nodeBST q = root;
	//若节点不为NULL，则继续寻找
	while (q != NULL)
	{
		temp = q;
		//根据节点值与目标值大小关系决定寻找方向
		if (in_num > temp->number)
			q = temp->right;
		else if (in_num < temp->number)
			q = temp->left;
		//若相等，则返回当前节点
		else
			return temp;
	}
	return temp;
}

//插入函数
//输入值：当前AVL树的root，插入数值
//输出值：更新后树的root
nodeBST insertBST(nodeBST root, int in_num)
{
	nodeBST temp = new BST;
	nodeBST q;
	//若树为空，则建立该树
	if (root == NULL)
		root = buildBST(in_num);
	else
	{
		//若树非空，则寻找应该插入位置的父节点
		q = findBST(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//根据大小判断插入具体位置
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		//更新父节点
		temp->pre = q;
	}
	return root;
}

//删除函数
//输入值：当前AVL树的root，需要删除的数值
//返回值：更新后树的root
nodeBST deletionBST(nodeBST root, int in_num)
{
	nodeBST q = new BST;
	nodeBST temp, p;
	int record;
	//寻找需要删除数值的节点位置
	q = findBST(root, in_num);
	temp = q->pre;
	//若不存在该数值
	if (q->number != in_num)
	{
		//输出错误删除结果
		//printf("delete %d is a bad choose\n",in_num);
		return root;
	}
	//若为叶节点，则直接删除即可
	else if (q->left == NULL && q->right == NULL)
	{
		//根据具体位置将父节点的左/右子树置为NULL
		if (temp->left == q)
			temp->left = NULL;
		else
			temp->right = NULL;
	}
	//若该节点不为叶子节点，且没有左子树，则寻找右子树最小值
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//迭代执行删除操作
		p = deletionBST(p, p->number);
		q->number = record;
	}
	//若有左子树，则寻找左子树最大值
	else
	{
		p = q->left;
		while (p->right != NULL)
			p = p->right;
		record = p->number;
		//迭代执行删除操作
		p = deletionBST(p, p->number);
		q->number = record;
	}
	return root;
}


//---***AVL***---
//AVL初始化函数
//输入值：需要插入/初始化的数值
//返回值：初始化的节点指针
nodeAVL buildAVL(int in_num)
{
	nodeAVL root = new AVL;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//寻找节点函数
//存在相同数值即返回该节点，不存在则返回最低节点
//输入值：需要寻找特定数值的（子）树root，特定整数
//返回值：含有该数值的节点/BS后最低的父节点
nodeAVL findAVL(nodeAVL root, int in_num)
{
	nodeAVL temp = root;
	nodeAVL q = root;
	//若节点不为NULL，则继续寻找
	while (q != NULL)
	{
		temp = q;
		//根据节点值与目标值大小关系决定寻找方向
		if (in_num > temp->number)
			q = temp->right;
		else if (in_num < temp->number)
			q = temp->left;
		//若相等，则返回当前节点
		else
			return temp;
	}
	return temp;
}

//-*插入旋转类函数*-
//左左旋转函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void AVLLL(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
	//如果祖父不为根节点
	if (r != root)
	{
		//直接将pre指针赋值即可
		l = r->pre;
		if (l->left == r)
			l->left = q;
		else if (l->right == r)
			l->right = q;
		q->pre = l;
		//根据左左旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
		r->left = q->right;
		//空节点不能直接调用pre
		if (q->right != NULL)
			q->right->pre = r;
		q->right = r;
		r->pre = q;
	}
	//若祖父节点为根节点，小心处理pre指针
	else
	{
		q->pre = r->pre;
		//根据左左旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
		r->left = q->right;
		//空节点不能直接调用pre
		if (q->right != NULL)
			q->right->pre = r;
		q->right = r;
		r->pre = q;
	}
	//不符合次数累加
	outnumber++;
}

//右右旋转函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void AVLRR(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
	//如果祖父不为根节点
	if (r != root)
	{
		//直接将pre指针赋值即可
		l = r->pre;
		if (l->left == r)
			l->left = q;
		else if (l->right == r)
			l->right = q;
		q->pre = l;
		//根据右右旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
		r->right = q->left;
		//空节点不能直接调用pre
		if (q->left != NULL)
			q->left->pre = r;
		q->left = r;
		r->pre = q;
	}
	//若祖父节点为根节点，小心处理pre指针
	else
	{
		q->pre = r->pre;
		//根据右右旋转调整三个指针的指向
	//注意pre与子节点指向改变的成对出现
		r->right = q->left;
		//空节点不能直接调用pre
		if (q->left != NULL)
			q->left->pre = r;
		q->left = r;
		r->pre = q;
	}
	//不符合次数累加
	outnumber++;
}

//左右旋转函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void AVLLR(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
	//如果祖父不为根节点
	if (r != root)
	{
		//空节点不能直接调用pre
		l = r->pre;
		if (l->left == r)
			l->left = p;
		else if (l->right == r)
			l->right = p;
		p->pre = l;
		//直接将pre指针赋值即可
		//根据左右旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
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
	//若祖父节点为根节点，小心处理pre指针
	else
	{
		p->pre = r->pre;
		//直接将pre指针赋值即可
		//根据左右旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
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
	//不符合次数累加
	outnumber++;
}

//右左旋转函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void AVLRL(nodeAVL p, nodeAVL q, nodeAVL r, nodeAVL root)
{
	nodeAVL l;
	//如果祖父不为根节点
	if (r != root)
	{
		//直接将pre指针赋值即可
		l = r->pre;
		if (l->left == r)
			l->left = p;
		else if (l->right == r)
			l->right = p;
		p->pre = l;
		//根据右左旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
		//空节点不能直接调用pre
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
	//若祖父节点为根节点，小心处理pre指针
	else
	{
		p->pre = r->pre;
		//根据右左旋转调整三个指针的指向
		//注意pre与子节点指向改变的成对出现
		//空节点不能直接调用pre
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
	//不符合次数累加
	outnumber++;
}

//插入函数
//输入值：当前AVL树的root，插入数值
//输出值：更新后树的root
nodeAVL insertAVL(nodeAVL root, int in_num)
{
	nodeAVL temp = new AVL;
	nodeAVL q,p;
	int result;
	//若树为空，则建立该树
	if (root == NULL)
		root = buildAVL(in_num);
	//若树非空，则寻找应该插入位置的父节点
	else
	{
		q = findAVL(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//根据大小判断插入具体位置
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
		//插入后校验是否发生不平衡现象
		while (q != root)
		{
			p = q->pre;
			//计算左右子树高度差值
			result = abs(nodehigh(p->left) - nodehigh(p->right));
			//若符合左左旋转条件，则实行
			if (result > 1 && p->left == q && q->left == temp)
			{
				AVLLL(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = q;
				break;
			}
			//若符合左右旋转条件，则实行
			else if (result > 1 && p->left == q && q->right == temp)
			{
				AVLLR(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = temp;
				break;
			}
			//若符合右左旋转条件，则实行
			else if (result > 1 && p->right == q && q->left == temp)
			{
				AVLRL(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = temp;
				break;
			}
			//若符合右右旋转条件，则实行
			else if (result > 1 && p->right == q && q->right == temp)
			{
				AVLRR(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = q;
				break;
			}
			//更新父节点
			temp = q;
			q = temp->pre;
		}
	}
	root->pre = NULL;
	return root;
}

//-*删除旋转类函数*-
//单次左旋函数
//输入值：两个连续的AVL节点指针，分别为当前节点，子节点
//返回值：更新后节点指针
nodeAVL AVLsl(nodeAVL p, nodeAVL q)
{
	//根据pre是否为根节点调整连接
	q->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = q;
	else
		p->pre->right = q;
	//根据单次左旋调整两个指针的指向
	//注意pre与子节点指向改变的成对出现
	p->right = q->left;
	//空节点不能直接调用pre
	if (q->left != NULL)
		q->left->pre = p;
	q->left = p;
	p->pre = q;
	//不符合次数累加
	outnumber++;
	return q;
}

//单次右旋函数
//输入值：两个连续的AVL节点指针，分别为当前节点，子节点
//返回值：更新后节点指针
nodeAVL AVLsr(nodeAVL p, nodeAVL q)
{
	//根据pre是否为根节点调整连接
	q->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = q;
	else
		p->pre->right = q;
	//根据单次右旋调整两个指针的指向
	//注意pre与子节点指向改变的成对出现
	p->left = q->right;
	//空节点不能直接调用pre
	if (q->right != NULL)
		q->right->pre = p;
	q->right = p;
	p->pre = q;
	//不符合次数累加
	outnumber++;
	return q;
}

//双次左旋函数
//输入值：三个连续的AVL节点指针，分别为当前节点，子节点，孙节点
//返回值：更新后节点指针
nodeAVL AVLdl(nodeAVL p, nodeAVL q, nodeAVL r)
{
	//根据pre是否为根节点调整连接
	r->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = r;
	else
		p->pre->right = r;
	//根据双次左旋调整两个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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
	//不符合次数累加
	outnumber++;
	return r;
}

//双次右旋函数
//输入值：三个连续的AVL节点指针，分别为当前节点，子节点，孙节点
//返回值：更新后节点指针
nodeAVL AVLdr(nodeAVL p, nodeAVL q, nodeAVL r)
{
	//根据pre是否为根节点调整连接
	r->pre = p->pre;
	if (p->pre == NULL);
	else if (p->pre->left == p)
		p->pre->left = r;
	else
		p->pre->right = r;
	//根据双次右旋调整两个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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
	//不符合次数累加
	outnumber++;
	return r;
}

//删除函数
//输入值：当前AVL树的root，需要删除的数值
//返回值：更新后树的root
nodeAVL deletionAVL(nodeAVL root, int in_num)
{
	nodeAVL q = new AVL;
	nodeAVL temp, p;
	int record, result, check;
	//寻找需要删除数值的节点位置
	q = findAVL(root, in_num);
	temp = q->pre;
	//若不存在该数值
	if (q->number != in_num)
	{
		//输出错误删除结果
		//printf("delete %d is a bad choose\n",in_num);
		return root;
	}
	//若为叶节点，则直接删除即可
	else if (q->left == NULL && q->right == NULL)
	{
		//根据具体位置将父节点的左/右子树置为NULL
		if (temp->left == q)
			temp->left = NULL;
		else
			temp->right = NULL;
		q = NULL;
		//检测删除操作是否打破平衡
		while (temp != NULL)
		{
			//计算左右子树高度差值
			result = abs(nodehigh(temp->left) - nodehigh(temp->right));
			if (result <= 1);
			else
			{
				//根据当前节点协调指针指向节点
				if (temp->left == q)
					p = temp->right;
				else
					p = temp->left;
				//右子节点情况
				if (p == temp->right)
				{
					//比较另一支子树的高度差值
					check = nodehigh(p->left) - nodehigh(p->right);
					//右子树大于左子树（基于子树的子树比较）
					if ((check == 0 && nodehigh(p->left) == nodehigh(q) + 1)|| (check == -1 && nodehigh(p->left) == nodehigh(q)))
						//进行单次左旋
						temp = AVLsl(temp, p);
					else
						//进行双次右旋
						temp = AVLdl(temp, p, p->left);
				}
				//左子节点情况
				else
				{
					//比较另一支子树的高度差值
					check = nodehigh(p->right) - nodehigh(p->left);
					//左子树大于右子树（基于子树的子树比较）
					if ((check == 0 && nodehigh(p->right) == nodehigh(q) + 1)|| (check == -1 && nodehigh(p->right) == nodehigh(q)))
						//进行单次右旋
						temp = AVLsr(temp, p);
					else
						//进行双次左旋
						temp = AVLdr(temp, p, p->right);
				}
				if (temp->pre == NULL)
					return temp;	
			}
			q = temp;
			temp = temp->pre;
		}
	}
	//若该节点不为叶子节点，且没有左子树，则寻找右子树最小值
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//迭代执行删除操作
		p = deletionAVL(p, p->number);
		q->number = record;
	}
	//若有左子树，则寻找左子树最大值
	else
	{
		p = q->left;
		while (p->right != NULL)
			p = p->right;
		record = p->number;
		//迭代执行删除操作
		p = deletionAVL(p, p->number);
		q->number = record;
	}
	return root;
}


//---***SPL***---
//SPL初始化函数
//输入值：需要插入/初始化的数值
//返回值：初始化的节点指针
//splay 每次插入后即查询
nodeSPL buildSPL(int in_num)
{
	nodeSPL root = new SPL;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//寻找节点函数
//存在相同数值即返回该节点，不存在则返回最低节点
//输入值：需要寻找特定数值的（子）树root，特定整数
//返回值：含有该数值的节点/BS后最低的父节点
nodeSPL findSPL(nodeSPL root, int in_num)
{
	nodeSPL temp = root;
	nodeSPL q = root;
	//若节点不为NULL，则继续寻找
	while (q != NULL)
	{
		temp = q;
		//根据节点值与目标值大小关系决定寻找方向
		if (in_num > temp->number)
			q = temp->right;
		else if (in_num < temp->number)
			q = temp->left;
		//若相等，则返回当前节点
		else
			return temp;
	}
	return temp;
}

//-*插入旋转函数*-
//zigzig左旋函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点
//返回值；更新后节点指针
nodeSPL zzLL(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//根据pre是否为根节点调整连接
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//根据zigzig左旋调整三个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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

//zigzig左旋函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点
//返回值；更新后节点指针
nodeSPL zzRR(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//根据pre是否为根节点调整连接
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//根据zigzig右旋调整三个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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

//zigzag右左旋函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点
//返回值；更新后节点指针
nodeSPL zzRL(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//根据pre是否为根节点调整连接
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//根据zigzag右左旋调整三个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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

//zigzag左右旋函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点，祖父节点
//返回值；更新后节点指针
nodeSPL zzLR(nodeSPL p, nodeSPL q, nodeSPL r)
{
	//根据pre是否为根节点调整连接
	p->pre = r->pre;
	if (r->pre == NULL);
	else if (r->pre->left == r)
		r->pre->left = p;
	else
		r->pre->right = p;
	//根据zigzag左右旋调整三个指针的指向
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
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

//顶点单次左旋转函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点（root）
//返回值；更新后节点指针
nodeSPL zL(nodeSPL p, nodeSPL q)
{
	//根据顶点左旋调整两个指针的指向
	p->pre = NULL;
	q->left = p->right;
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
	if (p->right != NULL)
		p->right->pre = q;
	p->right = q;
	q->pre = p;
	return p;
}

//顶点单次右旋函数
//输入值：三个连续的AVL节点指针，分别为当前子节点，父节点（root）
//返回值；更新后节点指针
nodeSPL zR(nodeSPL p, nodeSPL q)
{
	//根据顶点右旋调整两个指针的指向
	p->pre = NULL;
	q->right = p->left;
	//注意pre与子节点指向改变的成对出现
	//空节点不能直接调用pre
	if (p->left != NULL)
		p->left->pre = q;
	p->left = q;
	q->pre = p;
	return p;
}

//插入函数
//每次插入后作一次“冒泡寻找”
//输入值：当前AVL树的root，插入数值
//输出值：更新后树的root
nodeSPL insertSPL(nodeSPL root, int in_num)
{
	nodeSPL temp = new SPL;
	nodeSPL q, p;
	//若树为空，则建立该树
	if (root == NULL)
		return  buildSPL(in_num);
	else
	{
		//若树非空，则寻找应该插入位置的父节点
		q = findSPL(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//根据大小判断插入具体位置
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
		//插入新节点“冒泡”该新节点
		while (q != root)
		{
			p = q->pre;
			//若存在左侧zigzig结构，则左左旋转
			if (q->left == temp && p->left == q)
				temp = zzLL(temp, q, p);
			//若存在右侧zigzig结构，则右右旋转
			else if (q->right == temp && p->right == q)
				temp = zzRR(temp, q, p);
			//若存在右侧zigzag结构，则右左旋转
			else if (q->left == temp && p->right == q)
				temp = zzRL(temp, q, p);
			//若存在左侧zigzag结构，则左右旋转
			else if (q->right == temp && p->left == q)
				temp = zzLR(temp, q, p);
			//更新父节点
			q = temp->pre;
			//若已为根节点，则停止“冒泡”过程
			if (q == NULL)
				return temp;
		}
		//若仅剩单步，则实行单步左旋或右旋
		if (q->left == temp)
			temp = zL(temp, q);
		else
			temp = zR(temp, q);
	}
	return temp;
}

//删除函数
//输入值：当前AVL树的root，需要删除的数值
//返回值：更新后树的root
nodeSPL deletionSPL(nodeSPL root, int in_num)
{
	nodeSPL q = new SPL;
	nodeSPL temp, p;
	int record;
	//寻找需要删除数值的节点位置
	q = findSPL(root, in_num);
	temp = q->pre;
	//若不存在该数值
	if (q->number != in_num)
	{
		//输出错误删除结果
		//printf("delete %d is a bad choose\n",in_num);
		return root;
	}
	//若为叶节点，则直接删除即可
	else if (q->left == NULL && q->right == NULL)
	{
		//根据具体位置将父节点的左/右子树置为NULL
		if (temp->left == q)
			temp->left = NULL;
		else
			temp->right = NULL;
	}
	//若该节点不为叶子节点，且没有左子树，则寻找右子树最小值
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//迭代执行删除操作
		p = deletionSPL(p, p->number);
		q->number = record;
	}
	//若有左子树，则寻找左子树最大值
	else
	{
		p = q->left;
		while (p->right != NULL)
			p = p->right;
		record = p->number;
		//迭代执行删除操作
		p = deletionSPL(p, p->number);
		q->number = record;
	}
	return root;
}


//---***BB***---
//中序记录数组已填充量变量
int innumber;

//树大小计算函数
//输入值：BB树的root
//返回值：以root为根的BB树的包含node个数
int sizeBB(nodeBB root)
{
	int size = 0;
	//若书为空，则包含0个node
	if (root == NULL)
		return 0;
	//若为叶子节点，则包含1个
	else if (root->left == NULL && root->right == NULL)
		return 1;
	//其他情况进行递归计算
	else
		size = sizeBB(root->left) + sizeBB(root->right) + 1;
	return size;
}

//中序记录函数
//输入值：需要中序记录的BB树的root，记录中序序列的数组指针
//返回值：无
void inorderBB(nodeBB root, int array[])
{
	//若BB数为空，则空操作
	if (root == NULL);
	//若已为叶子节点，则记录至数组对映位置
	else if (root->left == NULL && root->right == NULL)
		array[innumber++] = root->number;
	//其它情况则迭代该函数
	else
	{
		//计算左子树中序
		inorderBB(root->left, array);
		//将根的数值记录于数组中
		array[innumber++] = root->number;
		//计算右子树中序
		inorderBB(root->right, array);
	}
}

//BB初始化函数
//输入值：需要插入/初始化的数值
//返回值：初始化的节点指针
nodeBB buildBB(int in_num)
{
	nodeBB root = new BB;
	root->number = in_num;
	root->left = NULL;
	root->right = NULL;
	root->pre = NULL;
	return root;
}

//根据中序序列重建BBtree函数
//输入值：原BBtree的根，中序序列数组，当前子树中序序列起始位置，当前子树中序序列截止位置
//返回值：新建BBtree的root
nodeBB newBB(nodeBB root, int* array, int start, int end)
{
	nodeBB temp, q;
	//若起始值超过终止值，直接返回
	if (start > end)
		return NULL;
	//若两值相等，建立节点后返回
	else if (start == end)
	{
		root = buildBB(array[start]);
		return root;
	}
	//其余情况，根据中间值建立树根
	root = buildBB(array[(end + 1 + start) / 2]);
	//迭代建立左右子树
	temp = newBB(root->left, array, start, (end + 1 + start) / 2 - 1);
	q = newBB(root->right, array, (end + 1 + start) / 2 + 1, end);
	root->left = temp;
	//空节点不能直接调用pre
	if (temp != NULL)
		temp->pre = root;
	root->right = q;
	if (q != NULL)
		q->pre = root;
	return root;
}

//BBtree平衡重置函数
//输入值：需要平衡的BBtree的root，BBtree的大小
//返回值：重置后的BBtree
nodeBB balanceBB(nodeBB root, int sizepre)
{
	//定义中序序列接收数组
	int* array = (int*)malloc(sizeof(int) * sizepre);
	innumber = 0;
	//建立BBtree的中序序列
	inorderBB(root, array);
	//重建BBtree
	nodeBB temp= newBB(NULL, array, 0, sizepre - 1);
	//累加平衡失效次数
	outnumber++;
	return temp;
}

//寻找节点函数
//存在相同数值即返回该节点，不存在则返回最低节点
//输入值：需要寻找特定数值的（子）树root，特定整数
//返回值：含有该数值的节点/BS后最低的父节点
nodeBB findBB(nodeBB root, int in_num)
{
	nodeBB temp = root;
	nodeBB q = root;
	//若节点不为NULL，则继续寻找
	while (q != NULL)
	{
		temp = q;
		//根据节点值与目标值大小关系决定寻找方向
		if (in_num > temp->number)
			q = temp->right;
		else if (in_num < temp->number)
			q = temp->left;
		//若相等，则返回当前节点
		else
			return temp;
	}
	return temp;
}

//插入函数
//输入值：当前AVL树的root，插入数值
//输出值：更新后树的root
nodeBB insertBB(nodeBB root, int in_num, double alfa)
{
	nodeBB temp = new BB;
	nodeBB q, p, result;
	int sizepre;
	int sizechl, sizechr;
	//若树为空，则建立该树
	if (root == NULL)
	{
		root = buildBB(in_num);
		return root;
	}
	else
	{
		//若树非空，则寻找应该插入位置的父节点
		q = findBB(root, in_num);
		temp->number = in_num;
		temp->left = NULL;
		temp->right = NULL;
		//根据大小判断插入具体位置
		if (in_num > q->number)
			q->right = temp;
		else
			q->left = temp;
		temp->pre = q;
	}
	//每次插入后，判断是否打破BB树规则
	while (q != NULL)
	{
		p = q->pre;
		//计算插入父节点的左右子树大小，并计算当前子树大小
		sizechl = sizeBB(q->left);
		sizechr = sizeBB(q->right);
		sizepre = sizechl + sizechr + 1;
		//判断是否满足BBtree条件
		if (sizepre * alfa < sizechl || sizepre * alfa < sizechr)
		{
			//若不满足，则重置该子树，详细见理论推导
			result = balanceBB(q, sizepre);
			//若已是整棵树的重置，则返回
			if (q == root)
				return  result;
			//其余情况则根据原节点分布重定义pre值
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
		//更新节点
		q = p;
	}
	return root;
}

//删除函数
//输入值：当前AVL树的root，需要删除的数值
//返回值：更新后树的root
nodeBB deletionBB(nodeBB root, int in_num, double alfa,nodeBB truroot)
{
	nodeBB q = new BB;
	nodeBB temp, p;
	int record;
	int sizepre;
	int sizechl, sizechr;
	//寻找需要删除数值的节点位置
	q = findBB(root, in_num);
	temp = q->pre;
	//若不存在该数值
	if (q->number != in_num)
	{
		//输出错误删除结果
		//printf("delete %d is a bad choose\n",in_num);
		return root;
	}
	//若为叶节点，则直接删除即可
	else if (q->left == NULL && q->right == NULL)
	{
		//根据具体位置将父节点的左/右子树置为NULL
		if (temp->left == q)
			temp->left = NULL;
		else
			temp->right = NULL;
		//每次删除后，判断是否还满足BBtree条件
		while (temp != NULL)
		{
			q = temp->pre;
			//计算插入父节点的左右子树大小，并计算当前子树大小
			sizechl = sizeBB(temp->left);
			sizechr = sizeBB(temp->right);
			sizepre = sizechl + sizechr + 1;
			//判断是否满足BBtree条件
			if (sizepre * alfa < sizechl || sizepre * alfa < sizechr)
			{
				//若不满足，则重置该子树
				p = balanceBB(temp, sizepre);
				if (temp->pre == NULL)
					return p;
				//其余情况则根据原节点分布重定义pre值
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
			//更新节点
			temp = q;
		}
	}
	//若该节点不为叶子节点，且没有左子树，则寻找右子树最小值
	else if (q->left == NULL)
	{
		p = q->right;
		while (p->left != NULL)
			p = p->left;
		record = p->number;
		//迭代执行删除操作
		p = deletionBB(p, p->number, alfa,truroot);
		q->number = record;
	}
	//若有左子树，则寻找左子树最大值
	else
	{
		p = q->left;
		while (p->right != NULL)
			p = p->right;
		record = p->number;
		//迭代执行删除操作
		p = deletionBB(p, p->number, alfa, truroot);
		q->number = record;
	}
	return root;
}

//------------------***主函数部分***-----------------
int main()
{
	//定义输入接收数组并接收输入的数据
	int* record = (int*)malloc(sizeof(int) * n);
	for (int i = 0;i < n;i++)
		scanf_s("%d", &record[i]);

	//根据需要定义各项空树（每次一棵）
	nodeAVL root = NULL;
	//nodeBST root = NULL;
	//nodeBB root = NULL;
	//nodeSPL root = NULL;

	//定义另一接收node（用于控制比较相同输入）
	nodeAVL receive = NULL;
	//nodeBB receive = NULL;
	//nodeBST receive = NULL;
	//nodeSPL receive = NULL;

	//设置BB tree 的[α]系数
	double alfa = 0.5;

	//置0不平衡次数检测变量（仅用于AVL tree与BB tree）
	outnumber = 0;
	//置0累计总树高位
	aveheight = 0;

	//定义时间记录变量
	clock_t start, end;
	//开始记录时间
	start = clock();

	//遍历数组record，开始执行插入或删除操作
	//外循环为重复次数
	for (int i = 0;i < m;i++)
	{
		//每次重置root，时间开销可忽略不计
		root = NULL;
		//内循环完成整棵的插入与删除
		for (int j = 0;j < n;j++)
		{
			//数据大于0表示插入
			if (record[j] > 0)
			{
				//以下选择相应的数据结构就可

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
			//输入小于0表示删除
			else
			{
				//若root为空，输出删除错误信息
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

	//停止时间记录
	end = clock();

	//输出运行总时间
	printf("total time = %f\n", (double)(end - start) / CLOCKS_PER_SEC);
	//输出发生不平衡现象的次数
	printf("error time = %d\n", outnumber / m);

	//为了方便，再做一轮，用于输出平衡树高（不影响时间的计算）
	root = NULL;
	for (int j = 0;j < n;j++)
	{
		if (record[j] > 0)
		{
			//以下选择相应的数据结构就可

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
		//输入小于0表示删除
		else
		{
			//若root为空，输出删除错误信息
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
		//累加插入/删除操作后总树高
		aveheight += nodehigh(root);
	}

	//定义空“类”堆并建立“类”堆
	psta* stacktree = NULL;
	stacktree = buildstack(root);
	//置0已输出node的个数
	exnumber = 0;
	//按level输出树的各个节点
	while (stacktree != NULL)
		stacktree = print(stacktree, exnumber);
	printf("\n");

	//输出插入后树的平均高度
	printf("average height = %lf\n", 1.0 * aveheight / n);

	return 0;
}