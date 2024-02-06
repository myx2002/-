#include<ws2tcpip.h>
#include<winsock.h>
#include<stdio.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<thread>
#include<queue>
#include<map>
#include<string>
#include<list>


//����sever�������ṹ
struct server
{
	SOCKET ser_socket;
	int ser_port;
	//���ܲ���Ҫ����������˳�
	//int exit_flag;
};

int initial_server_socket(struct server* sever);
void cli_thread_func(SOCKET* cli_socket, std::string* path_buf);
std::string get_content_type(std::string con_str);

int main()
{
	std::cout << "hello" << std::endl;
	//check and start winsock
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
	my_server->ser_port = 4530;
	//����׽���
	int ini_flag = initial_server_socket(my_server);
	//�ж��׽��������Ƿ�ɹ�
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
	server_addr.sin_port = htons(my_server->ser_port);
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

	//��ȡ��ǰ��������·��
	char route_buf[1024];
	if (!GetModuleFileNameA(NULL, route_buf, 1024))
	{
		std::cout << "Get path failed!\n" << std::endl;
		system("pause");
		return 0;
	}
	std::string route_buffer = std::string(route_buf);
	//std::cout << route_buffer << std::endl;

	//����һ������·����ַ
	//����б���뷴б�ܵĿ�����
	int last_num = route_buffer.find_last_of("\\/");
	//��δ�ҵ�,�����˳�
	if (last_num == std::string::npos)
	{
		std::cout << "Serch the path failed!\n" << std::endl;
		system("pause");
		return 0;
	}
	//ȥ��б��/��б�ܼ�֮���·����Ϣ
	route_buffer = route_buffer.substr(0, last_num);
	//for debug
	//std::cout << route_buffer << std::endl;

	//�������߳��б�
	std::list<std::thread> thread_list;
	//���߳�ѭ��
	while (1)
	{
		//�ȴ��ͻ�������
		sockaddr_in client_addr;
		//�ͻ���client_addr�ṹ��С
		int client_num = sizeof(client_addr);

		//û��������һֱ��������״̬
		//ͨ��client_addr���Ի����Ӧ�ͻ�����Ϣ
		SOCKET client_socket = accept(my_server->ser_socket, (sockaddr*)&client_addr, &client_num);
		std::cout << client_socket << std::endl;
		//�ж������Ƿ�ɹ�
		if (client_socket == INVALID_SOCKET)
		{
			//���Ӵ�����ʾ
			printf("Connect failed!\n");
			continue;
		}

		//�������߳���Ӧ
		std::thread cli_thread(&cli_thread_func, &client_socket,&route_buffer);
		thread_list.push_back(std::move(cli_thread));
		Sleep(200);
	}

	//�ȴ��������߳��ͷ�
	for (auto item = thread_list.begin();item != thread_list.end();item++)
		(*item).join();
	std::cout << "Server closed!" << std::endl;


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


//���뵱ǰcli_socket�׽���
void cli_thread_func(SOCKET* cli_socket, std::string* path_buf)
{
	std::cout << "In" << std::endl;
	//������Ϣ����buffer
	char buffer[1024];
	//���ܽ���жϲ���
	int rec_num;
	//�����˳�����
	int exit_num = 1;
	//string�������ڴ��http����
	std::string http_request_buf;
	//file-header��map��¼
	std::map<std::string, std::string>http_header;

	while (exit_num)
	{
		//ÿһ�����request buffer
		http_request_buf = "";
		http_header.clear();

		//������ȡhttp request����
		while (1)
		{
			//���buffer����ֹ��ͬ�ִε����ݸ���
			memset(buffer, 0, sizeof(buffer));
			//���ն������
			rec_num = recv(*cli_socket, buffer, sizeof(buffer), 0);
			//buffer[rec_num] = '\0';
			if (rec_num <= 0)
			{
				//�������ѶϿ����ر�Ϊ�ͻ���������׽���
				closesocket(*cli_socket);
				std::cout << "Socket closed!" << std::endl;
				//ֱ���˳�
				exit_num = 0;
				break;
			}
			http_request_buf = http_request_buf + std::string(buffer, 0, rec_num);
			//Ѱ��receive�ַ������Ƿ������������Ļس�
			if (http_request_buf.find("\r\n\r\n") != std::string::npos)
				break;
		}
		//for debug
		std::cout << buffer << std::endl;

		//����http request
		//��stringת��Ϊstream����
		std::istringstream http_request_stream(http_request_buf);
		std::string http_request_line;
		//��ȡhttp_header����Ĳ��֣�http request��һ�У�
		getline(http_request_stream, http_request_line);

		//��ȡ��һ����method��url����Ϣ
		std::istringstream first_line(http_request_line);
		std::string method;
		std::string url;
		//����http�ṹ���Կո�Ϊ�ֽ��
		first_line >> method >> url;

		//���http request��Ϣ
		std::string line_message;
		//��ȡһ������
		while (getline(http_request_stream, line_message) && !line_message.empty())
		{
			//Ѱ��ð�ŷָ���
			int clo_num = line_message.find(':');
			if (clo_num != -1)
			{
				std::string sub_key = line_message.substr(0, clo_num);
				//һ����ԣ�ð�ź󶼻����һ���ո����Ҳ��Ҫ����
				std::string sub_value = line_message.substr(clo_num + 2);
				http_header[sub_key] = sub_value;
			}
			//ÿ�����string
			line_message.clear();
		}

		//��Ҫ����url�е�б���뷴б��
		for (int i = 0;i < url.size();i++)
		{
			if (url[i] == '/')
				url[i] = '\\';
		}
		//for debug
		//std::cout << url << std::endl;

		//�����㹻���buffer�Խ����ļ���Ϣ
		char* response_buffer = (char*)malloc(sizeof(char) * 1024 * 1024);
		if (response_buffer == NULL)
			std::cout << "Buffer malloc error!" << std::endl;
		int buffer_size = sizeof(char) * 1024 * 1024;

		//���ݻ�ȡ��http request��Ϣ���в���
		if (method == "GET")
		{
			//��Ҫһ�����Ե�ַ
			std::string file_path = *path_buf + url;
			//��ע��·����ȷ����Ҫ��֤
			//for debug
			//std::cout << file_path << std::endl;

			//����buffer
			//��Ӱ����ҳ�����ٶ�
			//memset(response_buffer, 0, buffer_size);
			
			//����http��Ӧ�������ݰ�
			std::string http_response;

			//��ȡ�������ļ���Ϣ
			std::ifstream file_stream(file_path, std::ios::binary);
			if (file_stream.is_open())
			{
				//��ȡ�ļ���С
				file_stream.seekg(0, std::ios::end);
				int file_size = file_stream.tellg();
				//��λ�ļ���ָ��λ��
				file_stream.seekg(0, std::ios::beg);
				//�ж��ļ�������buffer��Ĵ�С��ϵ
				//for debug �����㹻ʹ��
				//std::cout << file_size << std::endl;
				//std::cout << buffer_size << std::endl;
				if (file_size > buffer_size)
				{
					std::cout << "Buffer too small!" << std::endl;
					return;
				}

				//��ȡ�ļ�����content-type
				int point_num = file_path.rfind(".");
				std::string point_str;
				//��ȡ�ļ�ԭʼ����
				if (point_num != -1)
					point_str = file_path.substr(point_num + 1);
				else
					point_str = "";
				std::string con_type = get_content_type(point_str);
				
				//����http_response�������ݰ�
				http_response = "HTTP/1.1 200 OK\r\n";
				http_response += "Content-Type: " + con_type + "\r\n";
				http_response += "Content-Length: " + std::to_string(file_size) + "\r\n\r\n";
				//������Ӧ���ݰ�
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);

				//��ò������ļ�������Ϣ
				file_stream.read(response_buffer, file_size);
				//�ر��ļ���
				file_stream.close();
				//��Ӱ����ҳ�����ٶ�
				//std::string output_buffer = std::string(response_buffer, file_size);
				//send(*cli_socket, output_buffer.c_str(), file_size, 0);
				send(*cli_socket, response_buffer, file_size, 0);
				//std::cout << response_buffer << std::endl;
			}
			else
			{
				//δ�ҵ��ļ���ֱ�ӷ��ش�����Ϣ
				http_response = "HTTP/1.1 404 Not Found\r\n\r\n";
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);
			}
			//for debug
			//std::cout << "over" << std::endl;
		}
		else if (method == "POST")
		{
			std::string http_response;
			int post_num = url.find("dopost");
			if (post_num != -1)
			{
				//�����dopost����
				int body_num = http_request_buf.find("\r\n\r\n");
				//���http�������岿���Ӵ�
				std::string http_body = http_request_buf.substr(body_num + 4);

				//���ɷ������ݰ�
				http_response = "HTTP/1.1 200 OK\r\n";
				http_response += "Content-Type: text/html\r\n";
				std::string back_http;
				//������Ҫչʾ����Ϣ
				if (http_body == "login=3210104530&pass=4530")
					back_http = "<html><body><h1>Login success!<h1></body></html>";
				else
					back_http = "<html><body><h1>Login failed!<h1></body></html>";

				//�������ذ�head
				http_response = http_response + "Content-Length: " + std::to_string(back_http.size()) + "\r\n\r\n";
				//�������ذ�����
				http_response = http_response + back_http + "\r\n";
				//for debug
				//std::cout << http_response << std::endl;
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);
			}
			else
			{
				std::cout << "Post error!" << std::endl;
				//����dopost����ֱ�ӷ��ش�����Ϣ
				http_response = "HTTP/1.1 404 Not Found\r\n\r\n";
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);
			}

			std::cout << "End post deal" << std::endl;
			//������Ϻ󣬹ر�socket���˳����߳� ---�����ܴ˴�����Ҫ��
			//�ر�Ϊ�ͻ���������׽���
			//closesocket(*cli_socket);
			//ֱ���˳�
			//exit_num = 0;
		}
	}
}

//���ļ���ʽת��Ϊhttp��Ӧ���ݰ��ı�׵��ʽ
std::string get_content_type(std::string con_str)
{
	if (con_str == "html")
		return "text/html";
	else if (con_str == "jpg")
		return "image/jpeg";
	else if (con_str == "txt")
		return "text/plain";
	else
		//λ���ļ����ͣ���������
		return "application/octet-stream";
}