all: client
	
clean:
			rm -rf client*.o client
			
client.o: client.cpp

			g++ -c -o client.o  client.cpp
			
client: client.o

			g++ -pthread -o client  client.o