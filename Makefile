warehouse.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

link : 
	g++ -o bin/main bin/WareHouse.o bin/main.o

run : 
	./bin/main configFileExample.txt

tamar : 
	echo f