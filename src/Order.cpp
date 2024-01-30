#include "../include/Order.h"
#include <string>

using std::string;

Order :: Order(int id, int customerId, int distance) :
id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {

}

int Order :: getId() const{
    return id;
}

int Order :: getCustomerId() const{
    return customerId;
}

void Order :: setStatus(OrderStatus status){
    this->status = status;
}

void Order :: setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order :: setDriverId(int driverId){
     this->driverId = driverId;
}

int Order :: getDistance() const{
    return distance;
}
int Order :: getCollectorId() const{
    return collectorId;
}

int Order :: getDriverId() const{
    return driverId;
}

OrderStatus Order :: getStatus() const{
    return status;
}

string Order:: EnumToOrderStatus(OrderStatus type) const {
    if (type == OrderStatus::PENDING) {
        return "PENDING";
    } 
    if (type == OrderStatus::COLLECTING) {
        return "COLLECTING";
    }
    if (type == OrderStatus::DELIVERING) {
        return "DELIVERING";
    } 
    return "COMPLETED";
}

const string Order :: toString() const{
    string driver = "None";
    int driverId = getDriverId();
    if (driverId != -1){
        driver = std::to_string(driverId);
    }

    string collector = "None";
    int collectorId = getCollectorId();
    if (collectorId != -1){
        collector = std::to_string(collectorId);
    }

    string output =
        "OrderId: " + std::to_string(id) +"\n"
        "OrderStatus: " + EnumToOrderStatus(status) + "\n"
        "CustomerID: " + std::to_string(customerId) + "\n"
        "Collector: " + collector + "\n"
        "Driver: " + driver;
    
    return output;
}

const string Order :: closeInfo() const{
    // OrderID: <order_1_id> , CustomerID<customer_1_id> , OrderStatus: <order_1_status>
    string output =
        "OrderId: " + std::to_string(id) + " "
        "CustomerID: " + std::to_string(customerId) + " "
        "OrderStatus: " + EnumToOrderStatus(status);
    
    return output;

}

Order* Order::clone() const {
    return new Order(*this);
}