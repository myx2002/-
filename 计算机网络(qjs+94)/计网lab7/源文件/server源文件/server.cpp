#include<ws2tcpip.h>
#include<winsock.h>
#include<stdio.h>
#include<iostream>
#include<thread>
#include<queue>
#include<map>
#include<string>
#include<list>

//用于获取本地时间
#include<time.h>

//定义消息接受结构
//数据包为size - type，size - value
//类型记录为type - value
struct message
{
	//一定要用map，记录属性名与属性值的对映
	std::map<std::string, std::string> msg_map;
};


//定义简易客户端的基本信息
struct client
{
	SOCKET cli_socket;
	int cli_port;
	std::string cli_IP;
};

//定义sever服务器结构
struct server
{
	SOCKET ser_socket;
	int port;
	int exit_flag;
	std::vector<client>sub_cli;
};

int initial_server_socket(struct server* sever);
void cli_thread_func(std::vector<client>* cli, SOCKET cli_socket,int* exit_flag);
int dis_open(message* msg, char* buffer, int num);
std::string di_close(message* msg);
int send_message(std::string& str, SOCKET cli_socket);

//记录连接的client个数
int con_num = 0;
int main()
{
	printf("Hello\n");
	printf("%d clients have connected!\n", con_num);
	//check winsock
	WSADATA wsaData;
	if (!(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0))
	{
		std::cout << "Failed to initialize Winsock." << std::endl;
		system("pause");
		return 0;
	}

	//初始化server结构体
	struct server* my_server = new server;
	//绑定端口
	my_server->port = 4530;
	my_server->exit_flag = 0;
	//获得套接字
	int ini_flag = initial_server_socket(my_server);
	//判断是否成功
	if (ini_flag == 1)
	{
		printf("Create socket failed!\n");
		system("pause");
		return 0;
	}
	//设置服务器地址信息
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	//使用ipv4协议
	server_addr.sin_family = AF_INET;
	//设置服务器端口
	//使用htons函数将其转换为网络字节序对映的端口号
	server_addr.sin_port = htons(my_server->port);
	//设置对映的服务器ip地址
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定套接字到指定地址和端口
	if (bind(my_server->ser_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("Failed to bind socket!\n");
		closesocket(my_server->ser_socket);
		system("pause");
		return 0;
	}

	// 监听连接请求
	if (listen(my_server->ser_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Failed to listen!\n");
		closesocket(my_server->ser_socket);
		system("pause");
		return 0;
	}
	std::list<std::thread> thread_list;
	while (1)
	{	
		//等待客户端连接
		sockaddr_in client_addr;
		//客户端client_addr结构大小
		int client_num = sizeof(client_addr);
		//没有请求则一直处于阻塞状态
		//通过client_addr可以获得相应客户端信息
		SOCKET client_socket = accept(my_server->ser_socket, (sockaddr*)&client_addr, &client_num);
		//用于介导服务器的关闭
		if (my_server->exit_flag)
		{
			printf("Waiting the sub-thread to exit!\n");
			break;
		}
		//判断连接是否成功
		if (client_socket == INVALID_SOCKET)
		{
			//连接错误提示
			printf("Connect failed!\n");
			continue;
		}

		//获得客户端信息
		//对映于list请求操作，可能需要返回客户端的port、IP以及num信息
		//声明IP的最大字节数为16（已包含'\0'）
		char cli_ip[16];
		inet_ntop(AF_INET, &(client_addr.sin_addr), cli_ip, 16);
		//保留client信息
		client sub_cli;
		sub_cli.cli_socket = client_socket;
		sub_cli.cli_port = client_addr.sin_port;
		sub_cli.cli_IP = cli_ip;
		//连接的client加1
		con_num++;
		//将获得的client客户端放入server结构体中
		my_server->sub_cli.emplace_back(sub_cli);

		//建立子线程响应
		std::thread cli_thread(&cli_thread_func, &my_server->sub_cli, client_socket, &my_server->exit_flag);
		thread_list.push_back(std::move(cli_thread));

		printf("Connect successfully!\n");
		printf("%d clients have connected!\n",con_num);
	}

	//等待所有子线程关闭
	for (auto item = thread_list.begin();item != thread_list.end();item++)
	{
		(*item).join();
	}
	printf("Server closed!\n");

	//关闭套接字
	closesocket(my_server->ser_socket);
	WSACleanup();
	system("pause");
	return 0;
}

//初始化server_socket
//直接将socket赋值于结构体中
//正常create返回0，不正常返回1
int initial_server_socket(struct server* sever)
{
	//使用ipv4 + TCP，自动选择合适的协议
	sever->ser_socket = socket(AF_INET, SOCK_STREAM, 0);
	//分配无效
	if (sever->ser_socket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		//清理Windows Sockets库所分配的资源
		WSACleanup();
		//返回分配错误的信息
		return 1;
	}
	return 0;
}

//传入client数组，当前cli_socket套接字，exit退出控制信号量
//注意exit_flag还未被修改！！！对映与客户端中需要发送请求断开连接的request
void cli_thread_func(std::vector<client>* cli, SOCKET cli_socket, int* exit_flag)
{
	//创造消息接受buffer
	char buffer[1024];
	//接受结果判断函数
	int rec_num = 0;
	int rec_offset = 0;
	//返回的request数量
	int re_num = 0;
	int total_offset = 0;
	while (1) 
	{
		//一直receive数据
		if (rec_offset != 0)
		{
			memcpy(buffer, buffer + total_offset - rec_offset, rec_offset);
			memset(buffer + rec_offset, 0, sizeof(buffer) - rec_offset);
		}
		else
		{
			memset(buffer, 0, sizeof(buffer));
			total_offset = 0;
		}
			
		rec_num = recv(cli_socket, buffer + rec_offset, sizeof(buffer) - rec_offset - 1, 0);
		rec_num = rec_num + rec_offset;
		//std::cout << "1" << rec_num << std::endl;
		//std::cout << buffer << std::endl;
		if (rec_num <= 0)
		{
			//若连接已断开
			//关闭为客户端申请的套接字
			closesocket(cli_socket);
			//查找当前cli_socket对映的client连接，删除
			//修改1
			for (auto item = (*cli).begin();item != (*cli).end();item++)
			{
				if (item->cli_socket == cli_socket)
				{
					(*cli).erase(item);
					break;
				}
			}
			con_num--;
			printf("%d clients have connected!\n", con_num);
			//直接退出
			break;
		}
		//接受信息的最后一位置空
		buffer[rec_num] = '\0';
		total_offset = rec_num;
		message msg_get;
		//解析buffer，将数据读出至message中
		while (1)
		{
			rec_offset = dis_open(&msg_get, buffer, rec_num);
			if (msg_get.msg_map["type"] == "timerequest")
			{
				re_num++;
				//std::cout << re_num << "  ";
				std::cout << "Get time request!" << std::endl;
				//获取时间offset
				time_t current_time = time(NULL);
				// 使用 localtime 函数将时间转换为本地时间
				struct tm* local_time = new tm;
				localtime_s(local_time, &current_time);
				// 格式化得到时间
				char timestr[32];
				strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", local_time);
				//生成时间返回数据包
				message msg;
				msg.msg_map["type"] = "timeresponse";
				msg.msg_map["time"] = timestr;
				//string化数据包
				std::string str_send = di_close(&msg);
				//发送直至成功
				while(send_message(str_send, cli_socket));
			}
			else if (msg_get.msg_map["type"] == "namerequest")
			{
				std::cout << "Get name request!" << std::endl;
				char host_name[32];
				//获得服务器名称
				gethostname(host_name, sizeof(host_name));
				//生成时间返回数据包
				message msg;
				msg.msg_map["type"] = "nameresponse";
				msg.msg_map["name"] = host_name;
				//string化数据包
				std::string str_send = di_close(&msg);
				//发送直至成功
				while (send_message(str_send, cli_socket));
			}
			else if(msg_get.msg_map["type"] == "sendrequest")
			{
				std::cout << "Get send request!" << std::endl;
				//获得需要发送消息的userID与message
				//基于port进行信息的发送
				int to_num = stoi(msg_get.msg_map["ID"]);
				std::string to_message = msg_get.msg_map["message"];
				//生成返回的message
				message msg;
				//获得发送消息的socket
				SOCKET send_cli_socket;
				int check_flag = 0;
				//判断指定port是否存在
				for (auto item = (*cli).begin();item != (*cli).end();item++)
				{
					if (item->cli_port == to_num)
					{
						//若存在，获得对映的socket
						check_flag = 1;
						send_cli_socket = item->cli_socket;
						break;
					}
				}
				//如果process不存在 需要修改
				if (check_flag == 0)
				{
					msg.msg_map["type"] = "sendresponse";
					msg.msg_map["from"] = "server";
					msg.msg_map["result"] = " fault: process ID(Port) not exists";

					//string化数据包
					std::string str_send = di_close(&msg);
					//发送直至成功
					while (send_message(str_send, cli_socket));
				}
				else
				{
					msg.msg_map["type"] = "sendresponse";
					msg.msg_map["from"] = "server";
					msg.msg_map["result"] = "success";
					//需要保证发送的顺序
					//string化数据包
					std::string str_send = di_close(&msg);
					//发送直至成功
					while (send_message(str_send, cli_socket));

					//发送消息，借助send_message数据包发送格式
					message send_msg;
					send_msg.msg_map["type"] = "sendmessage";
					send_msg.msg_map["from"] = "client";
					send_msg.msg_map["content"] = to_message;
					//组装消息发送数据包
					std::string str_send_to = di_close(&send_msg);
					//发送直至成功
					while (send_message(str_send_to, send_cli_socket));
				}
			}
			else if (msg_get.msg_map["type"] == "listrequest")
			{
				//生成关闭server回应数据包
				std::cout << "Get list request!" << std::endl;
				message msg;
				msg.msg_map["type"] = "listresponse";
				//标记client当前统计位置
				int i = 1;
				//获取客户端列表中各个客户端的对映信息
				for (auto item = cli->begin();item != cli->end();item++)
				{
					//std::cout << "Num" + std::to_string(i) <<"   "<< std::to_string(i) << std::endl;
					msg.msg_map["Num" + std::to_string(i)] = std::to_string(i);

					//std::cout << "IP" + std::to_string(i) << "   " << item->cli_IP << std::endl;
					msg.msg_map["IP" + std::to_string(i)] = item->cli_IP;

					//std::cout << "Port" + std::to_string(i) << "   " << item->cli_port << std::endl;
					msg.msg_map["Port" + std::to_string(i)] = std::to_string(item->cli_port);

					i++;
				}
				//string化数据包
				std::string str_send = di_close(&msg);
				//发送直至成功
				while (send_message(str_send, cli_socket));
			}
			else if (msg_get.msg_map["type"] == "shutdownrequest")
			{
				//生成关闭server回应数据包
				message msg;
				msg.msg_map["type"] = "shutdownresponse";
				msg.msg_map["result"] = "success";
				//string化数据包
				std::string str_send = di_close(&msg);
				//发送直至成功
				while(send_message(str_send, cli_socket));
				*exit_flag = 1;
				//关闭为客户端申请的套接字
				closesocket(cli_socket);
				//直接退出
				return ;
			}
			else
			{
				printf("Error request!\n");
			}
			if (rec_offset == 0)
				break;
			rec_num = rec_num - rec_offset;
			if (rec_num < rec_offset)
			{
				rec_offset = rec_num;
				break;
			}		
		}
	}
}

//解析buffer获得的信息
int dis_open(message* msg, char* buffer, int num)
{
	//msg->msg_map.clear();
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
		std::string value(buffer + index, value_num);
		index = index + value_num;
		//帧界定
		if (type == "zz_end" && index != num)
			return index;
		else if(type != "zz_end")
			msg->msg_map[type] = value;
	}
	return 0;
}

//send message信息函数
//传递应用参数
//正常发送返回 0
//发送失败返回 1
int send_message(std::string& str, SOCKET cli_socket)
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