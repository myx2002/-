# The report of project 1：Comparing Different Binary Search Trees 

Summary: In this experiment, I realized the structure of BST, AVL, splay tree and BB[α] tree；and also produced some test data in quantities ranging from 10 to 500 . The test data includes three categories: sequential input, out-of-order input, out-of-order input and deletion; on this basis, the advantages, disadvantages, and performance conditions of these data structures are compared using the runtime under the same input and the average tree height feature attribute differences after the insert/delete operation.



chapter 1 introduction: 

Data structure is a collection of data elements with structural characteristics. It studies the logical structure of data, the physical structure of data, and the interrelationships between them. It defines appropriate operations for this structure, designs corresponding algorithms, and ensures that the new structure obtained after these operations remains the original structure type. In short, a data structure is a collection of data elements that have one or more specific relationships with each other, that is, a collection of data elements with a "structure".

In our basic and advanced data structure courses, we have learned a large number of data structures related to trees, such as binary trees, balanced binary trees, red-black trees, and so on, each with its own bias and advantages and disadvantages; Therefore, sometimes we need to implement these structures through some practical experiments to gain a deeper understanding of these data structures. In this experiment, based on the implementation of BST, AVL, splay tree, and BB tree, I explored their runtime and average tree height characteristics after insert/delete operations to analyze their performance differences.



chapter 2 analysis of BB tree,based question 17-3:

I will expand my understanding and analysis of the BB tree based on questions 17-3.

BB tree is an ordinary binary search tree augmented by adding to each node x the attribute x.size giving the number of keys stored in the subtree rooted at x. let a be a constant in the range 1/2 <= a <1. we say that a given node x is a-balanced if x.left.size<= a * x.size and x.right.size <= a * x.size. the tree as a whole is a-balanced if every node in the tree is a-balanced .

first we assume that a = 1/2, which means the tree will as balanced as it can be ,and given a node x in an arbitrary binary search tree ,we can design an algorithm to rebuild the subtree rooted at x so that it becomes 1/2-balanced which will run in θ（x.size), and it only use O(x.size) auxiliary storage.

solution: first we can record the subtree in an in-order sequence ,this will cost O(x.size) auxiliary storage, and we can iteratively record the process of transforming it into a middle order sorting sequence, the time cost is equivalent to traversing the entire tree once, which will take O (x.size) time; Next, we use an iterative function to recreate the middle order sorting sequence into a BB tree, The iterative relationship function is T (n)=2 * T (n/2)+1, and the time cost is still O (n). Therefore, the BB tree is reconstructed to balance its time and space cost proof.



```伪代码
rebuild(the root of BB[α] tree)
{
	Define array variables;
	//output the tree in in-order, use the array to record it
	inorder(the root of BB[α] tree,array)
	Create a new root node；
	//Building a Tree Based on in-Order Sequences
	newTree(the root of BB[α] tree,array,the start point, the end point);
	//record the imbalanced number
	imbalanced number++;
	return the new root of BB[α]；
}

```

```伪代码
newTree(the root of BB[α] tree,array,the start point, the end point)
{
	if(the start point > the end point)
		return NULL;
	else if(the start point = the end point)
		build the only newnode,and return it;
	else
	{
		build the new root;
		//Recursively establishing left and right subtrees
		newTree(root->left,array,start,(end+start+1)/2-1);
		newTree(root->right,array,(end+start+1)/2+1,end);
		Connect the new node and root;
		return root;
	}
}
```



second we should find that performing a search in an n-node a-balanced binary search tree takes O（log n) as the worst-case time.

because the a is >=1/2 therefore after every comparison we can exclude (1-a) * n elements,Assuming that there is a tree height function D, we can obtain the equation D (n)=max {D (an), D (1-a) n)}+1. In order to consider the worst case and make the tree as high as possible, we assume D (an)>D (1-a) n) (because a>1/2, the more the number of an accounts for, the higher the total height of the tree). At this time, by solving the above equation, we can obtain h=log1/a n<log n, and we can prove that the worst time consumption for searching is O (logn).

