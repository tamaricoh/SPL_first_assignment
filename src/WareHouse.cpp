#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Order.h"
#include "../include/Action.h"


WareHouse :: WareHouse(const string &configFilePath):
isOpen(false),
actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
customerCounter(0), volunteerCounter(0), orderCounter(0), 
noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0)),
backupBool(false)
{
    parse(configFilePath);
}

void WareHouse :: start(){
    open();
    string inputLine; 
    do {
        std::getline(std::cin,inputLine);
        std::stringstream ss(inputLine);
        string firstWord;
        string input;
        string nothing;

        ss >> firstWord;
        if (firstWord == "step") {
            // step <numOfSteps>
            if (ss >> input && !(ss>>nothing) && isNumber(input)){
                int numOfSteps = stoi(input);
                SimulateStep* step = new SimulateStep(numOfSteps);
                step -> act(*this);
            }
            continue;
        }

        if (firstWord == "order") {
            // order <customerId>
            if (ss >> input && !(ss>>nothing) && isNumber(input)){
                int id = stoi(input);
                AddOrder* addOrder = new AddOrder(id);
                addOrder -> act(*this);
            }
            continue;
        }

        if (firstWord == "customer") {
            // customer <customer_name> <customer_type> <customer_distance> <max_orders>
            string name;
            string type;
            string distance;
            string maxOrder;
            if(ss >> name >> type >> distance >> maxOrder && !(ss >> nothing) && isNumber(distance) && isNumber(maxOrder)){
                AddCustomer* addCustomer = new AddCustomer(name, type, stoi(distance), stoi(maxOrder));
                addCustomer -> act(*this);
            }
            continue;
        }

        if (firstWord == "orderStatus") {
            // orderStatus <order_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(input));
                orderStatus -> act(*this);
            }
            continue;
        }

        if (firstWord == "customerStatus") {
            // customerStatus <customer_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(input));
                customerStatus -> act(*this);
            }
            continue;
        }

        if (firstWord == "volunteerStatus") {
            // volunteerStatus <volunteer_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(input));
                volunteerStatus -> act(*this);
            }
            continue;
        }

        if (firstWord == "log") {
            if (!(ss >> input)){
                PrintActionsLog* log = new PrintActionsLog();
                log -> act(*this);
            }
            continue;
        }

        if (firstWord == "close") {
            if (!(ss >> input)){
                Close* cl = new Close();
                cl -> act(*this);
            }
            continue;
        }

        if (firstWord == "backup") {
            if (!(ss >> input)){
                BackupWareHouse* bUp = new BackupWareHouse();
                bUp -> act(*this);
                this->setBackup(true);
            }
            continue;
        }

        if (firstWord == "restore") {
            if (!(ss >> input)){
                RestoreWareHouse* res = new RestoreWareHouse();
                res -> act(*this);
            }
            continue;
        }
    } while(isOpen);
}

const vector<BaseAction*>& WareHouse:: getActions() const{
    return actionsLog;
}

