#include<iostream>
#include<string>
#include<ctime>
#include<cmath>
using namespace std;
//-------------***The story***--------------------------
// 
//注意：为表示方便，我使用right替代east方向，left代替west方向
//		即：left，right，up，down控制角色行动方向
// 
//定义room总数量，以9为例（仅9有效）
#define record 9

//全局变量定义公主是否已找到
int withprincess = 0;

//函数声明
int fiction();

//定义房间类
class room
{
private:
	//定义方向，记录顺序为左右上下
	int direction[4];
	//记录总门数
	int total;
	//记录房间性质，有普通房间、出发地、密室、怪兽屋四类
	int role;
	string dire[4];
public:
	//class初始换函数
	room(int a, int b, int c, int d, int e);
	//房间信息输出函数
	void print_room();
	//房间性质置位函数
	void set_role(int num) { role = num;}
	//房间门开放状态置位函数
	int set_door(int num);
	//输入方向处理函数
	int check_input(string receive, int i);
	//房屋事件输出函数
	int print_event(int num);
};
room::room(int a, int b, int c, int d, int e)
{
	//置位room方向信息
	direction[0] = a;
	direction[1] = b;
	direction[2] = c;
	direction[3] = d;
	//置位房间性质
	role = e;
	//统计房间可用方向个数
	total = a + b + c + d;
	//记录方向表达
	dire[0] = "left";
	dire[1] = "right";
	dire[2] = "up";
	dire[3] = "down";
}
void room::print_room()
{
	int number = total;
	int i = 0;
	//输出可选方向总数目
	cout << "There are " << total << " exits:";
	while (i < 4)
	{
		//输出每一可选方向
		if (direction[i])
		{
			cout << " " << dire[i];
			//记录未输出可选方向个数
			number--;
			//根据剩余数目调整输出格式
			if (number >= 2)
				cout << ",";
			else if (number == 1)
				cout << " and";
			else if (number == 0)
				cout << ".";
		}
		i++;
	}
	cout << endl;
	cout << "Enter your command:" << endl;
}
int room::set_door(int num)
{
	int flag = 0;
	//关闭指定门
	if (direction[num] == 1)
	{
		//若关门成功，则返回1，总有效门数-1
		total--;
		flag = 1;
	}
	direction[num] = 0;
	return flag;
}
int room::check_input(string receive, int i)
{
	int result;
	//若游戏中途输入exit，表示中途退出
	if ((int)receive.find("exit") != -1)
		return -1;
	//未输入'go'情况下，提醒用户以输入前置'go'选择行走方向
	if ((int)receive.find("go") == -1)
	{
		cout << "Incorrect input! You need to use 'go' to lead the direction." << endl;
		return i;
	}
	//依次判断用户选择方向，若方向可选，则作出相应反馈
	//若不可选，则输出错误选择语句
	result = (int)receive.find("left");
	if (result != -1 && direction[0])
		return i - 1;
	else if(result != -1 && !direction[0])
	{
		cout << "Oops, there's a wall here!" << endl;
		return i;
	}
	result = (int)receive.find("right");
	if (result != -1 && direction[1])
		return i + 1;
	else if(result != -1 && !direction[1])
	{
		cout << "Oops, there's a wall here!" << endl;
		return i;
	}
	result = (int)receive.find("up");
	if (result != -1 && direction[2])
		return i - sqrt(record);
	else if(result != -1 && !direction[2])
	{
		cout << "Oops, there's a wall here!" << endl;
		return i;
	}
	result = (int)receive.find("down");
	if (result != -1 && direction[3])
		return i + sqrt(record);
	else if(result != -1 && !direction[3])
	{
		cout << "Oops, there's a wall here!" << endl;
		return i;
	}
	//若输入中不存在方向词，则输出错误语句
	cout << "Incorrect input!" << endl;
	return i;
}
int room::print_event(int num)
{
	//若为普通room，输出普通相应
	if (role == 0)
	{
		cout << "Welcome to a room " << num << ".";
		//输出该房间信息
		print_room();
	}
	else if (role == 2)
	{
		//若回到起始点时，公主同样存在，则游戏成功
		if (withprincess == 1)
		{
			cout << "You successfully saved the princess!" << endl;
			return 1;
		}
		//若公主不存在，则为普通访问起始点
		else
		{
			cout << "Welcome to the lobby.";
			print_room();
		}
	}
	//若进入密室
	else if (role == 1)
	{
		cout << "You entered the secret room." << endl;
		//首次进入引发剧情
		if (!withprincess)
		{
			withprincess = 1;
			cout << "You met the princess!" << endl;
			//触发剧情，根据用户选择触发不同结果
			if (!fiction())
			{
				cout << "The princess was heartbroken and chose to commit suicide." << endl;
				cout << "Game over!" << endl;
				return -1;
			}
		}
		//若多次进入，则等效于访问普通房间
		print_room();
	}
	//若进入怪兽巢穴，则游戏结束
	else if (role == -1)
	{
		cout << "You met the monster!" << endl;
		cout << "Game over!" << endl;
		return -1;
	}
	return 0;
}
//剧情函数
int fiction()
{
	string str;
	//模仿剧情对话
	cout << "Princess: Is that you, my knight?" << endl;
	//输入任意键继续
	cout << "<Enter any key to continue>" << endl;
	getline(cin, str);
	cout << "You: Yes, Princess, it's me." << endl;
	cout << "Princess: Help me! Get me out of here!" << endl;
	//彩蛋内容
	cout << "<Make your choice, 'yes' or 'no'>" << endl;
	getline(cin, str);
	if ((int)str.find("no") != -1)
		return 0;
	else
	{
		cout << "You: OK,let's go!" << endl;
		return 1;
	}
}
int main()
{
	//设置随机种子
	srand(time(0));
	//定义结果变量与总房间个数
	int flag = 0, i = record - 1;
	//定义随机化接收变量
	int door, reserve = -1;
	//接受方向选择输入
	string receive;
	//随机生成密室位置
	int a = rand() % record;
	//密室位置不能与起始地重合
	while (a == record-1)
		a = rand() % record;
	//随机生成怪兽巢穴位置
	int b = rand() % record;
	//位置不能与起始地与密室重合
	while (b == a || b == record-1)
		b = rand() % record;
	//最大可能性初始化castle room
	room castle[record] = { { 0,1,0,1,0 },{ 1,1,0,1,0 },{ 1,0,0,1,0 },
							{ 0,1,1,1,0 },{ 1,1,1,1,0 },{ 1,0,1,1,0 },
							{ 0,1,1,0,0 },{ 1,1,1,0,0 },{ 1,0,1,0,2 }};
	//随机为castle中两个不同的房间关门，增加多样性
	for (int j = 0;j < 2;j++)
	{
		door = rand() % record;
		//保证两次关门不属于同一个room
		while (door == reserve && j != 0)
			door = rand() % record;
		reserve = rand() % 4;
		//可能关闭已关闭的门，增加多样性
		if (castle[door].set_door(reserve))
		{
			//若关门成功，需要去对映的房间关闭对映门
			if (reserve == 0)
				castle[door - 1].set_door(1);
			else if (reserve == 1)
				castle[door + 1].set_door(0);
			else if (reserve == 2)
				castle[door - 3].set_door(3);
			else
				castle[door + 3].set_door(2);
		}
		reserve = door;
	}
	//设置公主与怪兽位置
	castle[a].set_role(1);
	castle[b].set_role(-1);
	//开始游戏
	while (flag == 0)
	{
		//check当前房间事件
		flag = castle[i].print_event(i+1);
		//若遭遇怪兽或已就出公主，则游戏结束
		if (flag != 0)
			break;
		//整行接受输入
		getline(cin, receive);
		//根据输入改变当前位置
		i = castle[i].check_input(receive, i);
		cout << endl;
		//对于中途退出游戏的相应反馈
		if (i == -1)
		{
			cout << "Exit succeeded!" << endl;
			break;
		}
	}
	return 0;
}