#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>

#endif

#define DEFAULT_PORT "49152"
#define DEFAULT_ADDRESS ""
#define DEFAULT_BUFLEN 512

std::mutex mtx;

void recv_func(int sock_id, char* recv_buf, int recv_buflen)
{	
	mtx.unlock();
	int iResult = recv(sock_id, recv_buf, recv_buflen, 0); 		
	if(iResult > 0)
	 	{
	 		printf("%s %d\n", "Bytes recieved", iResult);
	 	}
 	mtx.lock();
 	return;
}

void send_func(int sock_id, const char* send_buf, int send_buflen)
{	
	mtx.unlock();					 	
	int iSendResult = send(sock_id, send_buf, send_buflen, 0);
	if(iSendResult > 0)
		{
			printf("%s %d\n", "Bytes send", iSendResult);		
		}
	mtx.lock();
	return;
}


int main(int argc, char* argv[])
{
#if _WIN32
	#if (_WIN32_WINNT >= 0x0501)
		WSADATA wsaData;
		int iResult;

// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) 
			{
				printf("WSAStartup failed: %d\n", iResult);
				return 1;
			}
		else
			{
				printf("WSAStartup executed: %d\n", iResult);
			}
		
		struct addrinfo *result = NULL, *ptr = NULL, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		
// Resolve the server address and port
		iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
			{
				printf("getaddrinfo failed: %d\n", iResult);
				WSACleanup();
				return 1;
			}
		
		SOCKET ConnectSocket = INVALID_SOCKET;
		
		
// Attempt to connect to the first address returned by
// the call to getaddrinfo
		ptr=result;

// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
			{
				printf("Error at socket(): %d\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("getaddrinfo executed: %d\n", iResult);
			}
		
// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
			{
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
			}
			

// Should really try the next address returned by getaddrinfo
// if the connect call failed
// But for this simple example we just free the resources
// returned by getaddrinfo and print an error message

		freeaddrinfo(result);
		if (ConnectSocket == INVALID_SOCKET) 
			{
				printf("Unable to connect to server!\n");
				WSACleanup();
				return 1;
			}
		else
			{
				printf("Connect to server %d\n", iResult);
			}
			
		std::cout << "Enter someting data: " << "\n\n";	
		char* input_str = new char[DEFAULT_BUFLEN];
		gets(input_str);
		char* cod_data = coding_Data(input_str);
		const char *sendbuf = cod_data;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
					

// Receive data until the server closes the connection
		do 
		{
		
// Send an initial buffer

			iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR) 
				{
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return 1;
				}	

			printf("Bytes Sent: %d\n", iResult);
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
				{
					printf("Bytes received: %d\n", iResult);				
				}
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed: %d\n", WSAGetLastError());
								
		} while (true);		
				
	#endif
	
#else
		
		struct sockaddr_in addr;		
		int ls;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		const char *sendbuf = "Hello World";  	 					
		int sendbuf_len = sizeof(sendbuf);
		
// Initialize Socket
				
		ls = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, 0, 0);
		if(ls == -1)
			{
				perror( "Socket can not created!\n");
				return 1;
			}
		memset(&addr , 0, sizeof(addr));
		
		addr.sin_family = AF_INET;
		addr.sin_port = htons(std::atoi(DEFAULT_PORT));
		addr.sin_addr.s_addr = INADDR_ANY;
				
// Connect to server 	 	
		
		if (connect(ls, (const sockaddr*) &addr, sizeof(addr)) < 0)
			{
				 close(ls);
				 perror("connection error!\n");
				 return 1;
			}
		else 
			{
			  	 printf("%s\n", "connection success!");
	  	 	}	  		  	
		
		do		
			{
// Receive data until the server closes the connection
				
			  	std::thread thr_send(send_func, ls, sendbuf, sendbuf_len);		 				
				thr_send.join();
				
// Send an initial buffer				
							 	
		 		std::thread thr_recv(recv_func, ls, recvbuf, recvbuflen);				
				thr_recv.join();
													
			}while(true);
#endif
		
	return 0;	
}
