#include "../include/Action.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"

BaseAction:: BaseAction(){}

ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction:: complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string errorMsg){
    status = ActionStatus::ERROR;
    this -> errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
    
}

string BaseAction:: getErrorMsg() const{
    return errorMsg;
}


//-------------------------------------------------------------------------------------------------------------------
// SimulateStep
SimulateStep :: SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep :: act(WareHouse &wareHouse){
    wareHouse.addAction(this);
    int counter = 0;
    while(counter < numOfSteps){
        wareHouse.step();
        counter++;
    }
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}

std:: string SimulateStep:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "SimulateStep " + std::to_string(numOfSteps)+ " " ;

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// AddOrder
AddOrder:: AddOrder(int id) : customerId(id){}

void AddOrder:: act(WareHouse &wareHouse){
    wareHouse.addAction(this);
    if (customerId > wareHouse.getCustomerCount()){
        error("Cannot place this order");
        return;
    }
    Customer& orderCustomer = wareHouse.getCustomer(customerId);
    if (!orderCustomer.canMakeOrder()){
        error("Cannot place this order");
        
        return;
    }
    int OrderId = wareHouse.getOrderCount();
    orderCustomer.addOrder(OrderId);
    int distance = orderCustomer.getCustomerDistance();
    Order* newOrder = new Order(OrderId, customerId, distance);
    newOrder->setStatus(OrderStatus::PENDING);
    wareHouse.addOrder(newOrder);
    complete();
}

AddOrder* AddOrder:: clone() const{
    return new AddOrder(*this);
}

string AddOrder:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "order " + std::to_string(customerId)+ " ";

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// AddCustomer
AddCustomer:: AddCustomer(const string& customerName, const string& customerType, int distance, int maxOrders) :
customerName(customerName), customerType(CustomerTypeToEnum(customerType)), distance(distance), maxOrders(maxOrders) {}

CustomerType AddCustomer:: CustomerTypeToEnum(const string& type) const {
    if (type == "Soldier") {
        return CustomerType::Soldier;
    } 
    if (type == "Civilian") {
        return CustomerType::Civilian;
    }
}

string AddCustomer:: EnumToCustomerType(CustomerType type) const {
    if (type == CustomerType::Soldier) {
        return "Soldier";
    } 
    if (type == CustomerType::Civilian) {
        return "Civilian";
    }
}

void AddCustomer:: act(WareHouse &wareHouse){
    int customerId = wareHouse.getCustomerCount();
    Customer* newCustomer;
    if (customerType == CustomerType::Soldier){
            newCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
    }
    if (customerType == CustomerType::Civilian){
            newCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(newCustomer);
    complete();
    wareHouse.addAction(this);
}

AddCustomer* AddCustomer:: clone() const{
    return new AddCustomer(*this);
}

string AddCustomer:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "customer " +
    customerName + " " + EnumToCustomerType(customerType) + " " +
    std::to_string(distance) + " " +
    std::to_string(maxOrders)+ " ";

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// PrintOrderStatus
PrintOrderStatus:: PrintOrderStatus(int id) : orderId(id){}

void PrintOrderStatus:: act(WareHouse& wareHouse){
    Order& order = wareHouse.getOrder(orderId);
    if (order.getId() == -1){
        error("Order does not exist");
        return;
    }
    std::cout << order.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "orderStatus " + std::to_string(orderId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// PrintCustomerStatus
PrintCustomerStatus:: PrintCustomerStatus(int id) : customerId(id){}

void PrintCustomerStatus:: act(WareHouse& wareHouse){
    Customer& customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1){
        error("Customer does not exist");
        return;
    }
    string output = 
        "CustomerID: " + std::to_string(customer.getId()) + "\n";

    for (int id : customer.getOrdersIds()){
        Order ord = wareHouse.getOrder(id);
        output += "OrderId: " + std::to_string(ord.getId()) + "\n"
            "OrderStatus: " + ord.EnumToOrderStatus(ord.getStatus()) + "\n";
    }
    output += "numOrdersLeft: " + std::to_string(customer.getMaxOrders()-customer.getNumOrders()) +"\n";
    std::cout << output << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "customerStatus " + std::to_string(customerId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// PrintVolunteerStatus
PrintVolunteerStatus:: PrintVolunteerStatus(int id) : volunteerId(id){}

void PrintVolunteerStatus:: act(WareHouse& wareHouse){
    Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
    if (volunteer.getId() == -1){
        error("Volunteer does not exist");
        return;
    }
    std::cout << volunteer.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintVolunteerStatus* PrintVolunteerStatus:: clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "volunteerStatus " + std::to_string(volunteerId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// PrintActionsLog
PrintActionsLog:: PrintActionsLog(){}

void PrintActionsLog:: act(WareHouse& wareHouse){
    for (BaseAction* act : wareHouse.getActions()){
        std::cout << act->toString() << std::endl;
    }
    std::cout << "\n" << std::endl;
    complete();
    wareHouse.addAction(this);

}

PrintActionsLog* PrintActionsLog:: clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "log ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// Close
Close:: Close(){}

void Close:: act(WareHouse& wareHouse){
    wareHouse.close();
    wareHouse.~WareHouse();
}

Close* Close:: clone() const{
    return new Close(*this);
}

string Close:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "Close ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// BackupWareHouse
BackupWareHouse:: BackupWareHouse(){}

void BackupWareHouse:: act(WareHouse& wareHouse){
    //===============================================
    wareHouse.addAction(this);
}

BackupWareHouse* BackupWareHouse:: clone() const{
    return new BackupWareHouse(*this);
}

string BackupWareHouse:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "BackupWareHouse ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}


//-------------------------------------------------------------------------------------------------------------------
// RestoreWareHouse
RestoreWareHouse:: RestoreWareHouse(){}

void RestoreWareHouse:: act(WareHouse& wareHouse){
    //===============================================
    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse:: clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "RestoreWareHouse ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}