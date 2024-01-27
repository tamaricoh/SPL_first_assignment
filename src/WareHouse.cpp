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
noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0))
{
    parse(configFilePath);
}

void WareHouse :: start(){
    open();
    string input; 
    do {
        std::cout << "Tamar: start"<< std::endl;
        std::getline(std::cin,input);
        std::stringstream ss(input);
        string firstWord;
        string input;
        string nothing;

        ss >> firstWord;
        // iffffffffffff the info is correct to the act====================================================
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
                std::cout << "Tamar: start closing"<< std::endl;
                Close* cl = new Close(); // line 103
                cl -> act(*this);
            }
            continue;
        }

        if (firstWord == "backup") {
            if (!(ss >> input)){
                // ==============================================================
            }
            continue;
        }

        if (firstWord == "restore") {
            if (!(ss >> input)){
                // ==============================================================
            }
            continue;
        }
    std::cout << "\n" << std::endl;
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

void WareHouse:: close(){
    std::cout << "Tamar: close"<< std::endl;
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
            std::cout << id << std::endl;
            
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
    std::cout << "Tamar: cleanup "<< std::endl;
    for(Order* ord : pendingOrders){
        std::cout << "Tamar: ord " << ord->getId() << std::endl;
        delete ord;
        ord = nullptr;
    }

    for(Order* ord : inProcessOrders){
        std::cout << "Tamar: ord " << ord->getId() << std::endl;
        delete ord;
        ord = nullptr;
    }

    for(Order* ord : completedOrders){
        std::cout << "Tamar: ord " << ord->getId() << std::endl;
        delete ord;
        ord = nullptr;
    }

    for(Customer* cust : customers){
        std::cout << "Tamar: cust " << cust->getId() << std::endl;
        delete cust;
        cust = nullptr;
    }

    for(Volunteer* vol : volunteers){
        std::cout << "Tamar: vol " << vol->getId() << std::endl;
        delete vol;
        vol = nullptr;
    }

    for(BaseAction* act : actionsLog){
        std::cout << "Tamar: " << "blah" << std::endl;
        delete act;
        act = nullptr;
    }

    delete noCustomer;
    noCustomer = nullptr;

    delete noOrder;
    noOrder = nullptr;

    delete noVolunteer;
    noVolunteer = nullptr;
}

void WareHouse:: step(){
    //  --1--
    auto ordLoc = pendingOrders.begin();
    while (ordLoc != pendingOrders.end()){
        Order* order = *ordLoc;
        if (order->getStatus() == OrderStatus::PENDING && findCollector(*order)){
            inProcessOrders.push_back(order);
            ordLoc = pendingOrders.erase(ordLoc);
        }
        else if (order->getStatus() == OrderStatus::COLLECTING && findDriver(*order)){
            inProcessOrders.push_back(order);
            ordLoc = pendingOrders.erase(ordLoc);
        }
        else {
            ++ordLoc;
        }
    }
    //  --2 + 3--
    for (Volunteer* vol : volunteers){
        bool checkIfComplete = vol -> isBusy();
        vol -> step();
        if (checkIfComplete != vol->isBusy()){
            auto ordLoc = inProcessOrders.begin();
            while (ordLoc != inProcessOrders.end()){
                Order* order = *ordLoc;
                if (order->getStatus() == OrderStatus::COLLECTING){
                    pendingOrders.push_back(order);
                    ordLoc = inProcessOrders.erase(ordLoc);
                }
                else if (order->getStatus() == OrderStatus::DELIVERING){
                    order->setStatus(OrderStatus::COMPLETED);
                    completedOrders.push_back(order);
                    ordLoc = inProcessOrders.erase(ordLoc);
                }
                else {
                    ++ordLoc;
                }
            }
        }
    }
    // --4--
    auto voLoc = volunteers.begin();
    while (voLoc != volunteers.end()){
        Volunteer* volunteer = *voLoc;
        if (LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(volunteer)){
            bool toDelete = limitedCollector -> getNumOrdersLeft() == 0;
            if(toDelete){
                voLoc = volunteers.erase(voLoc);
                delete(limitedCollector);
            }
            else {
                ++voLoc;
            }
        }
        else if (LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(volunteer)){
            bool toDelete = limitedDriver -> getNumOrdersLeft() == 0;
            if(toDelete){
                voLoc = volunteers.erase(voLoc);
                delete(limitedDriver);  //watch out for memory leak
            }
            else {
                ++voLoc;
            }
        }
        else {
            ++voLoc;
        }
    }
}

bool WareHouse:: findCollector(Order& order) const{
    for(Volunteer* volunteer : volunteers){
        if (LimitedCollectorVolunteer* limitedCollectorVolunteer = dynamic_cast<LimitedCollectorVolunteer*>(volunteer)) {
            if (limitedCollectorVolunteer->canTakeOrder(order)){
                limitedCollectorVolunteer->acceptOrder(order);
                order.setCollectorId(limitedCollectorVolunteer->getId());
                order.setStatus(OrderStatus::COLLECTING);
                return true;
            }
        }
        else if (CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(volunteer)) {
            if (collectorVolunteer->canTakeOrder(order)){
                collectorVolunteer->acceptOrder(order);
                order.setCollectorId(collectorVolunteer->getId());
                order.setStatus(OrderStatus::COLLECTING);
                return true;
            }
        } 
    }
    return false; 
}

bool WareHouse:: findDriver(Order& order) const{
    for(Volunteer* volunteer : volunteers){
        if (LimitedDriverVolunteer* limitedDriverVolunteer = dynamic_cast<LimitedDriverVolunteer*>(volunteer)) {
            if (limitedDriverVolunteer->canTakeOrder(order)){
                limitedDriverVolunteer->acceptOrder(order);
                order.setDriverId(limitedDriverVolunteer->getId());
                order.setStatus(OrderStatus::DELIVERING);
                return true;
            }
        }
        else if (DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(volunteer)) {
            if (driverVolunteer->canTakeOrder(order)){
                driverVolunteer->acceptOrder(order);
                order.setDriverId(driverVolunteer->getId());
                order.setStatus(OrderStatus::DELIVERING);
                return true;
            }
        }
    }
    return false;
}

bool WareHouse:: isNumber(const std::string& str) {
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}


// rule of 5 ~~~~~~~~~~~~~~~~~~~~~~~~
WareHouse:: ~WareHouse(){
    // delete all memory from the heap
    std::cout << "Tamar: ~warehouse"<< std::endl;
    cleanUp();
}

WareHouse:: WareHouse(const WareHouse &other){
    //================================================================
}

WareHouse& WareHouse:: operator=(const WareHouse &other){
    //================================================================
}



















