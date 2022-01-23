#include <iostream>
#include <string>

using namespace std;

// Exception class
class InsufficientChangeException : public exception {
private:
    const char *message;

public:
    explicit InsufficientChangeException(const char *msg){
        this->message = msg;
    }

    const char *what() const throw (){
        return message;
    }
};

class VendingMachineModel {

    // Wariables
private:
int numQuarters = 10;
int numDimes = 10;
int numNickels = 10;
int value = 0;
bool changeButtonPressed = false;

    // Getters and Setters
public:
int getNumQuarters() {
        return numQuarters;
    }

void setNumQuarters(int numQ) {
        this->numQuarters = numQ;
    }

int getNumDimes() {
        return numDimes;
    }

void setNumDimes(int numD) {
        this->numDimes = numD;
    }

int getNumNickels() {
        return numNickels;
    }

void setNumNickels(int numN) {
        this->numNickels = numN;
    }

int getValue() {
        return value;
    }

void setValue(int val) {
        this->value = val;
    }

bool isChangeButtonPressed() {
        return changeButtonPressed;
    }

void setChangeButtonPressed(bool cBP) {
        this->changeButtonPressed = cBP;
    }

    // Private functions for neater code
    // The calculate functions assist the getChange method
private:
int calculateQuarters() {
        int numQuartersOut = 0;
        // if we actually need quarters
        if (this->getValue() / 25 > 0) {
            // either returns enough quarters or returns all quarters in the machine-> Whichever is LESS!
            numQuartersOut = min(this->getValue() / 25, this->getNumQuarters());
            // subtract value owed by numQuartersOut many quarters
            this->setValue(this->getValue() - 25 * numQuartersOut);
        }
        return numQuartersOut;
    }

int calculateDimes() {
        int numDimesOut = 0;
        // if we actually need dimes
        if (this->getValue() / 10 > 0) {
            // either returns enough dimes or returns all dimes in the machine-> Whichever is LESS!
            numDimesOut = min(this->getValue() / 10, this->getNumDimes());
            // subtract value owed by numDimesOut many dimes
            this->setValue(this->getValue() - 10 * numDimesOut);
        }
        return numDimesOut;
    }

int calculateNickels() {
        int numNickelsOut = 0;
        // if we actually need nickels
        if (this->getValue() / 5 > 0) {
            // either returns enough nickels
            numNickelsOut = min(this->getValue() / 5, this->getNumNickels());
            // subtract value owed by numNickelsOut many nickels
            this->setValue(this->getValue() - 5 * numNickelsOut);
        }
        return numNickelsOut;
    }
    
void getChange(int type) {

        // local wariables
        // for output and data management
        int numQuartersOut = 0;
        int numDimesOut = 0;
        int numNickelsOut = 0;
        // hold the amount of coins and the value that exists at the start of this change operation
        int initNumQuarters = this->getNumQuarters();
        int initNumDimes = this->getNumDimes();
        int initNumNickels = this->getNumNickels();
        int initVal = this->getValue();

        // this is the first attempt to find change
        numQuartersOut = calculateQuarters();
        numDimesOut = calculateDimes();
        numNickelsOut = calculateNickels();

        // if we did not get the proper change with the original coin combo, try alternate measures
        // ex-> "q=1,d=5,n=0,v=50,c=true" will need a second attempt and will go into this if block
        if (this->getValue() != 0) {

            // use this to see if we have come to an optimal change solution
            bool success = false;

            // loop until we succeed or fail completely
            while (!success) {

                // reset internal monetary value to what it was originally before trying again
                this->setValue(initVal);

                // intentionally exclude a quarter each time we loop
                if (this->getNumQuarters() > 0) {
                    this->setNumQuarters(this->getNumQuarters() - 1);
                }
                    // after quarters, intentionally exclude a dime each time we loop
                else if (this->getNumDimes() > 0) {
                    this->setNumDimes(this->getNumDimes() - 1);
                }

                // calculate again
                numQuartersOut = calculateQuarters();
                numDimesOut = calculateDimes();
                numNickelsOut = calculateNickels();

                // if this combo works, call succeed and use it
                if (this->getValue() == 0) {
                    success = true;
                }

                    // if we can't get a good combo, even with just nickels, then we're out of luck here->->->
                else if (getNumQuarters() == 0 && getNumDimes() == 0) {
                    throw InsufficientChangeException("Inefficient change in machine. Please call 1-800-COMPSCI for support.");
                }
            }
        }

        // handle output depending on the function we're currently
        switch (type) {
            // lambda
            case 0:
                // print out the change coins that we found
                cout << "Here is your change:" << endl;
                for (int q = 0; q < numQuartersOut; q++) {
                    cout << "q" << endl;
                }
                for (int d = 0; d < numDimesOut; d++) {
                    cout << "d" << endl;
                }
                for (int n = 0; n < numNickelsOut; n++) {
                    cout << "n" << endl;
                }

                if(numQuartersOut == 0 && numDimesOut == 0 && numNickelsOut == 0){
                    cout << "Nothing" << endl;
                }
                // reset the coins and value in the state to what they were before since delta will do the math again after this
                setNumQuarters(initNumQuarters);
                setNumDimes(initNumDimes);
                setNumNickels(initNumNickels);
                setValue(initVal);
                break;
                // delta
            case 1:
                // keep the modified value that the math found for us (always 0)
                // set the number of coins in the machine based on our results to find change
                setNumQuarters(initNumQuarters - numQuartersOut);
                setNumDimes(initNumDimes - numDimesOut);
                setNumNickels(initNumNickels - numNickelsOut);
                // reset the change button since we processed the change
                this->setChangeButtonPressed(false);
                break;
        }
    }

