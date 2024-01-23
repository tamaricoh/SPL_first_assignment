#include "../include/Action.h"

BaseAction:: BaseAction(){

}

ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction:: complete(){
    status = 0;
}

void BaseAction:: error(string errorMsg){
    status = 1;
    this -> errorMsg = errorMsg;
    
}

string BaseAction:: getErrorMsg() const{
    return errorMsg;
}



// SimulateStep
SimulateStep :: SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep :: act(WareHouse &wareHouse){

}

std:: string SimulateStep:: toString() const{

}
SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}



// Order
AddOrder:: AddOrder(int id) : customerId(id){}

void AddOrder:: act(WareHouse &wareHouse){
    // update error msg, update costeomr type
    int OrderId = wareHouse.orderCounter;
    int distance = wareHouse.getCustomer(id).getCustomerDistance();
    Order newOrder = new Order(OrderId, customerId, distance);
    newOrder.setStatus(0);
    pendingOrders.push_back(newOrder);
}

AddOrder* AddOrder:: clone() const{
    return new AddOrder(*this);
}

string AddOrder:: toString() const{
    // to be continued
}

AddCustomer:: AddCustomer(string customerName, string customerType, int distance, int maxOrders) : customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders) {}
void AddCustomer:: act(WareHouse &wareHouse){
    //to be continsendnudes
}
AddCustomer:: *clone(){
    return new AddCustomer(*this);
}
strimg AddCustomer:: toString(){}