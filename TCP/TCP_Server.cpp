#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <cstdlib>


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

void send_func(int sock_id, char* recv_buf, int recv_buflen)
{	
	mtx.unlock();					 	
	int iSendResult = send(sock_id, recv_buf, sizeof(recv_buf), 0);
	if(iSendResult > 0)
		{
			printf("%s %d\n", "Bytes send", iSendResult);		
		}
	mtx.lock();
	return;
}	


int main(int argc, char* argv[]) 
{
#ifdef _WIN32	
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

		struct addrinfo *result = NULL;
		//struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof (hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

// Resolve the local address and port to be used by the server
		iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) 
			{
				printf("getaddrinfo failed: %d\n", iResult);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("getaddrinfo executed: %d\n", iResult);
			}	

// Create a SOCKET for the server to listen for client connections
		SOCKET ListenSocket = INVALID_SOCKET;
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) 
			{
				printf("Error at socket(): %d\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("Socket fd: %d\n", (int)ListenSocket);
			}
			
// Setup the TCP listening socket
		iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		int iOptVal = 0;
		int iOptLen = sizeof (int);
		setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &iOptVal, iOptLen);
		if (iResult == SOCKET_ERROR) 
			{
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("bind not failed: %d\n", iResult);
			}
			
// Listening on a Socket
		if (listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR) 
			{
				printf("Listen failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("Listen not failed\n\n\n");
			}

			
// Accept a client socket
		printf("Hello Developer, it's not yet multithreading Server. My apologies\n\n\n");
		SOCKET ClientSocket = INVALID_SOCKET;
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) 
			{
				printf("accept failed: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
		else
			{
				printf("accept not failed: %d\n", (int)ClientSocket);
			}

			
		char recvbuf[DEFAULT_BUFLEN];
		int iSendResult;
		int recvbuflen = DEFAULT_BUFLEN;
		
// Receive 
		do 
			{
				iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
				if (iResult > 0) 
					{
						printf("Bytes received: %d\n", iResult);

// Echo the buffer back to the sender
						iSendResult = send(ClientSocket, recvbuf, iResult, 0);
						if (iSendResult == SOCKET_ERROR) 
							{
								printf("send failed: %d\n", WSAGetLastError());
								closesocket(ClientSocket);
								WSACleanup();
								return 1;
							}
						printf("Bytes sent: %d\n", iSendResult);			
					} 
					
				else if (iResult == 0)
					{
						printf("Connection closing...\n");
					}
						
				else 
					{
						printf("recv failed: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}
					
			} while (true);
	#endif

#else
		struct sockaddr_in addr;
		int ls;
		int rc;						
		int err;				
		char recvbuf[DEFAULT_BUFLEN];		
		int recvbuflen = DEFAULT_BUFLEN;

//Initialize Socket
				
		ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(ls == -1)
			{
				printf("%s\n" ,"Server failed!");
				return 1;
			}
		else
			{
				printf("%s\n" ,"Server executed!");
			}
		setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, 0, 0);
		memset(&addr , 0, sizeof(addr));
		
		addr.sin_family = AF_INET;
		addr.sin_port = htons(std::atoi(DEFAULT_PORT));
		addr.sin_addr.s_addr = INADDR_ANY;
		
// Setup the TCP listening socket		
		
		if ((err = bind(ls, (struct sockaddr*) &addr, sizeof(addr))) < 0)
			{
				 close(ls);
				 printf("%s\n" ,"Binding failed");
				 return 1;
			}
		else 
			{
				printf("%s\n", "Binding success");
			}
			
// Listening on a Socket
			
		if (listen(ls, 25) < 0) 
			{ 
				 close(ls);
				 printf("%s\n" ,"Listening error!");
				 return 1;
			}
		else
			{
				printf("%s\n", "Listening success");
			}
			
// Accept a client socket
			
		rc = accept(ls, NULL, NULL);				
		if (rc < 0) 
			{
				printf("%s\n", "Accept failed");	
			}
		
		else if (rc > 0 ) 
			 { 					 				 	 
				 printf("%s\n", "Accept not failed");						 				 
			 }				
			
		do
			{											
																
				if(rc)
					{
// Receive
						std::thread thr_recv(recv_func, rc, recvbuf, recvbuflen);
						thr_recv.join();
						
// Echo the buffer back to the sender
																		
						std::thread thr_send(send_func, rc, recvbuf, recvbuflen);		 				
		 				thr_send.join();		 					
					}																						
 				 	 				 				 				 				 		
			}while (true); 
#endif		
	
	return 0;
}

