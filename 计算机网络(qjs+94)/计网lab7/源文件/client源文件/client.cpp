#include<winsock.h>
//#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<thread>
#include<queue>
#include<map>
#include<string>

//定义消息接受结构
//数据包为size - type，size - value
//类型记录为type - value
struct message
{
	//一定要用map，记录属性名与属性值的对映
	std::map<std::string, std::string> msg_map;
};

//客户端client结构体
struct client
{
	SOCKET client_socket;
	//子线程
	std::thread* cli_thread;
	int con_flag;
	//消息队列
	std::queue<message> mess_que;
};


int initial_client_socket(struct client* user);
int client_connect(char* ip, int port, struct client* user);
int send_message(std::string &str, SOCKET cli_socket);
void show_content();
void threadFunction(SOCKET* cli_socket, std::queue<message>* mess_que, int* con_flag);
void dis_open(message* mess, char* buffer, int num);
std::string di_close(message* msg);
std::string message_solve(message* msg);

int main()
{
	//暂且认为库函数的使用不存在问题
	std::cout << "Hello" << std::endl;
	//check winsock
	WSADATA wsaData;
	if (!(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0))
	{
		std::cout << "Failed to initialize Winsock." << std::endl;
		system("pause");
		return 0;
	}

	//选择操作信号
	int op_num = -1;
	//记录列表是否查看信号
	int list_num = 0;
	//记录message有没有发送成功
	int sen_num;
	//信息接受等待信号
	int while_wait;

	//分配套接字空间
	struct client* my_client = new client;
	if (my_client == NULL)
	{
		system("pause");
		return 0;
	}
		
	//是否连接标志
	my_client->con_flag = 0;
	//初始化socket
	int ini_flag = initial_client_socket(my_client);
	//根据返回值判断正确性
	if (ini_flag == 1)
	{
		std::cout << "Create socket failed!" << std::endl;
		system("pause");
		return 0;
	}

	//总体大循环
	while (1)
	{
		show_content();
		std::cin >> op_num;
		sen_num = -1;
		while_wait = 0;
		if (my_client->con_flag == 0)
		{
			if (op_num != 1 && op_num != 7)
				printf("Not connected!\n");
			else if (op_num == 1)
			{
				//获得需要连接的服务器IP地址与端口信息
				char* IP = (char*)malloc(sizeof(char) * 32);
				char ip_get[32] = "127.0.0.1";
				int port = 4530;
				IP = ip_get;
				//printf("Please input the connect IP address:\n");
				//std::cin >> IP;
				//printf("Please input the connect port:\n");
				//std::cin >> port;
				
				//进行连接操作
				printf("Connecting\n");
				int con_check = client_connect(IP, port, my_client);
				if (con_check == 0)
				{
					//输出成功连接信息
					printf("Successfully connected!\n");
				}
				else if (con_check == 1)
				{
					//输出错误信息
					printf("Failed to connect to the server\n");
					printf("Quit\n");
					system("pause");
					return 0;
				}
			}
			else if (op_num == 7)
			{
				//直接退出程序
				printf("Good Bye!\n");
				system("pause");
				return 0;
			}
		}
		else if (my_client->con_flag == 1)
		{
			if (op_num == 1)
			{
				//每个客户端只有一个主线程连接
				printf("Have been connected!\n");
			}
			else if (op_num == 2)
			{
				//断开连接
				closesocket(my_client->client_socket);
				my_client->con_flag = 0;
				printf("Connect down!\n");
				//设置新的socket，以兼容下次连接
				initial_client_socket(my_client);
				my_client->con_flag = 0;
				list_num = 0;
			}
			else if (op_num == 3)
			{
				
				//Sleep(350);
				//合成时间请求message
				message msg;
				msg.msg_map["type"] = "timerequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				//for (int i = 0;i < 100;i++)
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
			else if (op_num == 4)
			{
				//合成name请求message
				message msg;
				msg.msg_map["type"] = "namerequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
			else if (op_num == 5)
			{
				//合成服务器列表请求message
				message msg;
				msg.msg_map["type"] = "listrequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				list_num = 1;
				printf("Waiting for the reply!\n");
			}
			else if (op_num == 6 && list_num == 1)
			{
				//发送需要发送的消息
				message msg;
				msg.msg_map["type"] = "sendrequest";
				//获取相应的客户端列表序号
				printf("Please enter the process ID(Port):\n");
				std::string ID;
				std::cin >> ID;
				//获取对应内容
				printf("Please enter the message:\n");
				std::string mess_text;
				std::cin >> mess_text;
				//将相应的信息放入msg的map中
				msg.msg_map["ID"] = ID;
				msg.msg_map["message"] = mess_text;
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
			else if (op_num == 6 && list_num == 0)
			{
				printf("Please get the process list(NO.5) first!\n");
			}
			else if (op_num == 7)
			{
				//先断开连接，关闭套接字
				closesocket(my_client->client_socket);
				my_client->con_flag = 0;
				//再关闭程序
				printf("Good Bye!\n");
				system("pause");
				return 0;
			}
			else if (op_num == 8)
			{
				//合成关闭服务器请求message
				message msg;
				msg.msg_map["type"] = "shutdownrequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
		}
		//判断是否发送成功
		if (sen_num == 1)
			printf("Send message failed!\n");
		else if (sen_num == 0)
			printf("Send message successfully!\n");
		
		//wait server的第一次操作（一般需要花费较长的时间）
		Sleep(200);
		//等到接收到server的数据包再关闭相应的套接字
		if (op_num == 8)
		{
			//断开连接，关闭套接字,告知服务器关闭信号
			closesocket(my_client->client_socket);
			my_client->con_flag = 0;
			list_num = 0;

			//设置新的socket并connect，以触发服务器的关闭步骤
			initial_client_socket(my_client);
			char IP[32] = "127.0.0.1";
			int port = 4530;
			client_connect(IP, port, my_client);

			//因为服务器关闭，需要断开客户端的连接接口
			closesocket(my_client->client_socket);
		}
		while_wait = 1;
		//小循环，若客户端存有消息队列，则要等消息队列处理完成后再开始下一个操作的问询
		while (while_wait == 1)
		{
			//一般而言只需要处理一个replay即可
			while (!my_client->mess_que.empty())
			{
				message msg;
				//获得map中
				msg = my_client->mess_que.front();
				my_client->mess_que.pop();
				//解析函数
				std::string re_str = message_solve(&msg);
				std::cout << re_str << std::endl;
			}
			while_wait = 0;
		}
		Sleep(200);
		//关闭服务器且接收到消息后，直接退出
		if (op_num == 8)
		{
			printf("Server being done!\n");
			system("pause");
			return 0;
		}
	}
	system("pause");
	return 0;
}
//展示选择操作信息
void show_content()
{
	printf("Please select an action(use the number):\n");
	printf("1.Connect\n");
	printf("2.Disconnect\n");
	printf("3.Get time\n");
	printf("4.Get name\n");
	printf("5.Get server process list\n");
	printf("6.Send message\n");
	printf("7.Quit\n");
	printf("8.Shutdown server\n");
}

//初始化client_socket
//直接将socket赋值于结构体中
//正常create返回0，不正常返回1
int initial_client_socket(struct client* user)
{
	//使用ipv4 + TCP，自动选择合适的协议
	user->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//分配无效
	if (user->client_socket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		//清理Windows Sockets库所分配的资源
		WSACleanup();
		//返回分配错误的信息
		return 1;
	}
	return 0;
}

//介导客户端与服务器连接函数
//若成功连接，则返回0
//若连接失败，则关闭套接字，并返回1
int client_connect(char* ip, int port, struct client* user)
{
	//设置服务器地址信息
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	//使用ipv4协议
	server_addr.sin_family = AF_INET;
	//设置服务器端口
	//使用htons函数将其转换为网络字节序对映的端口号
	server_addr.sin_port = htons(port);
	//设置对映的服务器ip地址
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	//设置完服务器地址信息之后，使用connect函数进行连接操作
	//判断connect连接操作是否成功
	if (connect(user->client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		//关闭套接字并释放分配的资源
		closesocket(user->client_socket);
		WSACleanup();
		return 1;
	}
	else
	{
		//建立子线程
		user->con_flag = 1;
		//将客户端的message列表、socket与con_flag连接信息传入子线程中
		user->cli_thread = new std::thread(threadFunction, &user->client_socket, &user->mess_que, &user->con_flag);
		return 0;
	}
	return 0;
}

//子线程实现函数
void threadFunction(SOCKET* cli_socket, std::queue<message>* mess_que, int* con_flag)
{
	//暂时设置buffer为1024字节
	char buffer[1024];
	//使用rec_num获得receive函数返回的消息字节数
	int rec_num;
	//只要连接处于on状态，就要一直接收服务器信息
	while (*con_flag)
	{
		//先将buffer清空
		memset(buffer, 0, sizeof(buffer));
		rec_num = recv(*cli_socket, buffer, sizeof(buffer) - 1, 0);
		if (rec_num <= 0 || rec_num >= 1023)
		{
			//若接收到的返回值已为0，说明连接已断开
			printf("The connection has done!\n");
			*con_flag = 0;
			return;
		}
		//printf("Get message successfully!\n");
		//添加消息结束符
		buffer[rec_num] = '\0';
		message msg;
		//解析buffer，将数据读出至message中
		dis_open(&msg, buffer, rec_num);
		//printf("Disopen successfully!\n");
		//将message信息放入client结构message队列中
		(*mess_que).push(msg);
	}
	return;
}

//解析buffer获得的信息
void dis_open(message* msg, char* buffer, int num)
{
	msg->msg_map.clear();
	//记录buffer的当前访问位置
	int index = 0;
	while (index < num)
	{
		//获取type的字节数
		unsigned long type_num = (unsigned long)buffer[index];
		index++;
		//获取type类型的具体字符串
		std::string type(buffer + index, type_num);
		index = index + type_num;
		//获取value的字节数
		unsigned long value_num = (unsigned long)buffer[index];
		index++;
		//获取value的具体信息
		std::string value(buffer + index,  value_num);
		index = index + value_num;
		msg->msg_map[type] = value;
	}
}

//send message信息函数
//传递应用参数
//正常发送返回 0
//发送失败返回 1
int send_message(std::string &str, SOCKET cli_socket)
{
	int send_num = send(cli_socket, str.c_str(), str.size(), 0);
	if (send_num < 0)
	{
		return 1;
	}
	return 0;
}

//将message中记录的map转换为string
std::string di_close(message* msg)
{
	std::string str;
	for (auto item = (*msg).msg_map.begin(); item != (*msg).msg_map.end(); item++) 
	{
		//先载入类型的type字节数
		str += (char)item->first.size();
		//载入type类型
		str += item->first;
		//再载入对映的value字节数与信息
		str += (char)item->second.size();
		str += item->second;
	}
	return str;
}

//解析获得的msg消息
int time_num = 0;
std::string message_solve(message* msg)
{
	//记录response类型
	std::string rece_data;
	//记录response对映的数据
	std::string retu_data;
	rece_data = msg->msg_map["type"];
	//判断类型
	if (rece_data == "timeresponse")
	{
		//输出返回时间结果
		//time_num++;
		//retu_data = std::to_string(time_num) + " Response: the time is " + msg->msg_map["time"] + " !\n";
		retu_data = " Response: the time is " + msg->msg_map["time"] + " !\n";
	}
	else if (rece_data == "nameresponse")
	{
		//输出返回姓名结果
		retu_data = "Response: the name is " + msg->msg_map["name"] + " !\n";
	}
	else if (rece_data == "sendresponse")
	{
		//输出发送信息结果
		retu_data = " response: the message send  is " + msg->msg_map["result"] + " !\n";
		retu_data = "From "+ msg->msg_map["from"] + retu_data;
	}
	else if (rece_data == "sendmessage")
	{
		//输出发送信息结果
		retu_data = " response: the message get is " + msg->msg_map["content"] + " !\n";
		retu_data = "From " + msg->msg_map["from"] + retu_data;
	}
	else if (rece_data == "listresponse")
	{
		//注意item需要自己在循环内修改
		//map内的顺序已被改变
		int num_bound = (int)msg->msg_map.size();
		num_bound = (num_bound - 1) / 3;
		int i = 1;
		while (i <= num_bound)
		{
			retu_data += "Num " + msg->msg_map["Num" + std::to_string(i)]+ "  ";
			retu_data += "IP " + msg->msg_map["IP" + std::to_string(i)] + "  ";
			retu_data += "Port " + msg->msg_map["Port" + std::to_string(i)] + "\n";
			i++;
		}
	}
	else if (rece_data == "shutdownresponse")
	{
		retu_data = "Response: the shutdown operation  is " + msg->msg_map["result"] + " !\n";
	}
	else
	{
		//其余情况，表示type解析错误
		retu_data = "Response error type!\n";
	}
	return retu_data;
}
