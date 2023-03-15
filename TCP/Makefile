all: TCP_Server
	
clean:
			rm -rf TCP_Server*.o
			
TCP_Server.o: TCP_Server.cpp

			g++ -c -o TCP_Server.o  TCP_Server.cpp
			
TCP_Server: TCP_Server.o

			g++ -pthread -o TCP_Server  TCP_Server.o
			
