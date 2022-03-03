#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>

#define DEFAULT_PORT "49152"
#define DEFAULT_ADDRESS ""
#define DEFAULT_BUFLEN 512

#define SEND_FILE '1'
#define EXIT '2'

std::mutex mtx;

void recv_func(int sock_id, char* recv_buf, int recv_buflen)
{	
    mtx.unlock();
    int iResult = recv(sock_id, recv_buf, recv_buflen, 0); 		
    if(iResult > 0)
    {
        printf("%s %s\n", "Bytes recieved", recv_buf);
    }
    mtx.lock();
    return;
}

void send_func(int sock_id, char* send_buf, int send_buflen)
{	
    mtx.unlock();					 	
    int iSendResult = send(sock_id, send_buf, send_buflen, 0);
    if(iSendResult > 0)
    {
        printf("%s %s\n", "Bytes send", send_buf);		
    }
    mtx.lock();
    return;
}

void clear_buf(char* recv_buf)
{    
    for(int i = 0; i < sizeof(recv_buf); i++)
    {
        recv_buf[i] = '/0';
    }
}


int main(int argc, char* argv[])
{		
    struct sockaddr_in addr;		
    int ls;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = sizeof(recvbuf);
    char sendbuf[DEFAULT_BUFLEN];  	 					
    int sendbuflen = sizeof(sendbuf);
    char enter;   
    
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
        std::cout << std::endl;
        std::cout << "Select action" << std::endl;
        std::cout << "SEND_FILE: enter 1" << std::endl;
        std::cout << "EXIT: enter 2" << std::endl;
        std::cout << "Enter: ";
        std::cin >> enter;
        std::cout << std::endl;
        if(enter == SEND_FILE)
        {
            std::ifstream in("file.txt");
            if (in.is_open())
            {
                while (in.getline(sendbuf, sendbuflen))
                {
                    std::thread thr_send(send_func, ls, sendbuf, sendbuflen);
                    thr_send.join();
                    clear_buf(sendbuf);          
                }                    
            }
            in.close();
        }
        else if(enter == EXIT)
        {            
            std::cout << "Auf wiedersehen!!!" << std::endl;
            break;
        }
        else
        {
            continue;
        }                                 		 				

    }while(true);
    				                            	
	return 0;	
}