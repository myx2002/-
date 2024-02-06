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


//定义sever服务器结构
struct server
{
	SOCKET ser_socket;
	int ser_port;
	//可能不需要处理服务器退出
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
	
	//初始化server结构体
	struct server* my_server = new server;
	//绑定端口
	my_server->ser_port = 4530;
	//获得套接字
	int ini_flag = initial_server_socket(my_server);
	//判断套接字申请是否成功
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
	server_addr.sin_port = htons(my_server->ser_port);
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

	//获取当前进程运行路径
	char route_buf[1024];
	if (!GetModuleFileNameA(NULL, route_buf, 1024))
	{
		std::cout << "Get path failed!\n" << std::endl;
		system("pause");
		return 0;
	}
	std::string route_buffer = std::string(route_buf);
	//std::cout << route_buffer << std::endl;

	//生成一个绝对路径地址
	//考虑斜杠与反斜杠的可能性
	int last_num = route_buffer.find_last_of("\\/");
	//若未找到,报错退出
	if (last_num == std::string::npos)
	{
		std::cout << "Serch the path failed!\n" << std::endl;
		system("pause");
		return 0;
	}
	//去掉斜杠/反斜杠及之后的路径信息
	route_buffer = route_buffer.substr(0, last_num);
	//for debug
	//std::cout << route_buffer << std::endl;

	//建立子线程列表
	std::list<std::thread> thread_list;
	//主线程循环
	while (1)
	{
		//等待客户端连接
		sockaddr_in client_addr;
		//客户端client_addr结构大小
		int client_num = sizeof(client_addr);

		//没有请求则一直处于阻塞状态
		//通过client_addr可以获得相应客户端信息
		SOCKET client_socket = accept(my_server->ser_socket, (sockaddr*)&client_addr, &client_num);
		std::cout << client_socket << std::endl;
		//判断连接是否成功
		if (client_socket == INVALID_SOCKET)
		{
			//连接错误提示
			printf("Connect failed!\n");
			continue;
		}

		//建立子线程响应
		std::thread cli_thread(&cli_thread_func, &client_socket,&route_buffer);
		thread_list.push_back(std::move(cli_thread));
		Sleep(200);
	}