假设有树高函数D，则我们可以得到等式D（n）=max{D（an），D（（1-a）n）}+1，为了考虑最坏情况，使树尽可能高，我们假定D（an）>D（（1-a）n）(因为a>1/2，所以往往an占的个数越多，会使得树总高更高），此时，解出上述等式，可得h=log1/a n<log n从而我们可以得证，查找的最坏时间消耗为O（logn）.

from now on，we assume that a is strictly greater than 1/2,suppose that we implement insert and deletion as usual for an n-node binary search tree,except that after every such operation ,if any node in the tree is no longer a-balanced, then we rebuild the subtree rooted at the highest such node in the tree so that it becomes 1/2-balanced.

and we also analyze this rebuilding scheme using the potential method ,for a node x in a binary search tree t ,we define △x=|x.left.size-x.right.size|and we define the potential of T as φ（t)=c*sum(△x) where the c is a sufficiently large constant that depends on a.

Because in the definition, △ x has an absolute value, and therefore the value of △ x itself is non negative, potential must also be non negative for binary trees;

For a 1/2-balanced tree, we can prove that the total number of left subtrees and right subtrees can differ by up to 1. According to the definition of the potential function, only nodes with a cumulative difference of 2 or more are counted, so the sum of the potential functions of the 1/2-balanced tree must be 0. The following is the proof process：

Assuming that the left subtree is n nodes and the right subtree is n-node, the total number of nodes is 2n-k+1, because the requirement of 1/2-balanced must be met, so that 2 (n-k)<=2n-k+1, and 2n<=2n-k+1, can be obtained consecutively, so that k<=1 can be obtained, and the maximum difference between the total number of left and right subtrees is 1.

因为在定义中，△x是带有绝对值的，因而△x本身的值是非负的，因此对于二分树而言，potential 也一定是非负的；

而对于1/2-平衡树而言，我们可以证明其左子树和右子树的总个数最多相差1，根据定义势函数只统计累加差值大于等于2的node，因此1/2-平衡树的势函数之和必定为0，下面便是证明过程：

假定左子树为n个node，右子树为n-knode，则总node个数为2n-k+1，因为要满足1/2-balanced的要求，从而2(n-k)<=2n-k+1,且2n<=2n-k+1,连立可以获得，从而可以得到k<=1，因而左右子树总数量最多相差1得证。

suppose that m units of potential can pay for rebuilding an m-mode subtree. Then we need to explore how large c is to reconstruct a BB tree, requiring only O (1)'s optimized time.

We can consider the optimal situation to calculate the minimum value of c, assuming that the tree is an a-balanced tree, and the left subtree is larger than the right subtree. The left subtree has broken our balance setting. At this time, x.left. size>a * x.size, and a * x.size=1/2 * x.size+(a-1/2) * x.size. As can be known from the previous proof, when a=1/2, the potential function is 0, which means that subtrees containing 1/2 * x.size may not generate any potential function values, Therefore, we assume that in the above equation, 1/2 * x.size does not generate any potential function values (which will inevitably occur in practice), and all potential function values are generated by the latter term. This transformation direction is greater than or equal to, consistent with the direction of the original equation, so it can be transformed; Because we assume that m units of potential can amortize the reconstruction costs of m nodes, c * (a-1/2) * x.size>=x.size can be used to obtain the next term of c, c>=1/(a-1/2).

Assuming that our tree is initially balanced, it can be seen from the previous proof that the worst-case time consumption of our search will not exceed O (logn), so during each insert or delete operation, the search addition operation itself only needs to spend O (logn) time; However, at this point, we need to consider the situation where the BB tree becomes unbalanced, so we need to spend a certain amount of time rebuilding the tree. As shown above, the amortization time for each reconstruction is only O (1), so we only need to consider the number of times to rebuild after each imbalance. According to analysis, imbalances that occur at the parent node can only be transmitted upward, not downward. Therefore, the case where the number of reconstructions occurs most often is only on the order of O (log n); Therefore, the amortization time consumption for each insert/delete operation is O (log n).

