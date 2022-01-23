#include <iostream>
#include <vector>
#include <fstream>
#include <cfloat>

using namespace std;

// Exception class (just like in previous C++ assignment)
class InsufficientChangeException : public exception {
private:
    const char *message;
public:
    explicit InsufficientChangeException(const char *msg) {
        this->message = msg;
    }

    const char *what() const throw() {
        return message;
    }
};

// Vending machine event class
class VendingMachineEvent {

    // private wariables
private:
    double time;
    int cValue;
    char coin;
    bool isOutput;

    // Public methods
public:

    // Constructors
    // Input events
    VendingMachineEvent(double t, int c, char co) {
        time = t;
        cValue = c;
        coin = co;
        isOutput = false;
    }
    // Output events self created by the program based on the output of ta() (notice a coin "input"
    // on an output event is simply 'o')
    VendingMachineEvent(double t, int c) {
        time = t;
        cValue = c;
        coin = 'o';
        isOutput = true;
    }

    // getters and setters
    double getTime() {
        return time;
    }
    int getCValue() {
        return cValue;
    }
    char getCoin() {
        return coin;
    }
    bool getIsOutput() {
        return isOutput;
    }
};

// Vending machine model class
class VendingMachineModel {

private:
    // Private Wariables
    int numQuarters;
    int numDimes;
    int numNickels;
    int value;
    double time;

    // internally stored event "queue" (organized in our own way later on) (this manner of organizing the events was
    // approved in the Java version)
    vector<VendingMachineEvent> *vMEList;

