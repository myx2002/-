#include<iostream>
#include<string>
#include<ctime>
#include<cmath>
using namespace std;
//-------------***The story***--------------------------
// 
//ע�⣺Ϊ��ʾ���㣬��ʹ��right���east����left����west����
//		����left��right��up��down���ƽ�ɫ�ж�����
// 
//����room����������9Ϊ������9��Ч��
#define record 9

//ȫ�ֱ������幫���Ƿ����ҵ�
int withprincess = 0;

//��������
int fiction();

//���巿����
class room
{
private:
	//���巽�򣬼�¼˳��Ϊ��������
	int direction[4];
	//��¼������
	int total;
	//��¼�������ʣ�����ͨ���䡢�����ء����ҡ�����������
	int role;
	string dire[4];
public:
	//class��ʼ������
	room(int a, int b, int c, int d, int e);
	//������Ϣ�������
	void print_room();
	//����������λ����
	void set_role(int num) { role = num;}
	//�����ſ���״̬��λ����
	int set_door(int num);
	//���뷽������
	int check_input(string receive, int i);
	//�����¼��������
	int print_event(int num);
};
room::room(int a, int b, int c, int d, int e)
{
	//��λroom������Ϣ
	direction[0] = a;
	direction[1] = b;
	direction[2] = c;
	direction[3] = d;
	//��λ��������
	role = e;
	//ͳ�Ʒ�����÷������
	total = a + b + c + d;
	//��¼������
	dire[0] = "left";
	dire[1] = "right";
	dire[2] = "up";
	dire[3] = "down";
}
void room::print_room()
{
	int number = total;
	int i = 0;
	//�����ѡ��������Ŀ
	cout << "There are " << total << " exits:";
	while (i < 4)
	{
		//���ÿһ��ѡ����
		if (direction[i])
		{
			cout << " " << dire[i];
			//��¼δ�����ѡ�������
			number--;
			//����ʣ����Ŀ���������ʽ
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
	//�ر�ָ����
	if (direction[num] == 1)
	{
		//�����ųɹ����򷵻�1������Ч����-1
		total--;
		flag = 1;
	}
	direction[num] = 0;
	return flag;
}
int room::check_input(string receive, int i)
{
	int result;
	//����Ϸ��;����exit����ʾ��;�˳�
	if ((int)receive.find("exit") != -1)
		return -1;
	//δ����'go'����£������û�������ǰ��'go'ѡ�����߷���
	if ((int)receive.find("go") == -1)
	{
		cout << "Incorrect input! You need to use 'go' to lead the direction." << endl;
		return i;
	}
	//�����ж��û�ѡ�����������ѡ����������Ӧ����
	//������ѡ�����������ѡ�����
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
	//�������в����ڷ���ʣ�������������
	cout << "Incorrect input!" << endl;
	return i;
}
int room::print_event(int num)
{
	//��Ϊ��ͨroom�������ͨ��Ӧ
	if (role == 0)
	{
		cout << "Welcome to a room " << num << ".";
		//����÷�����Ϣ
		print_room();
	}
	else if (role == 2)
	{
		//���ص���ʼ��ʱ������ͬ�����ڣ�����Ϸ�ɹ�
		if (withprincess == 1)
		{
			cout << "You successfully saved the princess!" << endl;
			return 1;
		}
		//�����������ڣ���Ϊ��ͨ������ʼ��
		else
		{
			cout << "Welcome to the lobby.";
			print_room();
		}
	}
	//����������
	else if (role == 1)
	{
		cout << "You entered the secret room." << endl;
		//�״ν�����������
		if (!withprincess)
		{
			withprincess = 1;
			cout << "You met the princess!" << endl;
			//�������飬�����û�ѡ�񴥷���ͬ���
			if (!fiction())
			{
				cout << "The princess was heartbroken and chose to commit suicide." << endl;
				cout << "Game over!" << endl;
				return -1;
			}
		}
		//����ν��룬���Ч�ڷ�����ͨ����
		print_room();
	}
	//��������޳�Ѩ������Ϸ����
	else if (role == -1)
	{
		cout << "You met the monster!" << endl;
		cout << "Game over!" << endl;
		return -1;
	}
	return 0;
}
//���麯��
int fiction()
{
	string str;
	//ģ�¾���Ի�
	cout << "Princess: Is that you, my knight?" << endl;
	//�������������
	cout << "<Enter any key to continue>" << endl;
	getline(cin, str);
	cout << "You: Yes, Princess, it's me." << endl;
	cout << "Princess: Help me! Get me out of here!" << endl;
	//�ʵ�����
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
	//�����������
	srand(time(0));
	//�������������ܷ������
	int flag = 0, i = record - 1;
	//������������ձ���
	int door, reserve = -1;
	//���ܷ���ѡ������
	string receive;
	//�����������λ��
	int a = rand() % record;
	//����λ�ò�������ʼ���غ�
	while (a == record-1)
		a = rand() % record;
	//������ɹ��޳�Ѩλ��
	int b = rand() % record;
	//λ�ò�������ʼ���������غ�
	while (b == a || b == record-1)
		b = rand() % record;
	//�������Գ�ʼ��castle room
	room castle[record] = { { 0,1,0,1,0 },{ 1,1,0,1,0 },{ 1,0,0,1,0 },
							{ 0,1,1,1,0 },{ 1,1,1,1,0 },{ 1,0,1,1,0 },
							{ 0,1,1,0,0 },{ 1,1,1,0,0 },{ 1,0,1,0,2 }};
	//���Ϊcastle��������ͬ�ķ�����ţ����Ӷ�����
	for (int j = 0;j < 2;j++)
	{
		door = rand() % record;
		//��֤���ι��Ų�����ͬһ��room
		while (door == reserve && j != 0)
			door = rand() % record;
		reserve = rand() % 4;
		//���ܹر��ѹرյ��ţ����Ӷ�����
		if (castle[door].set_door(reserve))
		{
			//�����ųɹ�����Ҫȥ��ӳ�ķ���رն�ӳ��
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
	//���ù��������λ��
	castle[a].set_role(1);
	castle[b].set_role(-1);
	//��ʼ��Ϸ
	while (flag == 0)
	{
		//check��ǰ�����¼�
		flag = castle[i].print_event(i+1);
		//���������޻��Ѿͳ�����������Ϸ����
		if (flag != 0)
			break;
		//���н�������
		getline(cin, receive);
		//��������ı䵱ǰλ��
		i = castle[i].check_input(receive, i);
		cout << endl;
		//������;�˳���Ϸ����Ӧ����
		if (i == -1)
		{
			cout << "Exit succeeded!" << endl;
			break;
		}
	}
	return 0;
}