#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Order.h"
#include "../include/Action.h"

using namespace std;

WareHouse :: WareHouse(const string &configFilePath):
isOpen(false),
actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
customerCounter(0), volunteerCounter(0), orderCounter(0), 
noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0))
{
    parse(configFilePath);
}

void WareHouse :: parse (const string &configFilePath){

    ifstream file(configFilePath); // Open the file

    // Check if the file opened successfully
    if (file.is_open()) {

        string line;
        string firstWord;
        string name;
        string orderLimit;
        string type; // or role
        string dist;
        string distPerStep;
        string temp; // Becaue orderLimit is optional in volunteers
        int id;

        while (getline(file, line)) { 

            // Skip lines that beggin with '#'
            if ((line.size() == 0) | ( line.size() > 0 && line[0] == '#')) {
                continue; 
            }

            istringstream iss(line);
            iss >> firstWord >> name >> type >> dist;

            // for costumers
            if (type == "civilian"){
                iss >> orderLimit;
                id = customers.size();
                customers.push_back(new CivilianCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "soldier"){
                iss >> orderLimit;
                id = customers.size();
                customers.push_back(new SoldierCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }

            
            // for volunteers
            if (type.find("driver") != std::string::npos){
                iss >> distPerStep;
            }

            if(iss >> temp && temp != "#"){
                orderLimit = temp;
            } 

            if (type == "collector"){
                id = volunteers.size();
                volunteers.push_back(new CollectorVolunteer(id, name, stoi(dist)));
            }
            if (type == "limited_collector"){
                id = volunteers.size();
                volunteers.push_back(new LimitedCollectorVolunteer(id, name, stoi(dist), stoi(orderLimit)));

            }
            if (type == "driver"){
                id = volunteers.size();
                volunteers.push_back(new DriverVolunteer(id, name, stoi(dist), stoi(distPerStep)));
            }
            if (type == "limited_driver"){
                id = volunteers.size();
                volunteers.push_back(new LimitedDriverVolunteer(id, name, stoi(dist), stoi(distPerStep), stoi(orderLimit)));
            }   
        } 
    }
}

void WareHouse :: start(){
    if (!isOpen) {
        isOpen = true;
        cout << "Warehouse is open!" << endl;
        // the program waits for the user to enter an action to execute
    } else {
        cout << "The Warehouse is already open." << endl;
    }
}

void WareHouse:: addOrder(Order* order){
    // order added to pendingOrders because this func handles only new orders.
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse:: addCustomer(Customer* Customer){
    customers.push_back(Customer);
    customerCounter++;
}

void WareHouse:: addAction(BaseAction* action){}

Customer& WareHouse:: getCustomer(int customerId) const{
    for (Customer* customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
    return *noCustomer;
}

Volunteer& WareHouse:: getVolunteer(int volunteerId) const{
    for (Volunteer* volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    return *noVolunteer;
}

Order& WareHouse:: getOrder(int orderId) const{
    for (const auto& order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }

    for (const auto& order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }

    for (const auto& order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return *noOrder;
}

const vector<BaseAction*>& WareHouse:: getActions() const{
    return actionsLog;
}

void WareHouse:: close(){}

void WareHouse:: open(){}

int WareHouse:: getOrderCount() const{
    return orderCounter;
}

int WareHouse:: getCustomerCount() const{
    return customerCounter;
}

const vector<Order*> WareHouse:: getCustomerOrders(int customerId) const{}