    // Private functions for neater code
    // The calculate functions assist the getChange method
    int calculateQuarters() {
        int numQuartersOut = 0;
        // if we actually need quarters
        if (this->getValue() / 25 > 0) {
            // either returns enough quarters or returns all quarters in the machine. Whichever is LESS!
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
            // either returns enough dimes or returns all dimes in the machine. Whichever is LESS!
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

    string getChange(int type, int numCoffeesServedPrior) {

        // output string will be built upon for the lambda output as we go through
        string output;

        // local wariables
        // for output and data management
        int numQuartersOut;
        int numDimesOut;
        int numNickelsOut;
        // hold the amount of coins and the value that exists at the start of this change operation for later reference
        int initNumQuarters = this->getNumQuarters();
        int initNumDimes = this->getNumDimes();
        int initNumNickels = this->getNumNickels();
        int initVal = this->getValue();

        // a modified value that accounts for the coffees made prior to this method call
        int accountedVal = this->getValue() - (100 * numCoffeesServedPrior);
        this->setValue(accountedVal);

        // this is the first attempt to find change
        numQuartersOut = calculateQuarters();
        numDimesOut = calculateDimes();
        numNickelsOut = calculateNickels();

        // a modified value to consider the coffees made prior to this
        this->setValue(accountedVal);

        // if we did not get the proper change with the original coin combo, try alternate measures
        // ex. "q=1,d=5,n=0,v=50,c=true" will need a second attempt and will go into this if block
        if (this->getValue() != 0) {

            // use this to see if we have come to an optimal change solution
            bool success = false;

            // loop until we succeed or fail completely
            while (!success) {

                // reset internal monetary value to what it was originally before trying again
                this->setValue(accountedVal);

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

                    // if we can't get a good combo, even with just nickels, then we're out of luck here...
                else if (getNumQuarters() == 0 && getNumDimes() == 0) {
                    throw InsufficientChangeException(
                            "Inefficient change in machine. Please call 1-800-COMPSCI for support.");
                }
            }
        }

        // handle output depending on the function we're currently
        switch (type) {
            // lambda
            case 0:
                // print out the change coins that we found
                for (int q = 0; q < numQuartersOut; q++) {
                    output += "q";
                }
                for (int d = 0; d < numDimesOut; d++) {
                    output += "d";
                }
                for (int n = 0; n < numNickelsOut; n++) {
                    output += "n";
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
                setValue(0);
                break;
        }
        return output;
    }

    // Public methods for external calling
public:

    // Constructor
    VendingMachineModel() {
        numQuarters = 0;
        numDimes = 0;
        numNickels = 0;
        value = 0;
        vMEList = new vector<VendingMachineEvent>();
    }

    // Getters and Setters
    int getNumQuarters() {
        return numQuarters;
    }

    void setNumQuarters(int nQ) {
        this->numQuarters = nQ;
    }

    int getNumDimes() {
        return numDimes;
    }

    void setNumDimes(int nD) {
        this->numDimes = nD;
    }

    int getNumNickels() {
        return numNickels;
    }

    void setNumNickels(int nN) {
        this->numNickels = nN;
    }

    int getValue() {
        return value;
    }

    void setValue(int v) {
        this->value = v;
    }

    double getTime() {
        return time;
    }

    vector<VendingMachineEvent> *getVMEList() {
        return this->vMEList;
    }

    // Lambda function
    string lambda() {

        string output;

        // show the top event information before showing machine output
        cout << this->getVMEList()->at(0).getTime() << "," << this->getVMEList()->at(0).getCValue() << " ";

        // output coffees if possible
        int coffeeCap = this->getValue() / 100;
        if (coffeeCap >= 1)
            cout << 'c';
        // handle change for lambda with respect to coffees made in the line above
        output += this->getChange(0, coffeeCap);
        return output;
    }

    // this method occurs when the time advance function arrives at 0, or in other words, 2 seconds of waiting has passed
    // this is when we do the math for the change in the machine
    // does not receive input variable here
    void deltaInternal() {
        // serve coffees and get the change needed
        this->value -= (this->getValue() / 100) * 100;
        // since coffees served is a lambda component and not a delta component, we pass in a 0 for the second argument
        getChange(1, 0);
    }

    // this method occurs when we see user input. We read in that input as well as the amount of time that has passed since
    // the time advance function began (even though we don't use timeElapsed. It is a formality for now)
    void deltaExternal(double timeElapsed, char input) {
        switch (input) {
            case 'q':
                this->numQuarters++;
                this->value += 25;
                break;
            case 'n':
                this->numNickels++;
                this->value += 5;
                break;
            case 'd':
                this->numDimes++;
                this->value += 10;
                break;
            default:
                break;
        }
    }

    // this method occurs when we see user input at exactly 2 seconds passed.
    // We read in that input, but assume a time elapsed of 2 seconds
    void deltaConfluent(char input) {
        // serve coffees and get the change needed
        this->value -= (this->getValue() / 100) * 100;
        switch (input) {
            case 'q':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this->numQuarters++;
                this->value = 25;
                break;
            case 'n':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this->numNickels++;
                this->value += 5;
                break;
            case 'd':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this->numDimes++;
                this->value += 10;
                break;
            default:
                break;
        }
    }

    // time advancement function handles the amount of time stamped for planned output events
    // if an output is planned with respect to ta() -> 2, then we will plan on an output event
    // 2 seconds after the corresponding deltaExternal or deltaConfluent call.
    // otherwise, a time of INFINITY will induce an output that will never actually be used in our event queue
    void timeAdvancement() {
        if (this->value == 0) {
            this->time = DBL_MAX;
        } else {
            this->time = 2;
        }
    }
};

int main() {

    // create the vending machine
    auto *frasier = new VendingMachineModel();

    // INPUT HANDLING BEGINS HERE --------------------------------------------------------------------------------------

    string line;
    ifstream inputFile("../A4Input.txt");
    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            double elapsedTime = stod(line.substr(0, line.find(',', 0)));
            int cVal = stoi(line.substr(line.find(',', 0) + 1, line.find(' ', 0) - line.find(',', 0) + 1));
            char coin = line.at(line.length() - 1);

            // add the events into an ArrayList to work like a queue, where the newest event is placed at the end of the collection.
            // To read this like a queue, we will only read from the first index in the main program.
            VendingMachineEvent vme = * new VendingMachineEvent(elapsedTime, cVal, coin);
            frasier->getVMEList()->insert(frasier->getVMEList()->end(), vme);
        }
        inputFile.close();

    // SIMULATION BEGINS HERE ------------------------------------------------------------------------------------------

        // boolean tracked to end the program when necessary
        bool end = false;
        while (!end) {

            // first, look to see if this event is the only existing event
            if (frasier->getVMEList()->size() == 1) {
                // if this is an output event
                if (frasier->getVMEList()->at(0).getIsOutput()) {
                    // call lambda as well as delta internal, since we are outputting, as well as adjusting the state
                    // for said output
                    cout << endl;
                    cout << frasier->lambda() << endl;
                    frasier->deltaInternal();
                    // we can end the program here since we know there won't be another event after this one
                    end = true;
                } else {
                    // otherwise, treat this like an input event and insert a new output event in the next slot
                    // we'll handle this newly created event on the next iteration
                    frasier->deltaExternal(frasier->getVMEList()->at(0).getTime(),
                                           frasier->getVMEList()->at(0).getCoin());
                    // call timeAdvancement since we have a new input to account for
                    frasier->timeAdvancement();
                    VendingMachineEvent vme = *new VendingMachineEvent(frasier->getVMEList()->at(0).getTime() + 2,
                                                                       frasier->getVMEList()->at(0).getCValue());
                    frasier->getVMEList()->insert(frasier->getVMEList()->begin() + 1, vme);
                }
            }
            // otherwise, we won't have to consider ending the program just yet
            else {
                // compare this element to the next one, since our actions depend on the current event and the next one
                // with respect to it
                // if the first event is scheduled to occur before the second
                if (frasier->getVMEList()->at(0).getTime() < frasier->getVMEList()->at(1).getTime()) {
                    if (frasier->getVMEList()->at(0).getIsOutput()) {
                        // call a delta internal since we can output at this point
                        // we will call lambda here as well
                        cout << endl;
                        cout << frasier->lambda() << endl;
                        frasier->deltaInternal();
                    } else {
                        // otherwise, treat this like an input event and insert a new output event in the next slot
                        // we'll handle this newly created event on the next iteration
                        frasier->deltaExternal(frasier->getVMEList()->at(0).getTime(),
                                               frasier->getVMEList()->at(0).getCoin());
                        frasier->timeAdvancement();
                        VendingMachineEvent vme = *new VendingMachineEvent(
                                (frasier->getVMEList()->at(0).getTime() + frasier->getTime()),
                                frasier->getVMEList()->at(0).getCValue());
                        frasier->getVMEList()->insert(frasier->getVMEList()->begin() + 1, vme);
                    }
                }
                // if the second event is scheduled to occur before the first
                else if (frasier->getVMEList()->at(0).getTime() > frasier->getVMEList()->at(1).getTime()) {
                    // remove this first event since we know that it can only ever be an output event placed before a preceding input event
                    // ex. "2.4,0 o" has no use remaining in the Queue if we must instead call "1.2,0 q"
                    frasier->getVMEList()->erase(frasier->getVMEList()->begin());
                    // treat this like an input event and insert a new output event in the next slot
                    // we'll handle this newly created event on the next iteration
                    frasier->deltaExternal(frasier->getVMEList()->at(0).getTime(),
                                           frasier->getVMEList()->at(0).getCoin());
                    frasier->timeAdvancement();
                    VendingMachineEvent vme = *new VendingMachineEvent(
                            (frasier->getVMEList()->at(0).getTime() + frasier->getTime()),
                            frasier->getVMEList()->at(0).getCValue());
                    frasier->getVMEList()->insert(frasier->getVMEList()->begin() + 1, vme);
                }
                    // if both events occur at the exact same time
                else {
                    // if the second line is the input event
                    if (frasier->getVMEList()->at(0).getIsOutput()) {
                        // call lambda and deltaConfluent since we are inputting and outputting at the same time
                        cout << endl;
                        cout << frasier->lambda() << endl;
                        frasier->deltaConfluent(frasier->getVMEList()->at(1).getCoin());
                        frasier->timeAdvancement();
                        VendingMachineEvent vme = *new VendingMachineEvent(
                                (frasier->getVMEList()->at(0).getTime() + frasier->getTime()),
                                frasier->getVMEList()->at(0).getCValue());
                        frasier->getVMEList()->insert(frasier->getVMEList()->begin() + 2, vme);
                    }
                        // if the first line is the input event
                    else {
                        // call lambda and deltaConfluent since we are inputting and outputting at the same time
                        cout << endl;
                        cout << frasier->lambda() << endl;
                        frasier->deltaConfluent(frasier->getVMEList()->at(0).getCoin());
                        frasier->timeAdvancement();
                        VendingMachineEvent vme = *new VendingMachineEvent(
                                (frasier->getVMEList()->at(0).getTime() + frasier->getTime()),
                                frasier->getVMEList()->at(0).getCValue());
                        frasier->getVMEList()->insert(frasier->getVMEList()->begin() + 2, vme);
                    }
                    // we have processed one event and will soon process the other at the end of this loop.
                    // This is to ensure that deltaConfluent is read correctly without screwing up our Event list indexing
                    // since deltaConfluent technically requires the presence of two events to be called.
                    frasier->getVMEList()->erase(frasier->getVMEList()->begin());
                }
            }
            // remove this element from the event list after we have processed it accordingly
            frasier->getVMEList()->erase(frasier->getVMEList()->begin());
        }
    }

    // else case for being unable to open the file
    else cout << "Unable to open file";
}