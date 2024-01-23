#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Order.h"
#include "../include/Action.h"

using namespace std;

WareHouse :: WareHouse(const string &configFilePath):
isOpen(false),
actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    parse(configFilePath);
}

void WareHouse :: parse (const string &configFilePath){

    ifstream file(configFilePath); // Open the file

    // Check if the file opened successfully
    if (file.is_open()) {

        string line;
        string firstWord;
        string name;
        string orderLimit;
        string type; // or role
        string dist;
        string distPerStep;
        string temp; // Becaue orderLimit is optional in volunteers
        int id;

        while (getline(file, line)) { 

            // Skip lines that beggin with '#'
            if ((line.size() == 0) | ( line.size() > 0 && line[0] == '#')) {
                continue; 
            }

            istringstream iss(line);
            iss >> firstWord >> name >> type >> dist;

            // for costumers
            if (type == "civilian"){
                iss >> orderLimit;
                id = customers.size();
                customers.push_back(new CivilianCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "soldier"){
                iss >> orderLimit;
                id = customers.size();
                customers.push_back(new SoldierCustomer(id, name, stoi(dist), stoi(orderLimit)));
            }

            
            // for volunteers
            if (type.find("driver") != std::string::npos){
                iss >> distPerStep;
            }

            if(iss >> temp && temp != "#"){
                orderLimit = temp;
            } 

            if (type == "collector"){
                id = volunteers.size();
            }
            if (type == "limited_collector"){
                id = volunteers.size();
            }
            if (type == "driver"){
                id = volunteers.size();
            }
            if (type == "limited_driver"){
                id = volunteers.size();
            }

            cout << id << " " << firstWord << " " << name << " " << type << " " << dist << " " << distPerStep << " " << orderLimit << endl;
            
        } 
    }
}

void WareHouse :: start(){
    if (!isOpen) {
        isOpen = true;
        cout << "Warehouse is open!" << endl;
        // the program waits for the user to enter an action to execute
    } else {
        cout << "The Warehouse is already open." << endl;
    }
}

const vector<BaseAction*> & WareHouse :: getActions() const{
    return actionsLog;
}

void WareHouse :: addOrder(Order* order){
    // verify the status!!!

}