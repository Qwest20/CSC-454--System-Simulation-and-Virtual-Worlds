#include <iostream>
#include <cfloat>
#include <vector>
#include <fstream>

using namespace std;

// CLASS HEADERS -------------------------------------------------------------------------------------------------------
template<typename X, typename Y>
class AtomicModel;

template<typename X, typename Y>
class ModelsCoupling;

template<typename X, typename Y>
class InputCoupling;

template<typename X, typename Y>
class OutputCoupling;

// ENUMERATION CLASS ---------------------------------------------------------------------------------------------------
enum class Type : int {
    INPUT, OUTPUT, CONFLUENT
};

// BASIC CLASSES -------------------------------------------------------------------------------------------------------
class Time {

// private Wariables
private:
    double real; // real time value
    int count; // 2D time count index

// Constructor
public:

    Time(double r, int c) {
        real = r;
        count = c;
    }

// Getters and Setters
    double getReal() {
        return real;
    }

    int getC() {
        return count;
    }

// New CompareTo method (use "sort" to use this later! :D)"
    bool operator<(Time t2) const {
        // time compareTo from class
        // LOGIC BEHIND THIS:
        // returns the mathematical difference between the times! 0 means same
        if (real == t2.getReal())
            return count < t2.getC();
        return count < t2.getReal();
    }
};

template<typename X, typename Y>
class Event {

// private Wariables
private:
    Time *moment;
    AtomicModel<X, Y> *machine;
    X input;
    Type type;

// Constructors
public:

// input and confluent events
    Event(Time *m, AtomicModel<int, int> *ma, X p, Type t) {
        moment = m;
        machine = ma;
        input = p;
        type = t;
    }

// output events
    Event(Time *m, AtomicModel<X, Y> *ma) {
        moment = m;
        machine = ma;
        type = Type::OUTPUT;
    }

// Getters and Setters
public:
    Time *getMoment() {
        return this->moment;
    }

    AtomicModel<X, Y> *getMachine() {
        return this->machine;
    }

    Type getType() {
        return this->type;
    }

    X getInput() { return this->input; }

    bool operator<(Event e2) const {
        if (moment == e2.getMoment())
            // if the times are the exact same, prioritize outputs
            if (type == Type::OUTPUT)
                return true;
            else
                return false;
        return moment < e2.getMoment();
    }
};

// MODEL CLASSES -------------------------------------------------------------------------------------------------------

// Generics for input and output type
template<typename X, typename Y>
class AtomicModel {
protected:
    // state (p = num inputs in the bin. s = time remaining to process the current part)
    int p = 0;
    double s = 0;

    // other wariables
    string name;
    double processingTime;
    int iD;

    // start at -1 ie before simulation begins
    double timeOfLastInput = -1;

public:

    // Constructor
    AtomicModel(string n, double pT, int id){
        this -> name = n;
        this -> processingTime = pT;
        this -> iD = id;
    }

    // virtual methods
    virtual Y lambda() { return 0; };

    virtual void deltaInt() {cout << "I will be overwritten!" << endl;} ;

    virtual void deltaCon(X input) {cout << "I will be overwritten!" << endl;} ;

    virtual void deltaExt(X input, double elapsedTime) {cout << "I will be overwritten!" << endl;} ;

    virtual double timeAdvance() { return 0; };

    // getters and setters
    double getTimeOfLastInput() {
        return this->getTimeOfLastInput();
    }
    double getProcessingTime() {
        return this->processingTime;
    }

    void setTimeOfLastInput(double d) {
        this->timeOfLastInput = d;
    }

    double getS() {
        return this->s;
    }

    int getId() {
        return this->iD;
    }

    int getP() {
        return p;
    }

    string getName(){
        return this->name;
    }

};

template<typename X, typename Y>
class PressModel : public AtomicModel<X, Y> {

public:

    // Constructor
    using AtomicModel<X,Y>::AtomicModel;

    // Functions
    Y lambda()

    override {
        return 1;
    }

    void deltaInt()

    override {
        this->p--;
        this->
                s = this->processingTime;
    }

    void deltaCon(X q)

    override {
        this->p += q - 1;
        this->
                s = this->processingTime;
    }

    void deltaExt(X q, double e)

    override {
        if (this->p > 0) {
            this->p +=
                    q;
            this->s -=
                    e;
        } else {
            this->p +=
                    q;
            this->
                    s = this->processingTime;
        }
    }

// use this to create new events. You will add this value to your next event's output time
    double timeAdvance()