void WareHouse:: addOrder(Order* order){
    // order added to pendingOrders because this func handles only new orders.
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse:: addAction(BaseAction* action){
    actionsLog.push_back(action);
}

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
    for (const auto& order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return *noOrder;
}

void WareHouse:: close(){
    for (Order* ord : pendingOrders){
        std::cout << ord->closeInfo() << std::endl;
    }
    for (Order* ord : inProcessOrders){
        std::cout << ord->closeInfo() << std::endl;
    }
    for (Order* ord : completedOrders){
        std::cout << ord->closeInfo() << std::endl;
    }
    isOpen = false;
}

void WareHouse:: open(){
     if (!isOpen) {
        isOpen = true;
        std::cout << "Warehouse is open!" << std::endl;
    } else {
        std::cout << "The Warehouse is already open." << std::endl;
        close();
    }
}


// new func ~~~~~~~~~~~~~~~~~~~~~~~~
int WareHouse:: getOrderCount() const{
    return orderCounter;
}

int WareHouse:: getCustomerCount() const{
    return customerCounter;
}


void WareHouse :: parse (const string &configFilePath){  

    std:: ifstream file(configFilePath); // Open the file

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

            std:: istringstream iss(line);
            iss >> firstWord >> name >> type >> dist;

            // for costumers
            if (type == "civilian" && isNumber(dist)){
                iss >> orderLimit;
                id = customerCounter;
                if (isNumber(orderLimit)) {
                    addCustomer(new CivilianCustomer(id, name, stoi(dist), stoi(orderLimit)));
                }
            }
            if (type == "soldier" && isNumber(dist) && isNumber(orderLimit)){
                iss >> orderLimit;
                id = customerCounter;
                if (isNumber(orderLimit)){
                    addCustomer(new SoldierCustomer(id, name, stoi(dist), stoi(orderLimit)));
                }
            }

            
            // for volunteers
            if (type.find("driver") != std::string::npos){
                iss >> distPerStep;
            }

            if(iss >> temp && temp != "#"){
                orderLimit = temp;
            } 

            if (type == "collector" && isNumber(dist)){
                id = volunteerCounter;
                addVolunteer(new CollectorVolunteer(id, name, stoi(dist)));
            }
            if (type == "limited_collector" && isNumber(dist) && isNumber(orderLimit)){
                id = volunteerCounter;
                addVolunteer(new LimitedCollectorVolunteer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "driver" && isNumber(dist) && isNumber(distPerStep)){
                id = volunteerCounter;
                addVolunteer(new DriverVolunteer(id, name, stoi(dist), stoi(distPerStep)));
            }
            if (type == "limited_driver" && isNumber(dist) && isNumber(distPerStep) && isNumber(orderLimit)){
                id = volunteerCounter;
                addVolunteer(new LimitedDriverVolunteer(id, name, stoi(dist), stoi(distPerStep), stoi(orderLimit)));
            }             
        } 
    }
}

void WareHouse:: addCustomer(Customer* Customer){
    customers.push_back(Customer);
    customerCounter++;
}

void WareHouse:: addVolunteer(Volunteer* Volunteer){
    volunteers.push_back(Volunteer);
    volunteerCounter++;
}

void WareHouse:: cleanUp(){
    for(Order* ord : pendingOrders){
        delete ord;
        ord = nullptr;
    }
    pendingOrders.clear();
    for(Order* ord : inProcessOrders){
        delete ord;
        ord = nullptr;
    }
    inProcessOrders.clear();;
    for(Order* ord : completedOrders){
        delete ord;
        ord = nullptr;
    }
    completedOrders.clear();
    for(Customer* cust : customers){
        delete cust;
        cust = nullptr;
    }
    customers.clear();
    for(Volunteer* vol : volunteers){
        delete vol;
        vol = nullptr;
    }
    volunteers.clear();
    for(BaseAction* act : actionsLog){
        delete act;
        act = nullptr;
    }
    actionsLog.clear();

    delete noCustomer;
    noCustomer = nullptr;

    delete noOrder;
    noOrder = nullptr;

    delete noVolunteer;
    noVolunteer = nullptr;
}

void WareHouse:: step(){

    // Stage 1
    for (auto pendOrdLoc = pendingOrders.begin(); pendOrdLoc != pendingOrders.end(); ++pendOrdLoc){
        Order* ord = *pendOrdLoc;
        if (ord->getStatus() == OrderStatus::COLLECTING && passToNextVol(*ord, "Driver")){ // Pass to a Driver
            pendOrdLoc = pendingOrders.erase(pendOrdLoc);
            --pendOrdLoc;
            continue;
        }
        if (ord->getStatus() == OrderStatus::PENDING && passToNextVol(*ord, "Collector")){ // Pass to a Collector
            pendOrdLoc = pendingOrders.erase(pendOrdLoc);
            --pendOrdLoc;
            continue;
        }
    }

    // Stage 2
    for (auto volLoc = volunteers.begin(); volLoc != volunteers.end(); ++volLoc){
        (*volLoc)->step(); 
    }

    // Stage 3
    for (auto inProOrdLoc = inProcessOrders.begin(); inProOrdLoc != inProcessOrders.end(); ++inProOrdLoc){
        Order* ord = *inProOrdLoc;
        int collectorId = ord->getCollectorId();
        int driverId = ord->getDriverId();
        Volunteer* collector = &(getVolunteer(collectorId));
        Volunteer* driver = &(getVolunteer(driverId));
        string test;
        driver->getCompleteInCurrentStep()? test = "true" : test = "false";
        if (ord->getStatus() == OrderStatus::DELIVERING && driver->getCompleteInCurrentStep() == true){
            ord->setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(ord);
            inProOrdLoc = inProcessOrders.erase(inProOrdLoc);
            --inProOrdLoc;
            continue;
        }
        if (ord->getStatus() == OrderStatus::COLLECTING && collector->getCompleteInCurrentStep() == true){
            pendingOrders.push_back(ord);
            inProOrdLoc = inProcessOrders.erase(inProOrdLoc);
            --inProOrdLoc;
            continue;
        }
    }

    // Stage 4
    for (auto volLoc = volunteers.begin(); volLoc != volunteers.end(); ++volLoc){
        Volunteer* vol = *volLoc;
        if (!vol->isBusy() && vol->getReachedMaxOrder()){
            volLoc = volunteers.erase(volLoc);
            --volLoc;
        }
    }
}

