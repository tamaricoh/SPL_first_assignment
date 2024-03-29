link : action.o order.o volunteer.o customer.o main.o warehouse.o
	g++ -o bin/warehouse bin/WareHouse.o bin/main.o bin/Customer.o bin/Volunteer.o bin/Order.o bin/Action.o

all:
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse configFileExample.txt

warehouse.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

main.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp 

customer.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp 

volunteer.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

order.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp

action.o :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

run : link
	./bin/warehouse configFileExample.txt

clean : 
	rm -f bin/*

tamar : 
	echo Tamar the greatest 