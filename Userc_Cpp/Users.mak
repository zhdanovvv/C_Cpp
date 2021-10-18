all: Users
	
clean:
			rm -rf Users*.o
			
Users.o: Users.cpp

			g++ -c -o Users.o  Users.cpp
			
Users: Users.o

			g++ -o Users  Users.o
			
