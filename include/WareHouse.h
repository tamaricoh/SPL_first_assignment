#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();

        // new func
        int getOrderCount() const;
        int getCustomerCount() const;
        void parse (const string &configFilePath);
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* Volunteer);
        void cleanUp();
        void step();
        bool findCollector (Order& order) const;
        bool findDriver (Order& order) const;
        bool isNumber(const std::string& str);

        // rule of 5
        virtual ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse& operator=(const WareHouse &other);

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

        // new member
        int orderCounter; //For assigning unique order IDs
        Customer* noCustomer;
        Volunteer* noVolunteer;
        Order* noOrder;
};