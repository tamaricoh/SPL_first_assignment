#include "../include/Order.h"
#include <string>

using std::string;

Order :: Order(int id, int customerId, int distance) :
id(id), customerId(customerId), distance(distance), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {

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

const string Order :: toString() const{
    // Form - <OrderId> <OrderStatus> <CustomerId> <CollectorId> <DriverId>
    // return std::to_string(id) + " " +
    //     std::to_string(static_cast<int>(status)) + " " +
    //     std::to_string(customerId) + " " +
    //     std::to_string(collectorId) + " " +
    //     std::to_string(driverId);

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
        "OrderStatus: " + std::to_string(static_cast<int>(status)) + "\n"
        "CustomerID: " + std::to_string(customerId) + "\n"
        "Collector: " + collector + "\n"
        "Driver: " + driver + "\n";
    
    return output;
}