    override {
        if (this->p > 0)
            return this->
                    s;
        return DBL_MAX;
    }
};

template<typename X, typename Y>
class DrillModel : public AtomicModel<X, Y> {

public:

    // Constructor
    using AtomicModel<X,Y>::AtomicModel;

    // Functions
    Y lambda() override {
        return 1;
    }

    void deltaInt() override {
        this->p--;
        this->s = this->processingTime;
    }

    void deltaCon(X q) override {
        this->p += q - 1;
        this->s = this->processingTime;
    }

    void deltaExt(X q, double e) override {
        if (this->p > 0) {
            this->p += q;
            this->s -= e;
        } else {
            this->p += q;
            this->s = this->processingTime;
        }
    }

// use this to create new events. You will add this value to your next event's output time
    double timeAdvance() override {
        if (this->p > 0)
            return this->s;
        return DBL_MAX;
    }
};

template<typename X, typename Y>
class NetworkModel {

    // Arraylist to store models (do atomics need to have different types?)
private:
    vector<AtomicModel<X, Y>*> modelCollection;
    vector<ModelsCoupling<X, Y>*> couplingsCollection;
    InputCoupling<X, Y>* inputCoupling;
    OutputCoupling<X, Y>* outputCoupling;

public:

    // Model Functions
    // return the lambda of the last modelCollection model as the network model's output
    Y lambda() {
        // local variable for some short output coupling here...
        Y output = (Y) this->outputCoupling->take();
        return output;
    }

    void deltaInt(AtomicModel<X, Y> *aM) {
        // to avoid using find, simply run the model we pass in from the event
        aM->deltaInt();
    }

    void deltaCon(AtomicModel<X, Y> *aM, X input) {
        // to avoid using find, simply run the model we pass in from the event
        aM->deltaCon(input);
    }

    void deltaExt(AtomicModel<X, Y> *aM, X input, double elapsedTime) {
        // to avoid using find, simply run the model we pass in from the event
        aM->deltaExt(input, elapsedTime);
    }

    // Extra functions
public:
    void addModel(AtomicModel<X, Y> *a) {
        modelCollection.insert(modelCollection.begin(), a);
    }

    AtomicModel<int, int>* getModelAt(int index) {
        return modelCollection.at(index);
    }

    vector<AtomicModel<X, Y>*> getModelCollection() {
        return modelCollection;
    }

    void addCoupling(ModelsCoupling<X, Y> *c) {
        couplingsCollection.insert(couplingsCollection.begin(), c);
    }

    vector<ModelsCoupling<X, Y>> getCouplingsCollection() {
        return couplingsCollection;
    }

    InputCoupling<X, Y> *getInputCoupling() {
        return inputCoupling;
    }

    void setInputCoupling(InputCoupling<X, Y> *iC) {
        this->inputCoupling = iC;
    }

    OutputCoupling<X, Y> *getOutputCoupling() {
        return outputCoupling;
    }

    void setOutputCoupling(OutputCoupling<X, Y> *oC) {
        this->outputCoupling = oC;
    }

    ModelsCoupling<X, Y>* findCouplingByModel(AtomicModel<X, Y> *machine, int type) {
        for (ModelsCoupling<X, Y>* xyModelsCoupling: couplingsCollection) {
            // input event
            if (xyModelsCoupling->getModelTo() == machine && type == 1) {
                return xyModelsCoupling;
            }
                // output event
            else if (xyModelsCoupling->getModelFrom() == machine && type == 0) {
                return xyModelsCoupling;
            }
        }
        // no coupling found with this machine
        return nullptr;
    }
};

// COUPLING CLASSES
template<typename X, typename Y>
class AtomicCoupling {
protected:
    X value;

    // getter and setters
public:
    X take() {
        return value;
    }

    void give(Y v) {
        this->value = (X) v;
    }
};

template<typename X, typename Y>
class InputCoupling : public AtomicCoupling<X, Y> {

// private reference
private:
    AtomicModel<X, Y> *modelTo;

public:

    // Constructor
    explicit InputCoupling(AtomicModel<X, Y> *i) {
        this->modelTo = i;
    }

// getter
    AtomicModel<X, Y> *getModelTo() {
        return modelTo;
    }
};

template<typename X, typename Y>
class OutputCoupling : public AtomicCoupling<X, Y> {

// private reference
private:
    AtomicModel<X, Y> *modelFrom;

public:
    explicit OutputCoupling(AtomicModel<X, Y> *o) {
        this->modelFrom = o;
    }

// getters
    AtomicModel<X, Y> *getModelFrom() {
        return modelFrom;
    }
};

