#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <csignal>

#define DEFAULT_PORT "49152"
#define DEFAULT_ADDRESS ""
#define DEFAULT_BUFLEN 512

#define GET_FILE '1'
#define SEND_SIGTERM '2'
#define SEND_SIGHUP '3'
#define EXIT '4'

std::mutex mtx;

void send_func(int sock_id, char* send_buf, int send_buflen)
{	
    mtx.unlock();					 	
    int iSendResult = send(sock_id, send_buf, sizeof(send_buf), 0);
    if(iSendResult > 0)
    {
        printf("%s %s\n", "Bytes send", send_buf);		
    }
    mtx.lock();
    return;
}

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

void clear_buf(char* recv_buf)
{    
    for(int i = 0; i < sizeof(recv_buf); i++)
    {
        recv_buf[i] = '\0';
    }
}

int check_buf(char* recv_buf)
{    
    std::string empty;
    if(recv_buf == empty)
    {        
        return 1;
    }
    else
    {
        return 0;
    }        
}

void signalHandler(int signum)
{
   //std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);  
}
	

int main(int argc, char* argv[]) 
{
    struct sockaddr_in addr;
    int ls;
    int rc;						
    int err;        
    char recvbuf[DEFAULT_BUFLEN];		
    int recvbuflen = DEFAULT_BUFLEN;
    char enter;
    char sendbuf[DEFAULT_BUFLEN];  	 					
    int sendbuflen = sizeof(sendbuf); 

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
        std::cout << std::endl;
        std::cout << "Select action" << std::endl;
        std::cout << "GET_FILE: enter 1" << std::endl;
        std::cout << "SIGTERM: enter 2" << std::endl;
        std::cout << "SIGHUP: enter 3" << std::endl;
        std::cout << "EXIT: enter 4" << std::endl;
        std::cout << "Enter: ";
        std::cin >> enter;
        std::cout << std::endl;        
        if(enter == GET_FILE)
        {   
            std::cout << "Send file from client" << std::endl << std::endl;
            std::ofstream out;        
            out.open("file_out.txt");
            if(out.is_open())
            {
                while(true)
                {
                    std::thread thr_recv(recv_func, rc, recvbuf, recvbuflen);
                    thr_recv.join();
                    out << recvbuf << std::endl;
                    int check = check_buf(recvbuf);                                       
                    if(check)
                    {
                        out.close();
                        break;
                    }
                    clear_buf(recvbuf);
                }                
            }            
        }
        else if(enter == SEND_SIGTERM)
        {
            std::signal(SIGTERM, signalHandler); 
            std::raise(SIGTERM);                 
        }
        else if(enter == SEND_SIGHUP)
        {
            std::signal(SIGHUP, signalHandler);
            std::raise(SIGTERM);                  
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