bool WareHouse:: passToNextVol(Order& order, string type){
    for(Volunteer* volunteer : volunteers){
        if (volunteer->type() == type && volunteer -> canTakeOrder(order)) {
                volunteer->acceptOrder(order); // Update ordersLeft volunteer member in case he is Limited volunteer
                if (type == "Collector"){
                    order.setStatus(OrderStatus::COLLECTING);
                    order.setCollectorId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    return true;
                }
                if (type == "Driver"){
                    order.setStatus(OrderStatus::DELIVERING);
                    order.setDriverId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    return true;
                }
            }
    }
    return false;
}

bool WareHouse:: isNumber(const std::string& str) const {
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

void WareHouse::setBackup(bool restored) {
    backupBool = restored;
}

bool WareHouse:: getBackup() const{
    return backupBool;
}

// rule of 5 ~~~~~~~~~~~~~~~~~~~~~~~~
WareHouse:: ~WareHouse(){
    // delete all memory from the heap
    cleanUp();
}

WareHouse:: WareHouse(const WareHouse &other) : 
    isOpen(other.isOpen),
    actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
    customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter), 
    noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0)),
    backupBool(false){
        for (Order* ord : other.pendingOrders){
            pendingOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.inProcessOrders){
            inProcessOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.completedOrders){
            completedOrders.push_back(ord -> clone());
        }
        for (Volunteer* vol : other.volunteers){
            volunteers.push_back(vol -> clone());
        }
        for (Customer* cust : other.customers){
            customers.push_back(cust -> clone());
        }
        for (BaseAction* act : other.actionsLog){
            actionsLog.push_back(act -> clone());
        }
}

WareHouse& WareHouse:: operator=(const WareHouse &other){
    if (this != &other) {// not a self-assignment
        cleanUp();

        // simple types 
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        backupBool = other.backupBool;

        // objects
        noCustomer = other.noCustomer -> clone();
        noVolunteer = other.noVolunteer -> clone();
        noOrder = other.noOrder -> clone();

        for (Order* ord : other.pendingOrders){
            pendingOrders.push_back(ord -> clone());
        }

        for (Order* ord : other.inProcessOrders){
            inProcessOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.completedOrders){
            completedOrders.push_back(ord -> clone());
        }
        for (Volunteer* vol : other.volunteers){
            volunteers.push_back(vol -> clone());
        }
        for (Customer* cust : other.customers){
            customers.push_back(cust -> clone());
        }
        for (BaseAction* act : other.actionsLog){
            actionsLog.push_back(act -> clone());
        }
    }
    return *this;
}

WareHouse:: WareHouse(WareHouse&& other) noexcept: 
    isOpen(other.isOpen),
    actionsLog(std::move(other.actionsLog)), 
    volunteers(std::move(other.volunteers)),
    pendingOrders(std::move(other.pendingOrders)),
    inProcessOrders(std::move(other.inProcessOrders)),
    completedOrders(std::move(other.completedOrders)),
    customers(std::move(other.customers)),
    customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
    noCustomer(other.noCustomer), noVolunteer(other.noVolunteer), noOrder(other.noOrder),
    backupBool(false){
    other.noCustomer = nullptr;
    other.noVolunteer = nullptr;
    other.noOrder = nullptr;
}

WareHouse& WareHouse::operator=(WareHouse&& other) noexcept{
    if(this != &other){
        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog); 
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);  
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        noCustomer = other.noCustomer;
        noVolunteer = other.noVolunteer;
        noOrder = other.noOrder;
        backupBool = other.backupBool;

        other.customerCounter = 0;
        other.orderCounter = 0;
        other.volunteerCounter = 0;
        other.noCustomer = nullptr;
        other.noVolunteer = nullptr;
        other.noOrder = nullptr;
    }
    return *this;
}

















