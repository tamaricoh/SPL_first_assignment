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
        cout << "Tamar" << endl;
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
                id = customerCounter;
                addCustomer(new CivilianCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "soldier"){
                iss >> orderLimit;
                id = customerCounter;
                addCustomer(new SoldierCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }

            
            // for volunteers
            if (type.find("driver") != std::string::npos){
                iss >> distPerStep;
            }

            if(iss >> temp && temp != "#"){
                orderLimit = temp;
            } 

            if (type == "collector"){
                id = volunteerCounter;
                addVolunteer(new CollectorVolunteer(id, name, stoi(dist)));
            }
            if (type == "limited_collector"){
                id = volunteerCounter;
                addVolunteer(new LimitedCollectorVolunteer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "driver"){
                id = volunteerCounter;
                addVolunteer(new DriverVolunteer(id, name, stoi(dist), stoi(distPerStep)));
            }
            if (type == "limited_driver"){
                id = volunteerCounter;
                addVolunteer(new LimitedDriverVolunteer(id, name, stoi(dist), stoi(distPerStep), stoi(orderLimit)));
            } 
            cout << id << endl;
            
        } 
    }
}

void WareHouse :: start(){
    open();
    string input; 
    do {
        std::getline(std::cin,input);
        stringstream ss(input);
        string firstWord;
        string word;

        ss >> firstWord;

        if (firstWord == "step") {
            // step <numOfSteps>
            if (ss >> word){
                int numOfSteps = stoi(word);
                SimulateStep* step = new SimulateStep(numOfSteps);
                step -> act(*this);
            }
            continue;
        }

        if (firstWord == "order") {
            // order <customerId>
            if (ss >> word){
                int id = stoi(word);
                AddOrder* addOrder = new AddOrder(id);
                addOrder -> act(*this);
            }
            continue;
        }

        if (firstWord == "customer") {
            // customer <customer_name> <customer_type> <customer_distance> <max_orders>
            string name;
            ss >> name;
            string type;
            ss >> type;
            string distance;
            ss >> distance;
            string maxOrder;
            ss >> maxOrder;
            
            AddCustomer* addCustomer = new AddCustomer(name, type, stoi(distance), stoi(maxOrder));
            addCustomer -> act(*this);
            continue;
        }

        if (firstWord == "orderStatus") {
            // orderStatus <order_id>
            string orderId;
            ss >> orderId;
            PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(orderId));
            orderStatus -> act(*this);
            continue;
        }

        if (firstWord == "customerStatus") {
            // customerStatus <customer_id>
            string customerId;
            ss >> customerId;
            PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(customerId));
            customerStatus -> act(*this);
            continue;
        }

        if (firstWord == "volunteerStatus") {
            // volunteerStatus <volunteer_id>
            string volunteerId;
            ss >> volunteerId;
            PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(volunteerId));
            volunteerStatus -> act(*this);
            continue;
        }

        if (firstWord == "log") {
            // ==============================================================
            continue;
        }

        if (firstWord == "close") {
            Close* cl = new Close(); // delete memory
            cl -> act(*this);
            close();
            continue;
        }

        if (firstWord == "backup") {
            // ==============================================================
            continue;
        }

        if (firstWord == "restore") {
            // ==============================================================
            continue;
        }

    } while(isOpen);
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

void WareHouse:: addVolunteer(Volunteer* Volunteer){
    volunteers.push_back(Volunteer);
    volunteerCounter++;
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

void WareHouse:: close(){
    isOpen = false;
}

void WareHouse:: open(){
     if (!isOpen) {
        isOpen = true;
        cout << "Warehouse is open!" << endl;
    } else {
        cout << "The Warehouse is already open." << endl;
        close();
    }
}

int WareHouse:: getOrderCount() const{
    return orderCounter;
}

int WareHouse:: getCustomerCount() const{
    return customerCounter;
}

const vector<Order*> WareHouse:: getCustomerOrders(int customerId) const{}