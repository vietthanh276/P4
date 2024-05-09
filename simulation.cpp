#include "shopQueue.h"
#include "customer.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

using std::cin;
using std::cout;
using std::ofstream;
using std::string;
using std::vector;

int main(int argc, char* argv[]){
/**
 * Check for the number of command line arguments
*/
    if (argc != 7)
    {
        puts("Error: Wrong number of arguments!");
        puts("Usage: ./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>");
        return 0;
    }

/**
 * Save the command line arguments in variables
*/
    int seed = atoi(argv[1]);
    srand(seed);
    int total_min = atoi(argv[2]);
    float arrival_prob = stof(argv[3]);
    float order_prob = stof(argv[4]);
    int num_queues = atoi(argv[5]);
    string outputFile = argv[6];

    int id = 1;
    int counter = 0;   
    vector<int> completedCashier (num_queues);

    puts("Welcome to Gompei Coffee Shop!");
    cout << "-- # of Checkout Queues: " << num_queues << " --\n"; 
    
/**
 * Create empty queues
*/
    vector<ShopQueue> shopQueues(num_queues);

/**
 * Add the first cistomer at time = 0
*/
    cout << "New customer at t = 0." << endl;
    shopQueues[0].addNewCustomer(0,0,0,0);

/**
 * Create a vector of Customers pointers to store the completed customer data
*/
    
    vector<shared_ptr<Customer>> completed;
    vector<int> cashier(num_queues);
    int total = 0;

    for(int min = 0; min < total_min; min++){
        // Calculate a random value between 0 and 1
        float random = ((float) rand() / (float) (RAND_MAX));
        // Find the shortest queue
        if(random < arrival_prob){
            int shortestQueue = 0;
            for(int i = 0; i < num_queues; i++){
                if(shopQueues[i].getLength() < shopQueues[shortestQueue].getLength()){
                    shortestQueue = i;
                }
            }
            cout<< "New customer at t = " << min << "."<< endl;

            shopQueues[shortestQueue].addNewCustomer(min, min, 0, id);
            id++;
        }
    
/**
 * Check to see if the front customer is done
*/
        for(int queueNum = 0; queueNum < num_queues; queueNum++){
            if(!shopQueues[queueNum].isEmpty()){

                float randNum = ((float) rand() / (float)(RAND_MAX));
                if(randNum < order_prob){
                    cashier[queueNum]++;
                    // Remove the customer from the queue
                    shared_ptr<Customer> completedCustomer = shopQueues[queueNum].removeFrontCustomer();
                    // Add to the completed vector
                    completed.push_back(completedCustomer);
                    // Modify the completedTime variable
                    completedCustomer -> completedTime = min;
                    
                    // Every time a customer is finished, print:
                    cout<< "Customer #" << completedCustomer -> ID 
                    << " was served from t = "<< completedCustomer -> startTime 
                    << " to "<< completedCustomer -> completedTime<<"."<<endl;

                    if(!shopQueues[queueNum].isEmpty()){
                        // Set the start time of the next customer
                        shared_ptr<Customer> nextCustomer = shopQueues[queueNum].peekFrontCustomer();
                        nextCustomer -> startTime = min;
                    }
                }
            }                 
        }
    }


    // Print the data in the completed vector to a file
    ofstream output;
    output.open(outputFile);
    if(output.is_open()){
        output << setw(8) << "ID" << setw(8) << "Arrive" << setw(8) << "Start" << setw(8) << "Leave" << endl;
        for(auto& customer : completed){
            // Use setw(8) between each output item
            output<< setw(8) << customer -> ID 
            << setw(8) << customer -> arrivalTime 
            << setw(8) << customer -> startTime 
            << setw(8) << customer -> completedTime << endl;
        }
        output.close();
    } else {
        cout << "Error!" << "\n";
    }

    // Print the current length of the queue to the terminal.
    for(int i = 0; i < num_queues; i++){
        cout<<"After "<< total_min <<" minutes, "<< shopQueues[i].getLength() <<" customers remain in queue #"<< i << "\n";
    }

    // Print a summary of how many customers each cashier served.
    for(int i = 0; i < num_queues; i++){
        cout << "Cashier " << i << " served " << cashier[i] << " customers\n";  
    }

}