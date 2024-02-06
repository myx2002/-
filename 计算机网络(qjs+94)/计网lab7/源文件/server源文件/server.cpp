#include<ws2tcpip.h>
#include<winsock.h>
#include<stdio.h>
#include<iostream>
#include<thread>
#include<queue>
#include<map>
#include<string>
#include<list>

//���ڻ�ȡ����ʱ��
#include<time.h>

//������Ϣ���ܽṹ
//���ݰ�Ϊsize - type��size - value
//���ͼ�¼Ϊtype - value
struct message
{
	//һ��Ҫ��map����¼������������ֵ�Ķ�ӳ
	std::map<std::string, std::string> msg_map;
};


//������׿ͻ��˵Ļ�����Ϣ
struct client
{
	SOCKET cli_socket;
	int cli_port;
	std::string cli_IP;
};

//����sever�������ṹ
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

//��¼���ӵ�client����
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

	//��ʼ��server�ṹ��
	struct server* my_server = new server;
	//�󶨶˿�
	my_server->port = 4530;
	my_server->exit_flag = 0;
	//����׽���
	int ini_flag = initial_server_socket(my_server);
	//�ж��Ƿ�ɹ�
	if (ini_flag == 1)
	{
		printf("Create socket failed!\n");
		system("pause");
		return 0;
	}
	//���÷�������ַ��Ϣ
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	//ʹ��ipv4Э��
	server_addr.sin_family = AF_INET;
	//���÷������˿�
	//ʹ��htons��������ת��Ϊ�����ֽ����ӳ�Ķ˿ں�
	server_addr.sin_port = htons(my_server->port);
	//���ö�ӳ�ķ�����ip��ַ
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ���׽��ֵ�ָ����ַ�Ͷ˿�
	if (bind(my_server->ser_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("Failed to bind socket!\n");
		closesocket(my_server->ser_socket);
		system("pause");
		return 0;
	}

	// ������������
	if (listen(my_server->ser_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Failed to listen!\n");
		closesocket(my_server->ser_socket);
		system("pause");
		return 0;
	}
	std::list<std::thread> thread_list;
	while (1)
	{	
		//�ȴ��ͻ�������
		sockaddr_in client_addr;
		//�ͻ���client_addr�ṹ��С
		int client_num = sizeof(client_addr);
		//û��������һֱ��������״̬
		//ͨ��client_addr���Ի����Ӧ�ͻ�����Ϣ
		SOCKET client_socket = accept(my_server->ser_socket, (sockaddr*)&client_addr, &client_num);
		//���ڽ鵼�������Ĺر�
		if (my_server->exit_flag)
		{
			printf("Waiting the sub-thread to exit!\n");
			break;
		}
		//�ж������Ƿ�ɹ�
		if (client_socket == INVALID_SOCKET)
		{
			//���Ӵ�����ʾ
			printf("Connect failed!\n");
			continue;
		}

		//��ÿͻ�����Ϣ
		//��ӳ��list���������������Ҫ���ؿͻ��˵�port��IP�Լ�num��Ϣ
		//����IP������ֽ���Ϊ16���Ѱ���'\0'��
		char cli_ip[16];
		inet_ntop(AF_INET, &(client_addr.sin_addr), cli_ip, 16);
		//����client��Ϣ
		client sub_cli;
		sub_cli.cli_socket = client_socket;
		sub_cli.cli_port = client_addr.sin_port;
		sub_cli.cli_IP = cli_ip;
		//���ӵ�client��1
		con_num++;
		//����õ�client�ͻ��˷���server�ṹ����
		my_server->sub_cli.emplace_back(sub_cli);

		//�������߳���Ӧ
		std::thread cli_thread(&cli_thread_func, &my_server->sub_cli, client_socket, &my_server->exit_flag);
		thread_list.push_back(std::move(cli_thread));

		printf("Connect successfully!\n");
		printf("%d clients have connected!\n",con_num);
	}

	//�ȴ��������̹߳ر�
	for (auto item = thread_list.begin();item != thread_list.end();item++)
	{
		(*item).join();
	}
	printf("Server closed!\n");

	//�ر��׽���
	closesocket(my_server->ser_socket);
	WSACleanup();
	system("pause");
	return 0;
}

//��ʼ��server_socket
//ֱ�ӽ�socket��ֵ�ڽṹ����
//����create����0������������1
int initial_server_socket(struct server* sever)
{
	//ʹ��ipv4 + TCP���Զ�ѡ����ʵ�Э��
	sever->ser_socket = socket(AF_INET, SOCK_STREAM, 0);
	//������Ч
	if (sever->ser_socket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		//����Windows Sockets�����������Դ
		WSACleanup();
		//���ط���������Ϣ
		return 1;
	}
	return 0;
}

//����client���飬��ǰcli_socket�׽��֣�exit�˳������ź���
//ע��exit_flag��δ���޸ģ�������ӳ��ͻ�������Ҫ��������Ͽ����ӵ�request
void cli_thread_func(std::vector<client>* cli, SOCKET cli_socket, int* exit_flag)
{
	//������Ϣ����buffer
	char buffer[1024];
	//���ܽ���жϺ���
	int rec_num = 0;
	int rec_offset = 0;
	//���ص�request����
	int re_num = 0;
	int total_offset = 0;
	while (1) 
	{
		//һֱreceive����
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
			//�������ѶϿ�
			//�ر�Ϊ�ͻ���������׽���
			closesocket(cli_socket);
			//���ҵ�ǰcli_socket��ӳ��client���ӣ�ɾ��
			//�޸�1
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
			//ֱ���˳�
			break;
		}
		//������Ϣ�����һλ�ÿ�
		buffer[rec_num] = '\0';
		total_offset = rec_num;
		message msg_get;
		//����buffer�������ݶ�����message��
		while (1)
		{
			rec_offset = dis_open(&msg_get, buffer, rec_num);
			if (msg_get.msg_map["type"] == "timerequest")
			{
				re_num++;
				//std::cout << re_num << "  ";
				std::cout << "Get time request!" << std::endl;
				//��ȡʱ��offset
				time_t current_time = time(NULL);
				// ʹ�� localtime ������ʱ��ת��Ϊ����ʱ��
				struct tm* local_time = new tm;
				localtime_s(local_time, &current_time);
				// ��ʽ���õ�ʱ��
				char timestr[32];
				strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", local_time);
				//����ʱ�䷵�����ݰ�
				message msg;
				msg.msg_map["type"] = "timeresponse";
				msg.msg_map["time"] = timestr;
				//string�����ݰ�
				std::string str_send = di_close(&msg);
				//����ֱ���ɹ�
				while(send_message(str_send, cli_socket));
			}
			else if (msg_get.msg_map["type"] == "namerequest")
			{
				std::cout << "Get name request!" << std::endl;
				char host_name[32];
				//��÷���������
				gethostname(host_name, sizeof(host_name));
				//����ʱ�䷵�����ݰ�
				message msg;
				msg.msg_map["type"] = "nameresponse";
				msg.msg_map["name"] = host_name;
				//string�����ݰ�
				std::string str_send = di_close(&msg);
				//����ֱ���ɹ�
				while (send_message(str_send, cli_socket));
			}
			else if(msg_get.msg_map["type"] == "sendrequest")
			{
				std::cout << "Get send request!" << std::endl;
				//�����Ҫ������Ϣ��userID��message
				//����port������Ϣ�ķ���
				int to_num = stoi(msg_get.msg_map["ID"]);
				std::string to_message = msg_get.msg_map["message"];
				//���ɷ��ص�message
				message msg;
				//��÷�����Ϣ��socket
				SOCKET send_cli_socket;
				int check_flag = 0;
				//�ж�ָ��port�Ƿ����
				for (auto item = (*cli).begin();item != (*cli).end();item++)
				{
					if (item->cli_port == to_num)
					{
						//�����ڣ���ö�ӳ��socket
						check_flag = 1;
						send_cli_socket = item->cli_socket;
						break;
					}
				}
				//���process������ ��Ҫ�޸�
				if (check_flag == 0)
				{
					msg.msg_map["type"] = "sendresponse";
					msg.msg_map["from"] = "server";
					msg.msg_map["result"] = " fault: process ID(Port) not exists";

					//string�����ݰ�
					std::string str_send = di_close(&msg);
					//����ֱ���ɹ�
					while (send_message(str_send, cli_socket));
				}
				else
				{
					msg.msg_map["type"] = "sendresponse";
					msg.msg_map["from"] = "server";
					msg.msg_map["result"] = "success";
					//��Ҫ��֤���͵�˳��
					//string�����ݰ�
					std::string str_send = di_close(&msg);
					//����ֱ���ɹ�
					while (send_message(str_send, cli_socket));

					//������Ϣ������send_message���ݰ����͸�ʽ
					message send_msg;
					send_msg.msg_map["type"] = "sendmessage";
					send_msg.msg_map["from"] = "client";
					send_msg.msg_map["content"] = to_message;
					//��װ��Ϣ�������ݰ�
					std::string str_send_to = di_close(&send_msg);
					//����ֱ���ɹ�
					while (send_message(str_send_to, send_cli_socket));
				}
			}
			else if (msg_get.msg_map["type"] == "listrequest")
			{
				//���ɹر�server��Ӧ���ݰ�
				std::cout << "Get list request!" << std::endl;
				message msg;
				msg.msg_map["type"] = "listresponse";
				//���client��ǰͳ��λ��
				int i = 1;
				//��ȡ�ͻ����б��и����ͻ��˵Ķ�ӳ��Ϣ
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
				//string�����ݰ�
				std::string str_send = di_close(&msg);
				//����ֱ���ɹ�
				while (send_message(str_send, cli_socket));
			}
			else if (msg_get.msg_map["type"] == "shutdownrequest")
			{
				//���ɹر�server��Ӧ���ݰ�
				message msg;
				msg.msg_map["type"] = "shutdownresponse";
				msg.msg_map["result"] = "success";
				//string�����ݰ�
				std::string str_send = di_close(&msg);
				//����ֱ���ɹ�
				while(send_message(str_send, cli_socket));
				*exit_flag = 1;
				//�ر�Ϊ�ͻ���������׽���
				closesocket(cli_socket);
				//ֱ���˳�
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

//����buffer��õ���Ϣ
int dis_open(message* msg, char* buffer, int num)
{
	//msg->msg_map.clear();
	//��¼buffer�ĵ�ǰ����λ��
	int index = 0;
	while (index < num)
	{
		//��ȡtype���ֽ���
		unsigned long type_num = (unsigned long)buffer[index];
		index++;
		//��ȡtype���͵ľ����ַ���
		std::string type(buffer + index, type_num);
		index = index + type_num;
		//��ȡvalue���ֽ���
		unsigned long value_num = (unsigned long)buffer[index];
		index++;
		//��ȡvalue�ľ�����Ϣ
		std::string value(buffer + index, value_num);
		index = index + value_num;
		//֡�綨
		if (type == "zz_end" && index != num)
			return index;
		else if(type != "zz_end")
			msg->msg_map[type] = value;
	}
	return 0;
}

//send message��Ϣ����
//����Ӧ�ò���
//�������ͷ��� 0
//����ʧ�ܷ��� 1
int send_message(std::string& str, SOCKET cli_socket)
{
	int send_num = send(cli_socket, str.c_str(), str.size(), 0);
	if (send_num < 0)
	{
		return 1;
	}
	return 0;
}

//��message�м�¼��mapת��Ϊstring
std::string di_close(message* msg)
{
	std::string str;
	for (auto item = (*msg).msg_map.begin(); item != (*msg).msg_map.end(); item++)
	{
		//���������͵�type�ֽ���
		str += (char)item->first.size();
		//����type����
		str += item->first;
		//�������ӳ��value�ֽ�������Ϣ
		str += (char)item->second.size();
		str += item->second;
	}
	return str;
}