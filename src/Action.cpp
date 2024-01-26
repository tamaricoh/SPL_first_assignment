#include "../include/Action.h"
#include "../include/WareHouse.h"
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
    cout << "Error: " << errorMsg << endl;
    
}

string BaseAction:: getErrorMsg() const{
    return errorMsg;
}


//-------------------------------------------------------------------------------------------------------------------
// SimulateStep
SimulateStep :: SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep :: act(WareHouse &wareHouse){
    //===============================================
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}

std:: string SimulateStep:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "SimulateStep " + std::to_string(numOfSteps)+ " " ;

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// AddOrder
AddOrder:: AddOrder(int id) : customerId(id){}

void AddOrder:: act(WareHouse &wareHouse){
    if (customerId > wareHouse.getCustomerCount()){
        error("Cannot place this order");
        return;
    }
    Customer& orderCustomer = wareHouse.getCustomer(customerId);
    if ( orderCustomer.getMaxOrders() == orderCustomer.getNumOrders()){
        error("Cannot place this order");
        return;
    }
    int OrderId = wareHouse.getOrderCount();
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
    string output = "AddOrder " + std::to_string(customerId)+ " ";

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
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
}

AddCustomer* AddCustomer:: clone() const{
    return new AddCustomer(*this);
}

string AddCustomer:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "AddCustomer " +
    customerName + " " + EnumToCustomerType(customerType) + " " +
    std::to_string(distance) + " " +
    std::to_string(maxOrders)+ " ";

    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
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
    cout << order.toString() << endl;
    // stringstream ss(order.toString());
    // string word;
    // vector<string> labels = {
    //     "OrderId: ",
    //     "OrderStatus: ",
    //     "CustomerID: ",
    //     "Collector: ",
    //     "Driver: "
    // };

    // for (string label : labels) {
    //     ss >> word;
    //     if (word == "-1"){
    //         word = "None";
    //     }
    //     cout << label << word << endl;
    // }
}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "PrintOrderStatus " + std::to_string(orderId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// PrintCustomerStatus
PrintCustomerStatus:: PrintCustomerStatus(int id) : customerId(customerId){}

void PrintCustomerStatus:: act(WareHouse& wareHouse){
    //===============================================
}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "PrintCustomerStatus " + std::to_string(customerId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// PrintVolunteerStatus
PrintVolunteerStatus:: PrintVolunteerStatus(int id) : volunteerId(id){}

void PrintVolunteerStatus:: act(WareHouse& wareHouse){
    Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
    if (volunteer.getId() == -1){
        error("Order does not exist");
        return;
    }
    cout << volunteer.toString() << endl;
    // stringstream ss(volunteer.toString());
    // string word;
    // string helper;
    
    // ss >> word;
    // cout << "VolunteerID: " << word << endl;
    // ss >> word;
    // cout << "isBusy: " << word << endl;
    // helper = word;
    // ss >> word;
    // if (helper == "False"){
    //     word = "None";
    // }
    // cout << "Orderid: " << word << endl;
    // helper = word;
    // ss >> word;
    // if (helper == "-1"){
    //     word = "None";
    // }
    // cout << "timeLeft: " << word << endl;
    // (ss >> word)? cout << "ordersLeft: " << word << endl: cout << "ordersLeft: No Limit" << endl;
    
    
}

PrintVolunteerStatus* PrintVolunteerStatus:: clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "PrintVolunteerStatus " + std::to_string(volunteerId) + " ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// PrintActionsLog
PrintActionsLog:: PrintActionsLog(){}

void PrintActionsLog:: act(WareHouse& wareHouse){
    //===============================================
}

PrintActionsLog* PrintActionsLog:: clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog:: toString() const{
    //===============================================
    // <action_1_name> <action_1_args> <action_1_status>
    // string output = "PrintVolunteerStatus ";
    
    // if (getStatus() == ActionStatus::COMPLETED){
    //     return output + "Completed";
    // }
    // return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// Close
Close:: Close(){}

void Close:: act(WareHouse& wareHouse){
    //===============================================
}

Close* Close:: clone() const{
    return new Close(*this);
}

string Close:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "Close ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// BackupWareHouse
BackupWareHouse:: BackupWareHouse(){}

void BackupWareHouse:: act(WareHouse& wareHouse){
    //===============================================
}

BackupWareHouse* BackupWareHouse:: clone() const{
    return new BackupWareHouse(*this);
}

string BackupWareHouse:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "BackupWareHouse ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}


//-------------------------------------------------------------------------------------------------------------------
// RestoreWareHouse
RestoreWareHouse:: RestoreWareHouse(){}

void RestoreWareHouse:: act(WareHouse& wareHouse){
    //===============================================
}

RestoreWareHouse* RestoreWareHouse:: clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse:: toString() const{
    // <action_1_name> <action_1_args> <action_1_status>
    string output = "RestoreWareHouse ";
    
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "Completed";
    }
    return output + "Error: " + getErrorMsg();
}