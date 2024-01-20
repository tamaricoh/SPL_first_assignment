warehouse.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

main.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp 

customer.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp 

link : 
	g++ -o bin/main bin/WareHouse.o bin/main.o bin/Customer.o

run : 
	./bin/main configFileExample.txt

tamar : 
	echo f