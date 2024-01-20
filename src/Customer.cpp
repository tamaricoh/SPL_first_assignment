#include "../include/Customer.h"
#include "../include/WareHouse.h"

Customer :: Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders){
    // when calling this constructor, the output will ve inserted to the costumer vactor
}

const string& Customer :: getName() const{
    return name;
}

int Customer :: getId() const{
    return id;
}

int Customer :: getCustomerDistance() const{
    return locationDistance;
}

int Customer :: getMaxOrders() const{
    return maxOrders;
}

int Customer :: getNumOrders() const{
    return ordersId.size();
}

bool Customer :: canMakeOrder() const{
    return (getNumOrders() < getMaxOrders());
}

vector<int>& Customer :: getOrders() const{
    // To be continued
}

int Customer :: addOrder(int orderId){
    if (canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    } else {
        return -1;
    }
}

SoldierCustomer :: SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){}
CivilianCustomer :: CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){}