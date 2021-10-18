all: Users
	
clean:
			rm -rf Users*.o
			
Users.o: Users.c

			gcc -c -o Users.o  Users.c
			
Users: Users.o

			gcc -o Users  Users.o
