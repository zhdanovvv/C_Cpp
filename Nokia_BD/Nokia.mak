all: Test_Nokia
	
clean:
			rm -rf Test_Nokia*.o
			
Test_Nokia.o: Test_Nokia.cpp

			g++ -c -o Test_Nokia.o  Test_Nokia.cpp
			
Test_Nokia: Test_Nokia.o

			g++ -o Test_Nokia  Test_Nokia.o
			
