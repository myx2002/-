#include<winsock.h>
//#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<thread>
#include<queue>
#include<map>
#include<string>

//������Ϣ���ܽṹ
//���ݰ�Ϊsize - type��size - value
//���ͼ�¼Ϊtype - value
struct message
{
	//һ��Ҫ��map����¼������������ֵ�Ķ�ӳ
	std::map<std::string, std::string> msg_map;
};

//�ͻ���client�ṹ��
struct client
{
	SOCKET client_socket;
	//���߳�
	std::thread* cli_thread;
	int con_flag;
	//��Ϣ����
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
	//������Ϊ�⺯����ʹ�ò���������
	std::cout << "Hello" << std::endl;
	//check winsock
	WSADATA wsaData;
	if (!(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0))
	{
		std::cout << "Failed to initialize Winsock." << std::endl;
		system("pause");
		return 0;
	}

	//ѡ������ź�
	int op_num = -1;
	//��¼�б��Ƿ�鿴�ź�
	int list_num = 0;
	//��¼message��û�з��ͳɹ�
	int sen_num;
	//��Ϣ���ܵȴ��ź�
	int while_wait;

	//�����׽��ֿռ�
	struct client* my_client = new client;
	if (my_client == NULL)
	{
		system("pause");
		return 0;
	}
		
	//�Ƿ����ӱ�־
	my_client->con_flag = 0;
	//��ʼ��socket
	int ini_flag = initial_client_socket(my_client);
	//���ݷ���ֵ�ж���ȷ��
	if (ini_flag == 1)
	{
		std::cout << "Create socket failed!" << std::endl;
		system("pause");
		return 0;
	}

	//�����ѭ��
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
				//�����Ҫ���ӵķ�����IP��ַ��˿���Ϣ
				char* IP = (char*)malloc(sizeof(char) * 32);
				char ip_get[32] = "127.0.0.1";
				int port = 4530;
				IP = ip_get;
				//printf("Please input the connect IP address:\n");
				//std::cin >> IP;
				//printf("Please input the connect port:\n");
				//std::cin >> port;
				
