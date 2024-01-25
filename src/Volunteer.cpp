#include "../include/Volunteer.h"

//Volunteer

Volunteer::Volunteer(int id, const string &name) :
id(id), name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) {}

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

bool Volunteer::isBusy() const{
    return(activeOrderId != completedOrderId);
}


//CollectorVolunteer

CollectorVolunteer::CollectorVolunteer(int id, const string& name, int coolDown) :
Volunteer(id, name), coolDown(coolDown), timeLeft(0){}

CollectorVolunteer* CollectorVolunteer:: clone() const{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (isBusy() && decreaseCoolDown()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return (timeLeft == 0);
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return (!isBusy());
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = getCoolDown();
    }

string CollectorVolunteer::toString() const {
    // Form - <volunteerId> <volunteerName> <isBusy> <activeOrderId> <timeLeft> <coolDown>
    return std::to_string(getId()) + " " +
        getName()+ " " +
        std::to_string(isBusy()) + " " +
        std::to_string(getActiveOrderId()) + " " +
        std::to_string(getTimeLeft()) + " " +
        std::to_string(getCoolDown()) + " ";
}


//LimitedCollectorVolunteer

LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string& name, int coolDown ,int maxOrders) :
CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer:: hasOrdersLeft() const {
    return (ordersLeft > 0);
}

bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const {
    return (CollectorVolunteer::canTakeOrder(order) & hasOrdersLeft());
}

void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
    ordersLeft--;
    CollectorVolunteer::acceptOrder(order);
    
}

int LimitedCollectorVolunteer:: getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer:: toString() const {
    // Form - <volunteerId> <volunteerName> <isBusy> <activeOrderId> <timeLeft> <coolDown> <ordersLeft> 
    return (CollectorVolunteer::toString() +
     " " + std::to_string(getNumOrdersLeft()));
}


//DriverVolunteer

DriverVolunteer:: DriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep) :
Volunteer(id, name), maxDistance(maxDistance), distanceLeft(maxDistance), distancePerStep(distancePerStep){}

DriverVolunteer* DriverVolunteer:: clone() const{
    return new DriverVolunteer(*this);
}

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
    return (distanceLeft <= 0);
} 

bool DriverVolunteer:: hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return (!isBusy() & maxDistance <= order.getDistance());
} 

void DriverVolunteer:: acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = maxDistance;
} 
void DriverVolunteer:: step() {
    if (isBusy() && decreaseDistanceLeft()){
        distanceLeft = 0;
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer:: toString() const {
    // Form - <volunteerId> <volunteerName> <isBusy> <activeOrderId> <distanceLeft> <maxDistance>
    return std::to_string(getId()) + " " +
        std::to_string(isBusy()) + " " +
        std::to_string(getActiveOrderId()) + " " +
        std::to_string(getDistanceLeft()) + " " +
        std::to_string(getMaxDistance()) + " ";
}


//LimitedDriverVolunteer

LimitedDriverVolunteer:: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

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
    return (DriverVolunteer:: canTakeOrder(order) & hasOrdersLeft());
}

void LimitedDriverVolunteer:: acceptOrder(const Order &order) {
    ordersLeft--;
    DriverVolunteer :: acceptOrder(order);
}

string LimitedDriverVolunteer:: toString() const {
    // Form - <volunteerId> <volunteerName> <isBusy> <activeOrderId> <distanceLeft> <maxDistance> <ordersLeft> 
    return (DriverVolunteer :: toString() + " " + std::to_string(getNumOrdersLeft()));
}