则我们需要探究，究竟 c为多大时，可以使重建一棵BB树，只需要O（1）的amortized time 

我们可以考虑最优情况来计算c的最小值，假定该树为a-平衡树，且左子树大于右子树，左子树已破坏了我们的平衡设定，此时x.left.size>a*x.size，而a*x.size=1/2*x.size+(a-1/2)*x.size，有前面的证明可知，当a=1/2时，势函数为0，这也就意味着包含1/2*x.size个数的子树可能不产生任何势函数数值，因此我们假定上述等式中，1/2*x.size不产生任何的势函数数值（实际必然会有产生），所有势函数的数值均由后一项产生，此次变换方向为大于等于，与原等式方向一致，因而可以变换；因为我们假定m units of potential 可以摊还 m node的重建开销，所以c*(a-1/2)*x.size>=x.size,从而可以得到c的下届，c>=1/(a-1/2).

假定我们的树一开始是平衡的，由前证明可得，我们搜索一次的最坏情况时间消耗也不会超过O（logn），因而在每次插入或删除操作时，查找加操作本身只需要花费O（logn）的时间；但这时，我们要考虑BB树变得不平衡的情况，因而我们要花费一定的时间对树进行重建，而由上证明可知，每次重建的摊还时间只有O（1），因而我们只要考虑每次不平衡后重建的次数即可。有分析可知，发生在父节点的不平衡只会向上传递，而不会向下传递，因而重建次数发生最多的情况，也只有O（logn）量级；因而，每次插入/删除操作的摊还时间消耗为O（logn）.

chapter 3 comparison of these data structures: For detailed analysis, see Chapter 5

建立表格，若干行（主标题：测试数据，运行时间，操作后平均高度，转换次数（AVL/BB）四列

chapter 4 detailed description of your experiments(question and solution):

First of all, except for BB trees, the design of these tree data structures depends more or less on my current learning accumulation. However, to achieve these trees, I may not necessarily be perfectly reproducing them according to the true standard. During the design process, I also considered universality; Although my understanding of C++template classes is not deep enough, I have unified the four data structures without affecting the implementation of the data structures as much as possible, so that my code can be reused when performing some common operations (although in actual code writing, I still use ctrl C+ctrl V). However, this has increased versatility to some extent, such as adding pre, a pointer to a parent node, to BST, This is minimal in terms of elapsed time consumption, but when performing some design operations, the same design thinking can often be used, which can bring a lot of convenience.

首先，除BB树以外，这些树数据结构的设计或多或少依赖于自己当下的学习积累，但实现这些树，我也未必是按照真正的标椎完美复刻，在设计的过程中，我也考虑了通用性；虽然我对于C++模板类的理解还不足够深，但我在尽可能不影响数据结构实现的基础上，使这四个数据结构归于统一，这样在进行一些共性的操作时，我的代码可以复用（虽然在实际代码编写时，我还是ctrl C+ctrl V了），但这在一定程度山增加了通用性，如我为BST也添加了pre，即指向父节点的指针，这对于运行时间的消耗而言是微乎其微的，但在进行一些设计操作时，往往可以使用同样的设计思维，因而可以带来不少便捷。

As for the construction of test data, I want to discuss it in two parts: one is structural test data, the other is performance test data; Structural test data refers to test data that verifies the correctness of structural design (code operation), such as NULL input, single input, unilateral biased input, balanced input, and other strange or boundary input conditions. Test data is often an uncommon type in practical applications, but plays a very important role in the correctness of structural design, often considering a small amount of boundary data; Performance test data is a large amount of data that is purposefully set or randomly generated. In this experiment, I considered three types of operations: sequential insertion, random insertion, and random insertion and deletion to simulate extreme input situations, general insertion situations, and general real-world usage situations, and then compare the differences between different tree data structures.