template<typename X, typename Y>
class ModelsCoupling : public AtomicCoupling<X, Y> {

// private references
private :
    AtomicModel<X, Y> *modelFrom;
    AtomicModel<X, Y> *modelTo;

    // Constructor
public:
    explicit ModelsCoupling(AtomicModel<X, Y> *o, AtomicModel<X, Y> *i) {
        this->modelFrom = o;
        this->modelTo = i;
    }

// getters
    AtomicModel<X, Y> *getModelFrom() {
        return modelFrom;
    }

    AtomicModel<X, Y> *getModelTo() {
        return modelTo;
    }
};

// PRIORITY QUEUE ------------------------------------------------------------------------------------------------------
template<typename X, typename Y>
class EventScheduler {

    // Member variables of this class
private:
    vector<Event<X, Y>> Heap;

    // Functions
    // Returning the position of
    // the parent for the node currently
    // at pos
private:
    int parent(int pos) {
        if (pos % 2 != 0) {
            return pos / 2;
        } else {
            return pos / 2 - 1;
        }
    }

    // Returning the position of the
    // left child for the node currently at pos
    int leftChild(int pos) {
        return (2 * pos) + 1;
    }

    // Returning the position of
    // the right child for the node currently
    // at pos
    int rightChild(int pos) {
        return (2 * pos) + 2;
    }

    // To swap two nodes of the heap
    void swap(int fpos, int spos) {
        iter_swap(Heap.begin() + fpos, Heap.begin() + spos);
    }

    int indexOfMachineOutput(AtomicModel<X, Y> *machine) {
        for (int i = 0; i < Heap.size(); i++) {
            if (Heap.at(i).getMachine() == machine && Heap.at(i).getType() == Type::OUTPUT) {
                return i;
            }
        }
        // if not found
        return -1;
    }

public:
// To insert a node into the heap
    void insert(Event<X, Y> element) {

        Heap.insert(Heap.begin() + Heap.size(), element);
        int current = Heap.size() - 1;

        while (current > 0 && Heap.at(current).operator<(Heap.at(parent(current)))) {
            swap(current, parent(current));
            current = parent(current);
        }
    }

    Event<X, Y> peek() {
        return Heap.at(0);
    }

    bool isNotEmpty() {
        return !Heap.empty();
    }

