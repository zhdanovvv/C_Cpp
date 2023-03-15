all: TCP_Client
	
clean:
			rm -rf TCP_Client*.o
			
TCP_Client.o: TCP_Client.cpp

			g++ -c -o TCP_Client.o  TCP_Client.cpp
			
TCP_Client: TCP_Client.o

			g++ -pthread -o TCP_Client  TCP_Client.o
			