    // Lambda function:
    // Takes in State from this class
    // Outputs string to console
public:
void lambda() {

        // if both coffee and changeButton
        if (this->value >= 100 && this->isChangeButtonPressed()) {
            // output coffees
            int coffeeCap = this->getValue() / 100;
            cout << "You got " << coffeeCap << " coffees!" << endl;
            cout << "Please wait. Change is queued for next tick." << endl;
        }

            // if value >= 100
        else if (this->value >= 100) {
            // output coffees
            int coffeeCap = this->getValue() / 100;
            cout << "You got " << coffeeCap << " coffees!" << endl;
        }

        else if(this->isChangeButtonPressed() && value == 0){
                cout << "System cancelled with no change to give." << endl;
                cout << "Have a good day!" << endl;
                exit(0);
        }

            // if the change button is pressed and we don't have coffees to worry about
        else if (this->isChangeButtonPressed()) {
            // handle change for lambda
            this->getChange(0);
        }

            // if no coffee output, lambda produces nothing
        else
            cout << "Machine produces nothing." << endl;
    }

    // Delta function:
    // Takes in input string
    // Updates the State in this class
void delta(string input) {

        // ACTION BASED ON PREVIOUS LINES

        // boolean value to see if we have given a coffee on this tick
        bool coffeeReceived = false;

        // Handle coffees
        while (this->getValue() / 100 > 0) {
            this->setValue(this->getValue() - 100);
            coffeeReceived = true;
        }

        // Handle change, but only if we haven't given a coffee on this tick
        if (isChangeButtonPressed() && !coffeeReceived) {
            // handle change for delta
            this->getChange(1);
        }

        // ACTIONS BASED ON THIS LINE

        // how many of these inputs are we putting into the machine?
        int numQs = 0;
        int numDs = 0;
        int numNs = 0;
        bool containsC = false;

        // read through the characters
        for (int i = 0; i < input.length(); i++) {
            if (input.at(i) == 'q') {
                numQs++;
            }
            if (input.at(i) == 'd') {
                numDs++;
            }
            if (input.at(i) == 'n') {
                numNs++;
            }
            if (input.at(i) == 'c') {
                containsC = true;
            }
        }

        // update coins
        this->numQuarters += numQs;
        this->numDimes += numDs;
        this->numNickels += numNs;

        // update value
        int addedTotal = (numQs * 25 + numDs * 10 + numNs * 5);
        this->setValue(this->getValue() + addedTotal);

        // update change button pressed
        if (containsC) {
            setChangeButtonPressed(true);
        }
    }
};

int main() {

    // create the vending machine
    auto *diane = new VendingMachineModel();

    // beginning output
    cout << endl;
    cout << "I am the coffee Vending Machine! Please enter your input with respect to the key below:" << endl;
    cout << "--------------------------------------------------------------------------------------------" << endl;
    cout << "q = 1 quarter" << endl;
    cout << "d = 1 dime" << endl;
    cout << "n = 1 nickel" << endl;
    cout << "w = wait" << endl;
    cout << "c = cancel (dispense change)" << endl;
    cout << "--------------------------------------------------------------------------------------------" << endl;
    cout << 
            "Note: you may enter as many of these LOWERCASE ONLY characters as you'd like in each singular input line->" << endl;
    cout << R"(Additionally, you can exit the program by inputting a "c" when the "value" variable in the machine is equal to 0)" << endl;
    cout << endl;

    // Run the simulation
    while (true) {

        // read input here
        cout << "Please enter your input:" << endl;
        string in;
        cin >> in;

        // check the previously existing state and print accordingly
        // S -> Y
        diane->lambda();
        // delta returns an exit state to see if we need to close the program
        // X,S -> S
        diane->delta(in);
        cout << endl;
    }
}

