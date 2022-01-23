#include <iostream>

using namespace std;

// Exception class
class NetworkModelException : public exception {
private:
    const char *message;
public:
    explicit NetworkModelException(const char *msg) {
        this->message = msg;
    }

    const char *what() const throw() {
        return message;
    }
};

// Model classes
// XOR Model Class
class XORModel {
    // Private Wariables
private:
    int state = 0;
public:
    // Lambda function
    // S -> Y
    int lambda() {
        return state;
    }

    // Delta Function
    // (X,S) -> S
    void delta(int a, int b) {
        state = a ^ b;
    }
};

// Memory Model Class
class MemoryModel {
    // private state bit
private:
    int state[2] = {0, 0};
    // Lambda function
    // S -> Y
public:
    int lambda() {
        return state[1];
    }

    // Delta Function
    // (X,S) -> S
    void delta(int x) {
        state[1] = state[0];
        state[0] = x;
    }
};

// Network Model Class
class NetworkModel {
    // private state bit
private:
    int state = 0;
    // Models
    XORModel *x1 = new XORModel();
    XORModel *x2 = new XORModel();
    MemoryModel *m = new MemoryModel();
    // private coupling variables
    int x1Out = 0;
    int x2Out = 0;
    int memOut = 0;
    // Getters and Setters
public:
    XORModel getX1() {
        return *x1;
    }
    XORModel getX2() {
        return *x2;
    }
    MemoryModel getM() {
        return *m;
    }
    // Lambda function
    // S -> Y
    int lambda() {
        // output the state of the network model
        // only output on every NETWORK tick, not every ATOMIC tick!
        cout << "NETWORK.lambda() = " << endl;
        return state;
    }
    // Coupling Function to specify variables for later use
    void coupling() {
        // coupling variables set for later
        x1Out = x1->lambda();
        x2Out = x2->lambda();
        memOut = m->lambda();
        state = x2Out;
    }
    // Delta Function
    // (X,S) -> S
    void delta(int a, int b) {
        // inner deltas
        x1->delta(a, b);
        x2->delta(x1Out, memOut);
        m->delta(x2Out);
    }
};

int main() {
    // Model instances
    auto *n = new NetworkModel();
    // input for this tick
    cout << "Welcome to the XOR State Machine!" << endl;
    cout << "NOTE: enter -1 for EITHER NUMBER INPUT and the program will exit" << endl;
    // Variable to keep track of network tick index
    int networkTick = 1;
    // simulation while loop
    while (true) {
        // newly added try catch block ensures that integers entered are only 0's and 1's
        try {
            cout << endl;
            cout << "Please enter your first value" << endl;
            int a;
            cin >> a;
            cout << "Please enter your second value" << endl;
            int b;
            cin >> b;
            cout << endl;
            // exit the program if we see a -1 in either bit
            if (a == -1 || b == -1)
                exit(0);
            // otherwise, keep going
            else if ((a == 0 || a == 1) && (b == 0 || b == 1)) {
                // run this three times to complete an entire tick
                cout << "-------------------Network Tick = " << networkTick << "-------------------" << endl;
                for (int atomicTick = 1; atomicTick < 4; atomicTick++) {
                    cout << "-------------------Atomic Tick = " << atomicTick << "-------------------" << endl;
//                    // inner lambdas if desired. You can comment this block back in if you wish
//                    cout << "XOR1.lambda() = " << endl;
//                    cout << n->getX1().lambda() << endl;
//                    cout << "XOR2.lambda() = " << endl;
//                    cout << n->getX2().lambda() << endl;
//                    cout << "MEM.lambda() = " << endl;
//                    cout << n->getM().lambda() << endl;
                    // coupling
                    n->coupling();
                    // delta
                    n->delta(a, b);
                }
                // lambda for the whole network outputs only on every network tick
                cout << n->lambda() << endl;
                // increment the network tick
                networkTick++;
            }
                // throw our exception if they broke the input rules
            else {
                throw a;
            }
        }
        catch (int a) {
            throw NetworkModelException("Please enter correct input values only (0 or 1 for either bit).");
        }
    }
}