    void removeOutputEventByMachine(AtomicModel<X, Y> *machine) {
        int index = indexOfMachineOutput(machine);
        if (index != -1) {
            Heap.erase(Heap.begin() + index);
        }
    }

// To remove and return the minimum
// element from the heap
    Event<X, Y> poll() {

        // read the top event for removal
        Event<X, Y> polled = Heap.at(0);
        Heap.erase(Heap.begin());

        // read the greatest event as the new min, and then remove the duplicate at the end
        if (Heap.size() > 0) {
            Heap.insert(Heap.begin(), Heap.at(Heap.size() - 1));
            Heap.erase(Heap.begin() + Heap.size() - 1);

            int i = 0;
            while (i < Heap.size() - 1) {
                // if the left child is smaller than the parent AND THE RIGHT (IF THE RIGHT EXISTS)
                if (((leftChild(i) < Heap.size() - 1 &&
                      Heap.at(leftChild(i)).operator<(Heap.at(rightChild(i)))) ||
                     (leftChild(i) == Heap.size() - 1)) &&
                    Heap.at(leftChild(i)).operator<(Heap.at(i))) {
                    swap(i, leftChild(i));
                    i = leftChild(i);
                }
                    // otherwise, if the right child is smaller than the parent AND THE LEFT
                else if (rightChild(i) < Heap.size() - 2 &&
                         Heap.at(rightChild(i)).operator<(Heap.at(leftChild(i))) &&
                         Heap.at(rightChild(i)).operator<(Heap.at(i))) {
                    swap(i, rightChild(i));
                    i = rightChild(i);
                } else
                    break;
            }
        }
        return polled;
    }

// run me before you poll at each step in the simulation
    void checkForConfluence() {

        // navigate through the tree and see if two events eX, Yist such that
        // we need a new confluent event to replace it. This occurs if the machine
        // is the same and the events occur at the same time
        // we process this potentially on both sides since I'm not quite sure where
        // confluent events would be just yet in the tree ordering

        // if they both have the same machine and real time, there's a confluent case!
        if (Heap.size() > 1 && Heap.at(leftChild(0)).getMachine() == Heap.at(0).getMachine() &&
            Heap.at(leftChild(0)).getMoment()->getReal() - Heap.at(0).getMoment()->getReal() == 0) {
            // snag one of the events and remove the other as well
            // if the first is the input event
            if (Heap.at(0).getType() == Type::INPUT) {
                Event<X, Y> e = poll();
                poll();
                Time *t = e.getMoment();
                // we have the information to mark a confluent event which will be run when we call poll in the next iteration
                auto *con = new Event<X, Y>(t, e.getMachine(), e.getInput(), Type::CONFLUENT);
                this->insert(*con);
            }
                // if the second is the input event
            else {
                poll();
                Event<X, Y> e = poll();
                Time *t = e.getMoment();
                // we have the information to mark a confluent event which will be run when we call poll in the next iteration
                // we have the information to mark a confluent event which will be run when we call poll in the next iteration
                auto *con = new Event<X, Y>(t, e.getMachine(), e.getInput(), Type::CONFLUENT);
                this->insert(*con);
            }
        } else if (Heap.size() > 2 && Heap.at(rightChild(0)).getMachine() == (Heap.at(0).getMachine()) &&
                   Heap.at(rightChild(0)).getMoment()->getReal() - Heap.at(0).getMoment()->getReal() == 0) {
            // if the first is the input event
            if (Heap.at(0).getType() == Type::INPUT) {
                Event<X, Y> e = poll();
                poll();
                Time *t = e.getMoment();
                // we have the information to mark a confluent event which will be run when we call poll in the next iteration
                auto *con = new Event<X, Y>(t, e.getMachine(), e.getInput(), Type::CONFLUENT);
                this->insert(*con);
            }
                // if the second is the input event
            else {
                poll();
                Event<X, Y> e = poll();
                Time *t = e.getMoment();
                // we have the information to mark a confluent event which will be run when we call poll in the next iteration
                auto *con = new Event<X, Y>(t, e.getMachine(), e.getInput(), Type::CONFLUENT);
                this->insert(*con);
            }
        }
    }

// To print the contents of the heap at each parent and their respective children
void print() {
    cout << &"Here is the current size of the heap: " [ Heap.size()] << endl;
    cout << "Here is the heap as an array:" << endl;
    for (Event<X, Y> current: Heap) {
        string typeToString;
        switch (current.getType()){
            case Type::INPUT:
                typeToString = "INPUT";
                break;
            case Type::OUTPUT:
                typeToString = "OUTPUT";
                break;
            default:
                typeToString = "DEFAULT";
                break;
        }
        cout << to_string(current.getMoment()->getReal()) + ", " + to_string(current.getMoment()->getC()) + ", " +
                current.getMachine()->getName() + ", " + to_string(current.getInput()) + ", " + typeToString << endl;
    }
}
};

// FRAMEWORK CLASS -----------------------------------------------------------------------------------------------------
template<typename X, typename Y>
class Framework {

