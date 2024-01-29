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
customerCounter(0), volunteerCounter(0), orderCounter(0), 
noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0)),
backupBool(false)
{
    parse(configFilePath);
}

void WareHouse :: start(){
    open();
    string input; 
    do {
        std::getline(std::cin,input);
        std::stringstream ss(input);
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
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "order") {
            // order <customerId>
            if (ss >> input && !(ss>>nothing) && isNumber(input)){
                int id = stoi(input);
                AddOrder* addOrder = new AddOrder(id);
                addOrder -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
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
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "orderStatus") {
            // orderStatus <order_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(input));
                orderStatus -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "customerStatus") {
            // customerStatus <customer_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(input));
                customerStatus -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "volunteerStatus") {
            // volunteerStatus <volunteer_id>
            if(ss >> input && !(ss>>nothing) && isNumber(input)){
                PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(input));
                volunteerStatus -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "log") {
            if (!(ss >> input)){
                PrintActionsLog* log = new PrintActionsLog();
                log -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "close") {
            if (!(ss >> input)){
                Close* cl = new Close();
                cl -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "backup") {
            if (!(ss >> input)){
                BackupWareHouse* bUp = new BackupWareHouse();
                bUp -> act(*this);
                this->setBackup(true);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
            continue;
        }

        if (firstWord == "restore") {
            if (!(ss >> input)){
                RestoreWareHouse* res = new RestoreWareHouse();
                res -> act(*this);
            }
            std::cout <<"Tamar: _____________________________"<<std::endl;
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
    std:: cout << "Tamar: step ________________________________________________" << std::endl;

    // Stage 1
    for (auto pendOrdLoc = pendingOrders.begin(); pendOrdLoc != pendingOrders.end(); ++pendOrdLoc){
        Order* ord = *pendOrdLoc;
        if (ord->getStatus() == OrderStatus::COLLECTING){ // Pass to a Driver
            std:: cout << "Tamar: pass to driver" << std::endl;
            passToNextVol(*ord, "Driver");
            pendOrdLoc = pendingOrders.erase(pendOrdLoc);
            --pendOrdLoc;
            continue;
        }
        if (ord->getStatus() == OrderStatus::PENDING){ // Pass to a Collector
            std:: cout << "Tamar: pass to collector" << std::endl;
            passToNextVol(*ord, "Collector");
            pendOrdLoc = pendingOrders.erase(pendOrdLoc);
            --pendOrdLoc;
            continue;
        }
    }
    for (auto inProOrdLoc = inProcessOrders.begin(); inProOrdLoc != inProcessOrders.end(); ++inProOrdLoc){
        Order* ord = *inProOrdLoc;
        std:: cout << "Tamar: order in process " << ord->getId() << " " << ord->EnumToOrderStatus(ord->getStatus()) << std::endl;
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
        std:: cout << "Tamar: stage 3 - " << ord->EnumToOrderStatus(ord->getStatus()) << " " << test << " " << ord-> getId() << std::endl;
        if (ord->getStatus() == OrderStatus::COLLECTING && collector->getCompleteInCurrentStep() == true){
            std:: cout << "Tamar: complete collect " << ord-> getId() << std::endl;
            pendingOrders.push_back(ord);
            inProOrdLoc = inProcessOrders.erase(inProOrdLoc);
            --inProOrdLoc;
            continue;
        }
        if (ord->getStatus() == OrderStatus::DELIVERING && driver->getCompleteInCurrentStep() == true){
            std:: cout << "Tamar: completed" << ord-> getId() << std::endl;
            ord->setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(ord);
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








    //  --1--
    // auto ordLoc = pendingOrders.begin();
    // while (ordLoc != pendingOrders.end()){
    //     Order* order = *ordLoc;
    //     std:: cout << "Tamar: step 1 ---" << order->getId() << " ---------- " << order->EnumToOrderStatus(order->getStatus()) << std::endl;
    //     if (order->getStatus() == OrderStatus::PENDING && findVol(*order, "Collector")){
    //         std:: cout << "Tamar: found collector" << std::endl;
    //         inProcessOrders.push_back(order);
    //         ordLoc = pendingOrders.erase(ordLoc);
    //     }
    //     else if (order->getStatus() == OrderStatus::COLLECTING && findVol(*order, "Driver")){ 
    //         std:: cout << "Tamar: found driver" << std::endl;
    //         std:: cout << "Tamar: " << order->EnumToOrderStatus(order->getStatus()) << std::endl;
    //         inProcessOrders.push_back(order);
    //         ordLoc = pendingOrders.erase(ordLoc);
    //     }
    //     else {
    //         std:: cout << "Tamar: found nothing" << std::endl;
    //         ++ordLoc;
    //     }
    // }
    // //  --2 + 3--
    // for (Volunteer* vol : volunteers){
    //     std:: cout << "Tamar: 2 -3 loop " << vol->getName() << std::endl;
    //     bool checkIfComplete = vol -> isBusy();
    //     vol -> step();
    //     if (checkIfComplete != vol->isBusy()){
    //         std:: cout << "Tamar: if vol complete" << std::endl;
    //         auto ordLoc = inProcessOrders.begin();
    //         while (ordLoc != inProcessOrders.end()){
    //             Order* order = *ordLoc;
    //              std:: cout << "Tamar: itirate inprocess " << order->getId() << " ---------- " << order->EnumToOrderStatus(order->getStatus())<< std::endl;
    //             if (vol->getId() == order->getCollectorId() && order->getStatus() == OrderStatus::COLLECTING && finishCollect(order)){
    //                 std:: cout << "Tamar: collect" << std::endl;
    //                 pendingOrders.push_back(order);
    //                 ordLoc = inProcessOrders.erase(ordLoc);
    //                 break;
    //             }
    //             else if (vol->getId() == order->getDriverId() && order->getStatus() == OrderStatus::DELIVERING){
    //                 std:: cout << "Tamar: deliver" << std::endl;
    //                 order->setStatus(OrderStatus::COMPLETED);
    //                 std:: cout << "Tamar: comp" << order->getId() << " ---------- " << order->EnumToOrderStatus(order->getStatus())<< std::endl;
    //                 completedOrders.push_back(order);
    //                 ordLoc = inProcessOrders.erase(ordLoc);
    //                 break;
    //             }
    //             else {
    //                 std:: cout << "Tamar: found no" << std::endl;
    //                 ++ordLoc;
    //             }
    //         }
    //     }
    // }
    // // --4--
    // auto voLoc = volunteers.begin();
    // while (voLoc != volunteers.end()){
    //     Volunteer* volunteer = *voLoc;
    //     if (!volunteer->isBusy()){
    //         if (LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(volunteer)){
    //             bool toDelete = limitedCollector -> getNumOrdersLeft() == 0;
    //             if(toDelete){
    //                 voLoc = volunteers.erase(voLoc);
    //                 delete(limitedCollector);
    //             }
    //             else {
    //                 ++voLoc;
    //             }
    //         }
    //         else if (LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(volunteer)){
    //             bool toDelete = limitedDriver -> getNumOrdersLeft() == 0;
    //             if(toDelete){
    //                 voLoc = volunteers.erase(voLoc);
    //                 delete(limitedDriver);  //watch out for memory leak
    //             }
    //             else {
    //                 ++voLoc;
    //             }
    //         }
    //         else {
    //             ++voLoc;
    //         }
    //     }
    //     else {
    //             ++voLoc;
    //         }
    // }
}

void WareHouse:: passToNextVol(Order& order, string type){
    std:: cout << "Tamar: type " << type << std::endl;
    for(Volunteer* volunteer : volunteers){
        if (volunteer->type() == type && volunteer -> canTakeOrder(order)) {
                volunteer->acceptOrder(order); // Update ordersLeft volunteer member in case he is Limited volunteer
                if (type == "Collector"){
                    order.setStatus(OrderStatus::COLLECTING);
                    order.setCollectorId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    std:: cout << "Tamar: collector name " << volunteer->getName() << std::endl;
                    break;
                }
                if (type == "Driver"){
                    order.setStatus(OrderStatus::DELIVERING);
                    order.setDriverId(volunteer->getId());
                    inProcessOrders.push_back(&order);
                    std:: cout << "Tamar: driver name " << volunteer->getName() << std::endl;
                    break;
                }
            }
    }
}

bool WareHouse:: isNumber(const std::string& str) const {
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

// bool WareHouse:: finishCollect(Order* order) const{
//     int id = order-> getCollectorId();
//     Volunteer& col = getVolunteer(id);
//     if (col.getCompletedOrderId() == order-> getId()){
//         return true;
//     }
//     return false;
// }

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
    isOpen(other.isOpen), customerCounter(other.customerCounter), orderCounter(other.orderCounter),
    noCustomer(new CivilianCustomer(-1,"",0,0)), noVolunteer(new CollectorVolunteer(-1,"",0)), noOrder(new Order(-1,0,0)), backupBool(false){
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
    customerCounter(other.customerCounter),
    orderCounter(other.orderCounter),
    volunteerCounter(other.volunteerCounter),
    noCustomer(other.noCustomer), 
    noVolunteer(other.noVolunteer), 
    noOrder(other.noOrder),
    actionsLog(std::move(other.actionsLog)), 
    volunteers(std::move(other.volunteers)),
    customers(std::move(other.customers)),
    pendingOrders(std::move(other.pendingOrders)),
    inProcessOrders(std::move(other.inProcessOrders)),
    completedOrders(std::move(other.completedOrders)) {
    other.noCustomer = nullptr;
    other.noVolunteer = nullptr;
    other.noOrder = nullptr;
}

WareHouse& WareHouse::operator=(WareHouse&& other) noexcept{
    if(this != &other){
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        orderCounter = other.orderCounter;
        other.isOpen = false;
        other.customerCounter = 0;
        other.orderCounter = 0;
        other.volunteerCounter = 0;
        volunteerCounter = other.volunteerCounter;
        actionsLog = std::move(other.actionsLog); 
        volunteers = std::move(other.volunteers);
        customers = std::move(other.customers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);   
    }
    return *this;
}

