				//�������Ӳ���
				printf("Connecting\n");
				int con_check = client_connect(IP, port, my_client);
				if (con_check == 0)
				{
					//����ɹ�������Ϣ
					printf("Successfully connected!\n");
				}
				else if (con_check == 1)
				{
					//���������Ϣ
					printf("Failed to connect to the server\n");
					printf("Quit\n");
					system("pause");
					return 0;
				}
			}
			else if (op_num == 7)
			{
				//ֱ���˳�����
				printf("Good Bye!\n");
				system("pause");
				return 0;
			}
		}
		else if (my_client->con_flag == 1)
		{
			if (op_num == 1)
			{
				//ÿ���ͻ���ֻ��һ�����߳�����
				printf("Have been connected!\n");
			}
			else if (op_num == 2)
			{
				//�Ͽ�����
				closesocket(my_client->client_socket);
				my_client->con_flag = 0;
				printf("Connect down!\n");
				//�����µ�socket���Լ����´�����
				initial_client_socket(my_client);
				my_client->con_flag = 0;
				list_num = 0;
			}
			else if (op_num == 3)
			{
				
				//Sleep(350);
				//�ϳ�ʱ������message
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
				//�ϳ�name����message
				message msg;
				msg.msg_map["type"] = "namerequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
			else if (op_num == 5)
			{
				//�ϳɷ������б�����message
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
				//������Ҫ���͵���Ϣ
				message msg;
				msg.msg_map["type"] = "sendrequest";
				//��ȡ��Ӧ�Ŀͻ����б����
				printf("Please enter the process ID(Port):\n");
				std::string ID;
				std::cin >> ID;
				//��ȡ��Ӧ����
				printf("Please enter the message:\n");
				std::string mess_text;
				std::cin >> mess_text;
				//����Ӧ����Ϣ����msg��map��
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
				//�ȶϿ����ӣ��ر��׽���
				closesocket(my_client->client_socket);
				my_client->con_flag = 0;
				//�ٹرճ���
				printf("Good Bye!\n");
				system("pause");
				return 0;
			}
			else if (op_num == 8)
			{
				//�ϳɹرշ���������message
				message msg;
				msg.msg_map["type"] = "shutdownrequest";
				msg.msg_map["zz_end"] = "end";
				std::string str = di_close(&msg);
				sen_num = send_message(str, my_client->client_socket);
				printf("Waiting for the reply!\n");
			}
		}
		//�ж��Ƿ��ͳɹ�
		if (sen_num == 1)
			printf("Send message failed!\n");
		else if (sen_num == 0)
			printf("Send message successfully!\n");
		
		//wait server�ĵ�һ�β�����һ����Ҫ���ѽϳ���ʱ�䣩
		Sleep(200);
		//�ȵ����յ�server�����ݰ��ٹر���Ӧ���׽���
		if (op_num == 8)
		{
			//�Ͽ����ӣ��ر��׽���,��֪�������ر��ź�
			closesocket(my_client->client_socket);
			my_client->con_flag = 0;
			list_num = 0;

			//�����µ�socket��connect���Դ����������Ĺرղ���
			initial_client_socket(my_client);
			char IP[32] = "127.0.0.1";
			int port = 4530;
			client_connect(IP, port, my_client);

			//��Ϊ�������رգ���Ҫ�Ͽ��ͻ��˵����ӽӿ�
			closesocket(my_client->client_socket);
		}
		while_wait = 1;
		//Сѭ�������ͻ��˴�����Ϣ���У���Ҫ����Ϣ���д�����ɺ��ٿ�ʼ��һ����������ѯ
		while (while_wait == 1)
		{
			//һ�����ֻ��Ҫ����һ��replay����
			while (!my_client->mess_que.empty())
			{
				message msg;
				//���map��
				msg = my_client->mess_que.front();
				my_client->mess_que.pop();
				//��������
				std::string re_str = message_solve(&msg);
				std::cout << re_str << std::endl;
			}
			while_wait = 0;
		}
		Sleep(200);
		//�رշ������ҽ��յ���Ϣ��ֱ���˳�
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
//չʾѡ�������Ϣ
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

//��ʼ��client_socket
//ֱ�ӽ�socket��ֵ�ڽṹ����
//����create����0������������1
int initial_client_socket(struct client* user)
{
	//ʹ��ipv4 + TCP���Զ�ѡ����ʵ�Э��
	user->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	//������Ч
	if (user->client_socket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		//����Windows Sockets�����������Դ
		WSACleanup();
		//���ط���������Ϣ
		return 1;
	}
	return 0;
}

//�鵼�ͻ�������������Ӻ���
//���ɹ����ӣ��򷵻�0
//������ʧ�ܣ���ر��׽��֣�������1
int client_connect(char* ip, int port, struct client* user)
{
	//���÷�������ַ��Ϣ
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	//ʹ��ipv4Э��
	server_addr.sin_family = AF_INET;
	//���÷������˿�
	//ʹ��htons��������ת��Ϊ�����ֽ����ӳ�Ķ˿ں�
	server_addr.sin_port = htons(port);
	//���ö�ӳ�ķ�����ip��ַ
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	//�������������ַ��Ϣ֮��ʹ��connect�����������Ӳ���
	//�ж�connect���Ӳ����Ƿ�ɹ�
	if (connect(user->client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		//�ر��׽��ֲ��ͷŷ������Դ
		closesocket(user->client_socket);
		WSACleanup();
		return 1;
	}
	else
	{
		//�������߳�
		user->con_flag = 1;
		//���ͻ��˵�message�б�socket��con_flag������Ϣ�������߳���
		user->cli_thread = new std::thread(threadFunction, &user->client_socket, &user->mess_que, &user->con_flag);
		return 0;
	}
	return 0;
}

//���߳�ʵ�ֺ���
void threadFunction(SOCKET* cli_socket, std::queue<message>* mess_que, int* con_flag)
{
	//��ʱ����bufferΪ1024�ֽ�
	char buffer[1024];
	//ʹ��rec_num���receive�������ص���Ϣ�ֽ���
	int rec_num;
	//ֻҪ���Ӵ���on״̬����Ҫһֱ���շ�������Ϣ
	while (*con_flag)
	{
		//�Ƚ�buffer���
		memset(buffer, 0, sizeof(buffer));
		rec_num = recv(*cli_socket, buffer, sizeof(buffer) - 1, 0);
		if (rec_num <= 0 || rec_num >= 1023)
		{
			//�����յ��ķ���ֵ��Ϊ0��˵�������ѶϿ�
			printf("The connection has done!\n");
			*con_flag = 0;
			return;
		}
		//printf("Get message successfully!\n");
		//�����Ϣ������
		buffer[rec_num] = '\0';
		message msg;
		//����buffer�������ݶ�����message��
		dis_open(&msg, buffer, rec_num);
		//printf("Disopen successfully!\n");
		//��message��Ϣ����client�ṹmessage������
		(*mess_que).push(msg);
	}
	return;
}

//����buffer��õ���Ϣ
void dis_open(message* msg, char* buffer, int num)
{
	msg->msg_map.clear();
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
		std::string value(buffer + index,  value_num);
		index = index + value_num;
		msg->msg_map[type] = value;
	}
}

//send message��Ϣ����
//����Ӧ�ò���
//�������ͷ��� 0
//����ʧ�ܷ��� 1
int send_message(std::string &str, SOCKET cli_socket)
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

//������õ�msg��Ϣ
int time_num = 0;
std::string message_solve(message* msg)
{
	//��¼response����
	std::string rece_data;
	//��¼response��ӳ������
	std::string retu_data;
	rece_data = msg->msg_map["type"];
	//�ж�����
	if (rece_data == "timeresponse")
	{
		//�������ʱ����
		//time_num++;
		//retu_data = std::to_string(time_num) + " Response: the time is " + msg->msg_map["time"] + " !\n";
		retu_data = " Response: the time is " + msg->msg_map["time"] + " !\n";
	}
	else if (rece_data == "nameresponse")
	{
		//��������������
		retu_data = "Response: the name is " + msg->msg_map["name"] + " !\n";
	}
	else if (rece_data == "sendresponse")
	{
		//���������Ϣ���
		retu_data = " response: the message send  is " + msg->msg_map["result"] + " !\n";
		retu_data = "From "+ msg->msg_map["from"] + retu_data;
	}
	else if (rece_data == "sendmessage")
	{
		//���������Ϣ���
		retu_data = " response: the message get is " + msg->msg_map["content"] + " !\n";
		retu_data = "From " + msg->msg_map["from"] + retu_data;
	}
	else if (rece_data == "listresponse")
	{
		//ע��item��Ҫ�Լ���ѭ�����޸�
		//map�ڵ�˳���ѱ��ı�
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
		//�����������ʾtype��������
		retu_data = "Response error type!\n";
	}
	return retu_data;
}