    // Private Wariables
private:
    double elapsedTime = 0;
    EventScheduler<int, int> *pQ = new EventScheduler<X, Y>();

public:
    void simulate(NetworkModel<X, Y> nM, vector<Event<X, Y>*> inputs) {

        // first, add the events we got to the eventScheduler
        for (Event<X, Y>* input: inputs) {
            pQ->insert(*input);
        }

        cout << "We add the events to the EventHandler" << endl;

        // next, add output events for all the models at t=MAX.VALUE, to appropriate with the
        // "there is always one output event" rule
        for (int i = 0; i < nM.getModelCollection().size(); i++) {
            AtomicModel<X, Y> *aM = nM.getModelAt(i);
            Time *t = new Time(aM->timeAdvance(), i);
            auto *e = new Event<X, Y>(t, aM);
            pQ->insert(*e);
        }

        // simulation loop that stops once we hit output events of positive infinity or no events at all
        while (pQ->isNotEmpty() && pQ->peek().getMoment()->getReal() != DBL_MAX) {

            pQ->print();

            // check at the top will ensure that we don't have any unaddressed confluent events given the following criteria
            // confluents have the same real and c values on the same machine
            pQ->checkForConfluence();

            // now that we have checked for confluents, we can poll the event
            Event<X, Y> top = pQ->poll();

            // update elapsed time to this event
            elapsedTime = top.getMoment()->getReal();

            // process the event based on the type
            if (top.getType() == Type::INPUT) {

                // COUPLING
                // Store input variable for later reference
                X input;
                // If we're looking at the first machine in the NM
                if (top.getMachine() == nM.getInputCoupling()->getModelTo()) {
                    // read from the input coupling model's value
                    // we need give and take at the same step
                    nM.getInputCoupling()->give((Y) top.getInput());
                    input = nM.getInputCoupling()->take();
                } else {
                    // find the input coupling for this machine
                    AtomicCoupling<X, Y>* ourCouple = nM.findCouplingByModel(top.getMachine(), 1);
                    input = ourCouple->take();
                }

                // derive a timePassed variable, so we know how much time has passed since the last input event
                // cases in which there is NO part already in there are covered with the if conditions
                // - we're at t=-1 (start of simulation
                // - OR there were no parts in the machine before this input, and therefore no time should be subtracted
                double timePassed;
                if (top.getMachine()->getTimeOfLastInput() == -1 ||
                    elapsedTime - top.getMachine()->getTimeOfLastInput() > top.getMachine()->getProcessingTime())
                    timePassed = 0;
                else
                    timePassed = elapsedTime - top.getMachine()->getTimeOfLastInput();
                // run this particular machine's delta external
                nM.deltaExt(top.getMachine(), input, timePassed);
                // update the time of last input, since we have taken in a new input at this time
                top.getMachine()->setTimeOfLastInput(elapsedTime);

                // Corresponding output event created here
                // (only do this if s = processing time, since we know we can process a new input)
                if (top.getMachine()->getS() == top.getMachine()->getProcessingTime()) {
                    double r = elapsedTime + top.getMachine()->timeAdvance();
                    // c value will be equivalent to the machine's index
                    int c = top.getMachine()->getId();
                    Time *t = new Time(r, c);
                    // output from this same machine at the designated time
                    Event<X, Y> newO = *new Event<X, Y>(t, top.getMachine());

                    // remove the old event and add the new
                    pQ->removeOutputEventByMachine(top.getMachine());
                    pQ->insert(newO);
                }
            } else if (top.getType() == Type::OUTPUT) {

                // COUPLING
                // Store output variable for later reference
                Y output = top.getMachine()->lambda();
                // If we're looking at the last machine in the NM
                if (top.getMachine() == nM.getOutputCoupling()->getModelFrom()) {

                    // set the network model's output coupling to this value
                    nM.getOutputCoupling()->give(output);

                    // perform the network model's lambda here, since now we can!
                    // (also functions as outputCoupling's take method)
                    cout << to_string(this->elapsedTime) + " " + to_string(nM.lambda());

                } else {
                    // find the output coupling for this machine
                    AtomicCoupling<X, Y>* ourCouple = nM.findCouplingByModel(top.getMachine(), 0);
                    ourCouple->give(output);
                }

                // run this particular machine's deltaInt (p is decremented here)
                nM.deltaInt(top.getMachine());

                // if this model is going to another model in any case, we need to make a new input event
                if (nM.getOutputCoupling()->getModelFrom() != top.getMachine()) {

                    // Corresponding input event created here
                    // receive information on the next model in the network model based on the couplings
                    AtomicModel<X, Y> *nextMachine = nM.findCouplingByModel(top.getMachine(), 0)->getModelTo();

                    // time will be the SAME for the coming input event as the current output event!
                    double r = elapsedTime;
                    // c value will be the value of the next machine
                    Time *t = new Time(r, nextMachine->getId());
                    // input from this same machine at the designated time
                    auto *newI = new Event<X, Y>(t, nextMachine, (X) output, Type::INPUT);

                    // add the new input event for the next model in the sequence
                    pQ->insert(*newI);
                }

                // if the top machine has a bin of processing inputs to work with,
                // then we'll need to schedule a new output event accordingly
                if (top.getMachine()->getP() > 0) {
                    // new output event on the top machine since we have finished processing a part
                    double re = elapsedTime + top.getMachine()->timeAdvance();
                    // c value will be the same
                    Time *ti = new Time(re, top.getMachine()->getId());
                    // output from this same machine at the designated time
                    Event<X, Y> newOu = *new Event<X, Y>(ti, top.getMachine());

                    // remove the old event (if it exists) and add the new one
                    // (shouldn't find one in this case though...)
                    pQ->removeOutputEventByMachine(top.getMachine());
                    pQ->insert(newOu);
                }
            } else {
                // COUPLING
                // OUTPUT FIRST
                Y output = top.getMachine()->lambda();
                // If we're looking at the last machine in the NM
                if (top.getMachine() == nM.getOutputCoupling()->getModelFrom()) {

                    // set the network model's output coupling to this value
                    nM.getOutputCoupling()->give(output);

                    // perform the network model's lambda here, since now we can!
                    // (also functions as outputCoupling's take method)
                    cout << to_string(this->elapsedTime) + " " + to_string(nM.lambda());

                } else {
                    // find the output coupling for this machine
                    AtomicCoupling<X, Y>* ourCouple = nM.findCouplingByModel(top.getMachine(), 0);
                    ourCouple->give(output);
                }
                // INPUT SECOND
                // Store input variable for later reference
                X input;
                // If we're looking at the first machine in the NM
                if (top.getMachine() == nM.getInputCoupling()->getModelTo()) {
                    // read from the input coupling model's value
                    // we need give and take at the same step
                    nM.getInputCoupling()->give((Y) top.getInput());
                    input = nM.getInputCoupling()->take();
                } else {
                    // find the input coupling for this machine
                    AtomicCoupling<X, Y>* ourCouple = nM.findCouplingByModel(top.getMachine(), 1);
                    input = ourCouple->take();
                }

                // update the time of last input, since we have taken in a new input at this time
                top.getMachine()->setTimeOfLastInput(elapsedTime);

                // call deltaCon (no need to worry about time passed, since processing time will be reset)
                nM.deltaCon(top.getMachine(), input);

                // EVENT CREATION
                // output first
                // Corresponding output event created here
                // this should run in every case, since we just ran deltaCon to reset S
                if (top.getMachine()->getS() == top.getMachine()->getProcessingTime()) {
                    double r = elapsedTime + top.getMachine()->timeAdvance();
                    // c value will stay the same
                    Time *t = new Time(r, top.getMachine()->getId());
                    // output from this same machine at the designated time
                    Event<X, Y> newOut = *new Event<X, Y>(t, top.getMachine());
                    pQ->insert(newOut);
                }

                // input second
                // if this model is going to another model in any case, we need to make a new input event
                if (nM.getOutputCoupling()->getModelFrom() != top.getMachine()) {

                    // Corresponding input event created here
                    // receive information on the next model in the network model based on the couplings
                    AtomicModel<X, Y> *nextMachine = nM.findCouplingByModel(top.getMachine(), 0)->getModelTo();

                    // time will be the SAME for the coming input event as the current output event!
                    double r = elapsedTime;
                    // c value will be the value of the next machine
                    Time *t = new Time(r, nextMachine->getId());
                    // input from this same machine at the designated time
                    Event<X, Y> newI = *new Event<X, Y>(t, nextMachine, (X) output, Type::INPUT);

                    // add the new input event for the next model in the sequence
                    pQ->insert(newI);
                }
                // no need to see if we have an output event on this confluent case,
                // because we did that with the confluent's input event
            }
        }
    }
};

