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

proj :
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

link : 
	g++ -o bin/main bin/WareHouse.o bin/main.o bin/Customer.o bin/Volunteer.o bin/Order.o bin/Action.o

run : 
	./bin/main configFileExample.txt

tamar : 
	echo Tamar the queen 