	//等待所有子线程释放
	for (auto item = thread_list.begin();item != thread_list.end();item++)
		(*item).join();
	std::cout << "Server closed!" << std::endl;


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


//传入当前cli_socket套接字
void cli_thread_func(SOCKET* cli_socket, std::string* path_buf)
{
	std::cout << "In" << std::endl;
	//创造消息接受buffer
	char buffer[1024];
	//接受结果判断参数
	int rec_num;
	//定义退出参数
	int exit_num = 1;
	//string类型用于存放http请求
	std::string http_request_buf;
	//file-header的map记录
	std::map<std::string, std::string>http_header;

	while (exit_num)
	{
		//每一轮清空request buffer
		http_request_buf = "";
		http_header.clear();

		//连续获取http request内容
		while (1)
		{
			//清空buffer，防止不同轮次的数据干扰
			memset(buffer, 0, sizeof(buffer));
			//接收读入个数
			rec_num = recv(*cli_socket, buffer, sizeof(buffer), 0);
			//buffer[rec_num] = '\0';
			if (rec_num <= 0)
			{
				//若连接已断开，关闭为客户端申请的套接字
				closesocket(*cli_socket);
				std::cout << "Socket closed!" << std::endl;
				//直接退出
				exit_num = 0;
				break;
			}
			http_request_buf = http_request_buf + std::string(buffer, 0, rec_num);
			//寻找receive字符串中是否有两个连续的回车
			if (http_request_buf.find("\r\n\r\n") != std::string::npos)
				break;
		}
		//for debug
		std::cout << buffer << std::endl;

		//解析http request
		//将string转化为stream类型
		std::istringstream http_request_stream(http_request_buf);
		std::string http_request_line;
		//获取http_header以外的部分（http request第一行）
		getline(http_request_stream, http_request_line);

		//获取第一行中method与url的信息
		std::istringstream first_line(http_request_line);
		std::string method;
		std::string url;
		//根据http结构，以空格为分界符
		first_line >> method >> url;

		//获得http request信息
		std::string line_message;
		//获取一行数据
		while (getline(http_request_stream, line_message) && !line_message.empty())
		{
			//寻找冒号分隔符
			int clo_num = line_message.find(':');
			if (clo_num != -1)
			{
				std::string sub_key = line_message.substr(0, clo_num);
				//一般而言，冒号后都会跟随一个空格，因而也需要跳过
				std::string sub_value = line_message.substr(clo_num + 2);
				http_header[sub_key] = sub_value;
			}
			//每次清空string
			line_message.clear();
		}

		//需要调整url中的斜杠与反斜杠
		for (int i = 0;i < url.size();i++)
		{
			if (url[i] == '/')
				url[i] = '\\';
		}
		//for debug
		//std::cout << url << std::endl;

		//建立足够大的buffer以接收文件信息
		char* response_buffer = (char*)malloc(sizeof(char) * 1024 * 1024);
		if (response_buffer == NULL)
			std::cout << "Buffer malloc error!" << std::endl;
		int buffer_size = sizeof(char) * 1024 * 1024;

		//根据获取的http request信息进行操作
		if (method == "GET")
		{
			//需要一个绝对地址
			std::string file_path = *path_buf + url;
			//关注，路径正确性需要验证
			//for debug
			//std::cout << file_path << std::endl;

			//清理buffer
			//会影响网页呈现速度
			//memset(response_buffer, 0, buffer_size);
			
			//建立http响应请求数据包
			std::string http_response;

			//读取并传输文件信息
			std::ifstream file_stream(file_path, std::ios::binary);
			if (file_stream.is_open())
			{
				//获取文件大小
				file_stream.seekg(0, std::ios::end);
				int file_size = file_stream.tellg();
				//归位文件流指针位置
				file_stream.seekg(0, std::ios::beg);
				//判断文件容量与buffer间的大小关系
				//for debug 容量足够使用
				//std::cout << file_size << std::endl;
				//std::cout << buffer_size << std::endl;
				if (file_size > buffer_size)
				{
					std::cout << "Buffer too small!" << std::endl;
					return;
				}

				//获取文件类型content-type
				int point_num = file_path.rfind(".");
				std::string point_str;
				//获取文件原始类型
				if (point_num != -1)
					point_str = file_path.substr(point_num + 1);
				else
					point_str = "";
				std::string con_type = get_content_type(point_str);
				
				//构建http_response返回数据包
				http_response = "HTTP/1.1 200 OK\r\n";
				http_response += "Content-Type: " + con_type + "\r\n";
				http_response += "Content-Length: " + std::to_string(file_size) + "\r\n\r\n";
				//发送响应数据包
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);

				//获得并发送文件数据信息
				file_stream.read(response_buffer, file_size);
				//关闭文件流
				file_stream.close();
				//会影响网页呈现速度
				//std::string output_buffer = std::string(response_buffer, file_size);
				//send(*cli_socket, output_buffer.c_str(), file_size, 0);
				send(*cli_socket, response_buffer, file_size, 0);
				//std::cout << response_buffer << std::endl;
			}
			else
			{
				//未找到文件，直接返回错误信息
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
				//如果是dopost请求
				int body_num = http_request_buf.find("\r\n\r\n");
				//获得http请求主体部分子串
				std::string http_body = http_request_buf.substr(body_num + 4);

				//生成返回数据包
				http_response = "HTTP/1.1 200 OK\r\n";
				http_response += "Content-Type: text/html\r\n";
				std::string back_http;
				//定义需要展示的信息
				if (http_body == "login=3210104530&pass=4530")
					back_http = "<html><body><h1>Login success!<h1></body></html>";
				else
					back_http = "<html><body><h1>Login failed!<h1></body></html>";

				//构建返回包head
				http_response = http_response + "Content-Length: " + std::to_string(back_http.size()) + "\r\n\r\n";
				//构建返回包内容
				http_response = http_response + back_http + "\r\n";
				//for debug
				//std::cout << http_response << std::endl;
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);
			}
			else
			{
				std::cout << "Post error!" << std::endl;
				//不是dopost请求，直接返回错误信息
				http_response = "HTTP/1.1 404 Not Found\r\n\r\n";
				send(*cli_socket, http_response.c_str(), http_response.size(), 0);
			}

			std::cout << "End post deal" << std::endl;
			//发送完毕后，关闭socket，退出子线程 ---（可能此处不需要）
			//关闭为客户端申请的套接字
			//closesocket(*cli_socket);
			//直接退出
			//exit_num = 0;
		}
	}
}

//将文件格式转换为http响应数据包的标椎格式
std::string get_content_type(std::string con_str)
{
	if (con_str == "html")
		return "text/html";
	else if (con_str == "jpg")
		return "image/jpeg";
	else if (con_str == "txt")
		return "text/plain";
	else
		//位置文件类型，不作处理
		return "application/octet-stream";
}