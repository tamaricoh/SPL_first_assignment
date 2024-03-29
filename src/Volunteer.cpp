#include "../include/Volunteer.h"

//-------------------------------------------------------------------------------------------------------------------
//Volunteer
Volunteer::Volunteer(int id, const string &name) :
completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), completeInCurrentStep(false), reachedMaxOrder(false),
id(id), name(name), typeStr(""){}

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
    return(activeOrderId != NO_ORDER);
}

string Volunteer:: type(){
    return typeStr;
}

void Volunteer:: setType(string type){
    typeStr = type;
}

bool Volunteer:: getCompleteInCurrentStep() const{
    return completeInCurrentStep;
}

bool Volunteer::getReachedMaxOrder() const{
    return reachedMaxOrder;
}

void Volunteer:: setReachedMaxOrder(bool reachedLimit){
    reachedMaxOrder = reachedLimit;
}

//-------------------------------------------------------------------------------------------------------------------
//CollectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string& name, int coolDown) :
Volunteer(id, name), coolDown(coolDown), timeLeft(0){
    setType("Collector");
}

CollectorVolunteer* CollectorVolunteer:: clone() const{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    completeInCurrentStep = false;
    if (isBusy() && decreaseCoolDown()){ // if the volunteer has completed the order in this step,
        completeInCurrentStep = true;
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
    // Form - <volunteerId> <isBusy> <activeOrderId> <timeLeft>
    string orderId = "None";
    string timeLeft = "None";
    string isBus = isBusy()? "True" : "False";

    if (isBus == "True") {
        orderId = std::to_string(getActiveOrderId());
        timeLeft = std::to_string(getTimeLeft());
    }
    
    string output = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + isBus + "\n"
        "OrderID: " + orderId + "\n"
        "TimeLeft: " + timeLeft + "\n"
        "OrdersLeft: No Limit";

    return output;
}


//-------------------------------------------------------------------------------------------------------------------
//LimitedCollectorVolunteer
LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string& name, int coolDown ,int maxOrders) :
CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){
    if (maxOrders == 0){
        setReachedMaxOrder(true);
    }
}

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
    if (ordersLeft == 0){
        setReachedMaxOrder(true);
    }
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
    string orderId = "None";
    string timeLeft = "None";
    string isBus = isBusy()? "True" : "False";

    if (isBus == "True") {
        orderId = std::to_string(getActiveOrderId());
        timeLeft = std::to_string(getTimeLeft());
    }
    
    string output = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + isBus + "\n"
        "OrderID: " + orderId + "\n"
        "TimeLeft: " + timeLeft + "\n"
        "OrdersLeft: " + std::to_string(getNumOrdersLeft());

    return output;
}

//-------------------------------------------------------------------------------------------------------------------
//DriverVolunteer
DriverVolunteer:: DriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep) :
Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(maxDistance){
    setType("Driver");
}

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
    string test;
    (distanceLeft <= 0)? test = "true" : test = "false";
    distanceLeft -= getDistancePerStep();
    return (distanceLeft <= 0);
} 

bool DriverVolunteer:: hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return (!isBusy() && maxDistance >= order.getDistance());
} 

void DriverVolunteer:: acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
} 
void DriverVolunteer:: step() {
    completeInCurrentStep = false;
    if (isBusy() && decreaseDistanceLeft()){ // if the volunteer has completed the order in this step,
        completeInCurrentStep = true;
        distanceLeft = 0;
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer:: toString() const {
    string orderId = "None";
    string timeLeft = "None";
    string isBus = isBusy()? "True" : "False";

    if (isBus == "True") {
        orderId = std::to_string(getActiveOrderId());
        timeLeft = std::to_string(getDistanceLeft());
    }
    
    string output = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + isBus + "\n"
        "OrderID: " + orderId + "\n"
        "TimeLeft: " + timeLeft + "\n"
        "OrdersLeft: No Limit";

    return output;
}

//-------------------------------------------------------------------------------------------------------------------
//LimitedDriverVolunteer
LimitedDriverVolunteer:: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){
    if (maxOrders == 0){
        setReachedMaxOrder(true);
    }
}

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
    if (ordersLeft == 0){
        setReachedMaxOrder(true);
    }
    DriverVolunteer :: acceptOrder(order);
}

string LimitedDriverVolunteer:: toString() const {
    // Form - <volunteerId> <isBusy> <activeOrderId> <distanceLeft> <ordersLeft> 
    string orderId = "None";
    string timeLeft = "None";
    string isBus = isBusy()? "True" : "False";

    if (isBus == "True") {
        orderId = std::to_string(getActiveOrderId());
        timeLeft = std::to_string(getDistanceLeft());
    }
    
    string output = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + isBus + "\n"
        "OrderID: " + orderId + "\n"
        "TimeLeft: " + timeLeft + "\n"
        "OrdersLeft: " + std::to_string(getNumOrdersLeft());

    return output;
}