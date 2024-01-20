#include "../include/Volunteer.h"

//Volunteer

Volunteer::Volunteer(int id, const string &name) :
id(id), name(name), completedOrderId(0), activeOrderId(0) {}

int Volunteer::getId() const {
    return id;
}

const string &Volunteer:: getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{}

bool Volunteer::hasOrdersLeft() const{}

void Volunteer::acceptOrder(const Order &order){}

void Volunteer::step(){}



//CollectorVolunteer

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown) :
Volunteer(id, name), coolDown(coolDown), timeLeft(0){}

CollectorVolunteer* CollectorVolunteer:: clone() const{
    // to complete
}

void CollectorVolunteer::step() {
    // to complete
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return timeLeft == 0;
    //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
}

bool CollectorVolunteer::hasOrdersLeft() const {}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {}

void CollectorVolunteer::acceptOrder(const Order &order) {}

string CollectorVolunteer::toString() const {}


//LimitedCollectorVolunteer

LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders) :
CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const {}

bool LimitedCollectorVolunteer:: hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const {}

void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {}

int LimitedCollectorVolunteer:: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer:: toString() const {}


//DriverVolunteer

DriverVolunteer:: DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) :
Volunteer(id, name), maxDistance(maxDistance), distanceLeft(maxDistance), distancePerStep(distancePerStep){}

DriverVolunteer* DriverVolunteer:: clone() const{}

int DriverVolunteer:: getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer:: getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer:: getDistancePerStep() const{
    return distancePerStep;
} 

bool DriverVolunteer:: decreaseDistanceLeft(){
    distanceLeft--;
    return distanceLeft <= 0;
    //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
} 

bool DriverVolunteer:: hasOrdersLeft() const {}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    // Signal if the volunteer is not busy and the order is within the maxDistance
} 

void DriverVolunteer:: acceptOrder(const Order &order) {
    // Reset distanceLeft to maxDistance
} 
void DriverVolunteer:: step() {
    // Decrease distanceLeft by distancePerStep
}

string DriverVolunteer:: toString() const {}


//LimitedDriverVolunteer

LimitedDriverVolunteer:: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer:: hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const {
    // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
}

void LimitedDriverVolunteer:: acceptOrder(const Order &order) {
    // Reset distanceLeft to maxDistance and decrease ordersLeft
}

string LimitedDriverVolunteer:: toString() const {}