// MAIN CLASS
int main() {

    //setup for PressDrill Network Model Case
    auto *Cliff = new Framework<int, int>();
    // index parameter included to differentiate, should we have multiple instances of the same model type
    auto *p = new PressModel<int, int>(&"Press #"[1], 1, 1);
    auto *d = new DrillModel<int, int>(&"Drill#"[1], 1, 1);
    auto *c = new ModelsCoupling<int, int>(p, d);
    NetworkModel<int, int> n = *new NetworkModel<int, int>();
    n.addModel(p);
    n.addModel(d);
    n.addCoupling(c);
    n.setInputCoupling(new InputCoupling<int, int>(p));
    n.setOutputCoupling(new OutputCoupling<int, int>(d));

    // input loop to derive an arraylist of events
    vector<Event<int, int>*> inputEvents;

    //      INPUT VIA FILE
    ifstream myReadFile(R"(C:\Users\ljmcm\OneDrive\Desktop\Programming\CSC 454\A6\A6Input.txt)");
    string myText;
    while (getline(myReadFile, myText)) {
        // FIX ME!
        double occurrenceTime = stod(myText.substr(0, myText.find(',')));
        int cVal = stoi(myText.substr(myText.find(',') + 1, 1));
        int input = stoi(myText.substr(myText.length() - 1));

        // form a new time instance for the new event instance that follows
        Time *t = new Time(occurrenceTime, cVal);
        // only create this event for the press machine since this is where it will be inserted
        auto *e = new Event<int, int>(t, p, input, Type::INPUT);

        // add this event to the queue
        inputEvents.insert(inputEvents.begin() + inputEvents.size(), e);
    }
    myReadFile.close();

    // run the simulation with this list of composed events
    Cliff->simulate(n, inputEvents);
}