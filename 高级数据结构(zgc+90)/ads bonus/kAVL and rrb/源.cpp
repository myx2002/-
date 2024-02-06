#include<iostream>
#include<algorithm>
#include<time.h>

//---***基础变量说明***---
//定义总输入量
#define n 271
//定义重复次数
#define m 1000

//记录非平衡次数
int outnumber;
//记录平均高度
int aveheight;
//记录k的数值
int k = 2;

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
//kAVL
//定义kAVL tree数据结构，并进行重命名
typedef struct treekAVL kAVL;
typedef kAVL* nodekAVL;
//包含数据记录，左右子树及父节点
struct treekAVL {
	int number;
	nodekAVL left;
	nodekAVL right;
	nodekAVL pre;
};

//---***非完全通用函数定义***--------
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
	nodekAVL notree;
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
	nodekAVL next = head->notree;
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

//---***kAVL***---
//kAVL初始化函数
//输入值：需要插入/初始化的数值
//返回值：初始化的节点指针
nodekAVL buildkAVL(int in_num)
{
	nodekAVL root = new kAVL;
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
nodekAVL findkAVL(nodekAVL root, int in_num)
{
	nodekAVL temp = root;
	nodekAVL q = root;
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
//输入值：三个连续的kAVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void kAVLLL(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
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
//输入值：三个连续的kAVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void kAVLRR(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
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
//输入值：三个连续的kAVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void kAVLLR(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
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
//输入值：三个连续的kAVL节点指针，分别为当前子节点，父节点，祖父节点；根节点
//返回值：无
void kAVLRL(nodekAVL p, nodekAVL q, nodekAVL r, nodekAVL root)
{
	nodekAVL l;
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
//输入值：当前kAVL树的root，插入数值
//输出值：更新后树的root
nodekAVL insertkAVL(nodekAVL root, int in_num)
{
	nodekAVL temp = new kAVL;
	nodekAVL q, p;
	int result;
	//若树为空，则建立该树
	if (root == NULL)
		root = buildkAVL(in_num);
	//若树非空，则寻找应该插入位置的父节点
	else
	{
		q = findkAVL(root, in_num);
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
			if (result > k && p->left == q && q->left == temp)
			{
				kAVLLL(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = q;
				break;
			}
			//若符合左右旋转条件，则实行
			else if (result > k && p->left == q && q->right == temp)
			{
				kAVLLR(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = temp;
				break;
			}
			//若符合右左旋转条件，则实行
			else if (result > k && p->right == q && q->left == temp)
			{
				kAVLRL(temp, q, p, root);
				//若已为根节点，则跳出循环
				if (p == root)
					root = temp;
				break;
			}
			//若符合右右旋转条件，则实行
			else if (result > k && p->right == q && q->right == temp)
			{
				kAVLRR(temp, q, p, root);
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
nodekAVL kAVLsl(nodekAVL p, nodekAVL q)
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
nodekAVL kAVLsr(nodekAVL p, nodekAVL q)
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
nodekAVL kAVLdl(nodekAVL p, nodekAVL q, nodekAVL r)
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
nodekAVL kAVLdr(nodekAVL p, nodekAVL q, nodekAVL r)
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
nodekAVL deletionkAVL(nodekAVL root, int in_num)
{
	nodekAVL q = new kAVL;
	nodekAVL temp, p;
	int record, result, check;
	//寻找需要删除数值的节点位置
	q = findkAVL(root, in_num);
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
			if (result <= k);
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
					if (check < 0)
						//进行单次左旋
						temp = kAVLsl(temp, p);
					else
						//进行双次右旋
						temp = kAVLdl(temp, p, p->left);
				}
				//左子节点情况
				else
				{
					//比较另一支子树的高度差值
					check = nodehigh(p->right) - nodehigh(p->left);
					//左子树大于右子树（基于子树的子树比较）
					if (check < 0)
						//进行单次右旋
						temp = kAVLsr(temp, p);
					else
						//进行双次左旋
						temp = kAVLdr(temp, p, p->right);
				}
				if (temp->pre == NULL);
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
		p = deletionkAVL(p, p->number);
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
		p = deletionkAVL(p, p->number);
		q->number = record;
	}
	return root;
}

int main()
{
	//定义输入接收数组并接收输入的数据
	int* record = (int*)malloc(sizeof(int) * n);
	for (int i = 0;i < n;i++)
		scanf_s("%d", &record[i]);

	//根据需要定义各项空树（每次一棵）
	nodekAVL root = NULL;

	//定义另一接收node（用于控制比较相同输入）
	nodekAVL receive = NULL;

	//置0不平衡次数检测变量
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
				//kAVLtree
				receive = findkAVL(root, record[j]);
				if (receive == NULL)
					root = insertkAVL(root, record[j]);
				else if (receive->number == record[j])
					printf("%d already exist!\n", receive->number);
				else
					root = insertkAVL(root, record[j]);
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
				root = deletionkAVL(root, -record[j]);
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
			//kAVLtree
			receive = findkAVL(root, record[j]);
			if (receive == NULL)
				root = insertkAVL(root, record[j]);
			else if (receive->number == record[j])
				printf("%d already exist!\n", receive->number);
			else
				root = insertkAVL(root, record[j]);
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
			root = deletionkAVL(root, -record[j]);
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
	printf("average height = %lf\n", 1.0 * aveheight / n );

	return 0;
}

