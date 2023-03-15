all: server
	
clean:
			rm -rf server*.o server
			
server.o: server.cpp

			g++ -c -o server.o  server.cpp
			
server: server.o

			g++ -pthread -o server  server.o