而对于测试数据的构造，我想分为两部分来谈，一部分是结构测试数据，一部分是性能测试数据；结构测试数据是指验证结构设计（代码运行）正确性的测试数据，如NULL输入、单个输入、单侧偏向输入、平衡输入等各种奇怪，或者说边界输入条件的测试，测试数据往往是实际运用中不常见的类型，但对于结构设计的正确性却有着很重要的作用，往往是考虑边界的少量数据；而性能测试数据，则是有目的性设置或者随机生成的大量数据，在本实验中，我考虑了顺序插入，随机插入，随机插入删除等三类操作，用以模仿极端输入情况，一般插入情况及一般现实使用情况，进而来比较不同树数据结构间的差异。

chapter 5 the result analysis:

通过比较时间，操作后平均次数来比较不同树之间的实现性能差异（基于一些树的特性）

顺序输入和乱序输入的比较



结合陈列的表格数据与全部的测试数据，我们可以看到，在较小输入数据量的情况下，BST的运行时间消耗往往占据相对的优势，但其平均树高明显偏高的劣势也十分明显，特别是在顺序输入的情况下，他的平均树高高达O(n)量级，这对于相连的insertion操作十分不友善，但对于一般的随机输入而言，其平均树高也只在其他数据结构的两倍左右，属于O(log n)的量级，因而可以接受。而对于AVL树而言，因为有相应的调节手段，依赖额外开销来平衡树高，因而该数据结构的平均树高为此四项的最小值，且其可以很好的处理顺序插入的边界情况，在一定数据量的顺序插入中，运行速度甚至可以优于BST，但对于一般的插入删除操作，因为平衡旋转的存在，其运行时间往往几倍于BST等其他数据结构，且不平衡发生的次数，随着数据规模的扩大急速上升（特别是顺序插入的情况下），这也可以说是为平衡树高而付出的时间代价。

接下来是splay tree的分析，虽然在每次插入后，我们都要遍历一遍路径，使插入的节点“冒泡”到root上，这样的操作看似会花费大量的时间，但在实际的运行中，这却很好的剪短了总时长的运行消耗，在数据集合中我们可以看到，虽然对于顺序输入的边界情况，splay tree有着很高的平均树高，但是它的运行时间，还是处于快速的状态，因为每次插入后查找的特性，使树高不会影响其顺序插入的速度；而对于一般的随机输入而言，splay tree则表现出相对较高的性能，但总体而言，其运行时间和插入删除操作后树高平均值还是略逊于BST等数据结构。

最后是崭新的数据类型，BB[α]树，总体比较而言，BB[α]树没有明显的优势，甚至在每个方面都有不小的弊端（可能是我的实现有一丢丢问题）；较优的一点是，基于重构操作的BB[α]树，有较低的平均树高优势，因为重构的“绝对”平衡性，使得在α = 0.5时，其插入操作后的平均树高甚至低于AVL tree；但是，较低的平均高度并不能很好的分担重构的时间消耗，因而在数据测试时，BB[α]运行时间的消耗往往是其他数据结构的数倍，且对于BB[α]树本身而言，相同数据量下，随机插入的时间性能远高于顺序插入，这也愈加说明BB[α]的大量时间开销往往来源于重构的进行。

chapter 6 conclusion:

优缺点总结，运行时间还是树高有优势，每棵树都适合与什么

总体而言，我认为splay tree 是上述四种数据结构中最优的一种；首先对于BB树而言，它为了平衡的树高，开销了太多的时间，以至于我认为得不偿失，另外上述讨论只是基于a=0.5的情况，对于更大的a而言，其平均树高的优势也会消失；而比较与AVL树来说，虽然splay tree有着平均树高的劣势，但是每次插入后的查找操作往往会使splay tree的结构变得简单，因而有着更好的时间性能，在一般的实际情况下，我们可以接受适当的树高复杂度，来换取大幅提升的时间性能；最后是与BST的比较，虽然两者在时间及平均树高的考量上几乎平分秋色，但是BST对于边界情况的处理却不显得那么乐观，而splay tree对极端情况的输入，也有着较好的处理性能；综上所述，splay tree，真的太厉害了。

