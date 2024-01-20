#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/WareHouse.h"
#include "../include/Volunteer.h"

using namespace std;

WareHouse :: WareHouse(const string &configFilePath):
isOpen(false), customerCounter(0), volunteerCounter(0)
{
    // actionsLog(new vector<BaseAction*>);
    // volunteers(new vector<Volunteer*>);
    // pendingOrders(new vector<Order*>);
    // inProcessOrders(new vector<Order*>);
    // completedOrders(new vector<Order*>);
    // customers(new vector<Customer*>);

    // Parse - 

    ifstream file(configFilePath); // Open the file

    // Check if the file opened successfully
    if (file.is_open()) {

        // Read the contents of the file, word by word in eace line 
        string line; 
        while (getline(file, line)) { 

            // Skip lines that beggin with '#'
            if (line.size() > 0 && line[0] == '#') {
                continue; 
            }

            istringstream iss(line);

            string firstWord;
            string name;
            string orderLimit;
            int id;

            iss >> firstWord >> name;

            // for costumers
            if (firstWord == "customer"){

                // Get all info from line    
                string type;
                string dist;

                iss >> type >> dist >> orderLimit;

                id = customers.size();

                // Call the constructor
                if (type == "civilian"){
                    // call constructor and insert to the customer vector
                }
                if (type == "soldier"){
                    // call constructor and insert to the customer vector
                }
                cout << id << " " << firstWord << " " << name << " " << type << " " << dist << " " << orderLimit << endl;
            }
            
            // for volunteers
            if (firstWord == "volunteer"){
                
                // Get all info from line
                string role;
                string cdORmd;
                string distPerStep;
                string temp;

                iss >> role >> cdORmd;

                id = volunteers.size();

                if (role.find("driver") != std::string::npos){
                    iss >> distPerStep;
                }

                if(iss >> temp && temp != "#"){
                    orderLimit = temp;
                } 

                // Call the constructor
                if (role == "collector"){}
                if (role == "limited_collector"){}
                if (role == "driver"){}
                if (role == "limited_driver"){}

                cout << id << " " << firstWord << " " << name << " " << role << " " << cdORmd << " " << distPerStep << " " << orderLimit << endl;
            }
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

const vector<BaseAction*> & WareHouse :: getActionsLog() const{
    return actionsLog;
}

void WareHouse :: addOrder(Order* order){
    // verify the status!!!
    
}