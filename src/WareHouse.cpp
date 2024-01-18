#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/WareHouse.h"
#include "../include/Volunteer.h"

using namespace std;

WareHouse :: WareHouse(const string &configFilePath){

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
                if (type == "civilian"){}
                if (type == "soldier"){}
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
};