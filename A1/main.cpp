#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

// This program was brought to you in part by the Homestuck OST

//classes (declared early for global method reference)
class Room;

class Creature;

class PC;

class NPC;

class Animal;

// global methods for various uses (declared early for class reference)
int *stringToIntArray(string str);
char *shuffle_array(char arr[], int n);
int getIndexByIDInRoom(Room *r, int id);
int getIndexByID(vector<Creature*> c, int id);
void allBad(Room *r, PC *pc);

//classes cont.
class Room {

    //wariables
private:
    int state;
    int roomNum;
    vector<Creature *> population;
    // Reference neighboring rooms
    int *neighbors = new int[4];
    Room *northNeighbor;
    Room *southNeighbor;
    Room *eastNeighbor;
    Room *westNeighbor;

    //methods
public:

    // constructor
    Room(int n, int c) {
        roomNum = n;
        state = c;
        northNeighbor = nullptr;
        southNeighbor = nullptr;
        eastNeighbor = nullptr;
        westNeighbor = nullptr;
    }

    //getters
    int getState() {
        return state;
    }

    int getRoomNum() {
        return roomNum;
    }

    int *getNeighbors() {
        return neighbors;
    }

    Room *getNorthNeighbor() {
        return northNeighbor;
    }

    Room *getSouthNeighbor() {
        return southNeighbor;
    }

    Room *getEastNeighbor() {
        return eastNeighbor;
    }

    Room *getWestNeighbor() {
        return westNeighbor;
    }

    vector<Creature *> getPopulation() {
        return population;
    }

    //setters
    void setState(int c) {
        state = c;
    }

    void setNeighbors(int *n) {
        neighbors = n;
    }

    void setNorthNeighbor(Room *n) {
        // return the address of room n
        northNeighbor = n;
    }

    void setSouthNeighbor(Room *s) {
        // return the address of room s
        southNeighbor = s;
    }

    void setEastNeighbor(Room *e) {
        // return the address of room e
        eastNeighbor = e;
    }

    void setWestNeighbor(Room *w) {
        // return the address of room w
        westNeighbor = w;
    }

    // actions
    void insertCreatureInRoom(Creature *c) {
        population.push_back(c);
    }

    void removeCreatureFromRoom(int id) {
        int index = getIndexByIDInRoom(this,id);
        population.erase(population.begin() + index);
    }
};

class Creature {

    //wariables
private:
    Room *currentRoom;
    int id;

    // protected wariables (can be unique depending on the child class)
protected:
    int type;
    string name;

    //methods
public:

    // constructor
    Creature(Room *cR, int i) {
        currentRoom = cR;
        id = i;
        type = 4; // by default because the IDE was yelling at me when I didn't do this...
    }

    //getters
    string getName() {
        return name;
    }

    Room *getRoom() {
        return currentRoom;
    }

    int getType() {
        return type;
    }

    int getID() {
        return id;
    }

    //setters
    void setRoom(Room *r) {
        currentRoom = r;
    }

    //actions
    // virtual methods serve as methods that can be overridden by child classes should the need arise
    virtual void move(Room *r, int dir) {
        // from the original given room, move in the corresponding direction
        switch (dir) {
            // N
            case 0:
                cout << this->getName() << " attempts to go north." << endl;
                if (r->getPopulation().size() < 9) {
                    this->movementCode(r);
                } else {
                    cout << "It fails, northern room is full." << endl;
                }
                break;
                // S
            case 1:
                cout << this->getName() << " attempts to go south." << endl;
                if (r->getPopulation().size() < 9) {
                    this->movementCode(r);
                } else {
                    cout << "It fails, southern room is full." << endl;
                }
                break;
                // E
            case 2:
                cout << this->getName() << " attempts to go east." << endl;
                if (r->getPopulation().size() < 9) {
                    this->movementCode(r);
                } else {
                    cout << "It fails, eastern room is full." << endl;
                }
                break;
                // W
            case 3:
                cout << this->getName() << " attempts to go west." << endl;
                if (r->getPopulation().size() < 9) {
                    this->movementCode(r);
                } else {
                    cout << "It fails, western room is full." << endl;
                }
                break;
            default:
                break;
        }
    }

    // "true" virtual methods are assigned to 0 since they have no original body.
    virtual void updateRoomState(Room *r) = 0;

    virtual int reactToClean(PC *pc, int x) = 0;

    virtual int reactToDirty(PC *pc, int x) = 0;

    virtual void toldToClean(PC *pc) = 0;

    virtual void toldToDirty(PC *pc) = 0;

    // supplement for the "move" virtual method, stored separately for cleanliness
    void movementCode(Room *r) {
        // remove the creature from this room
        this->getRoom()->removeCreatureFromRoom(this->getID());
        // add the creature to the desired room
        r->insertCreatureInRoom(this);
        // move the creature's internal reference to the new room
        this->setRoom(r);
        cout << "It succeeds." << endl;
        this->updateRoomState(this->getRoom());
    }

    // supplement for clean and dirty methods seen later, stored separately in the parent class for cleanliness
    int wantsToLeave(Room *thisRoom, PC *pc, int x){
        cout << name << " wants to leave" << endl;

        // randomize an array of characters to determine random AI behavior
        char choices[4] = {'n', 's', 'e', 'w'};
        char *choose = shuffle_array(choices, (sizeof choices / sizeof choices[0]));

        // bool to make sure the creature actually moved
        bool moved = false;

        // loop through 4 possible directions, including a last resort case at 4
        for (int e = 0; e <= 4; e++) {
            if (e == 4) {
                // leave the game since there are no more room options!
                cout << name
                     << " escapes the game through the ceiling!"
                     << endl;
                // remove creature from the room
                thisRoom->removeCreatureFromRoom(
                        this->getID());
                // handle all other respect issues in the room with a global method (didn't seem to work when placed in an object)
                allBad(thisRoom, pc);
            } else {
                switch (choose[e]) {
                    case 'n':
                        if (thisRoom->getNorthNeighbor() != nullptr) {
                            cout << name
                                 << " attempts to go north." << endl;
                            if (thisRoom->getNorthNeighbor()->getPopulation().size() <
                                9) {
                                // add the creature to the desired room
                                thisRoom->getNorthNeighbor()->insertCreatureInRoom(
                                        this);
                                // move the creature's internal reference to the new room
                                this->setRoom(
                                        thisRoom->getNorthNeighbor());
                                // remove the creature from this room
                                thisRoom->removeCreatureFromRoom(
                                        this->getID());
                                moved = true;
                                // modify this variable since it is passed through from the main method
                                // x signifies a counter on a for loop to iterate through all the creatures in a room
                                // if a creature is removed from the room, we need to compensate for that in this manner
                                x--;
                                cout << "It succeeds." << endl;
                                this->updateRoomState(
                                        this->getRoom());
                            } else {
                                cout << "It fails, northern room is full." << endl;
                            }
                        }
                        break;
                    case 's':
                        if (thisRoom->getSouthNeighbor() != nullptr) {
                            cout << name
                                 << " attempts to go south." << endl;
                            if (thisRoom->getSouthNeighbor()->getPopulation().size() <
                                9) {
                                // add the creature to the desired room
                                thisRoom->getSouthNeighbor()->insertCreatureInRoom(
                                        this);
                                // move the creature's internal reference to the new room
                                this->setRoom(
                                        thisRoom->getSouthNeighbor());
                                // remove the creature from this room
                                thisRoom->removeCreatureFromRoom(
                                        this->getID());
                                moved = true;
                                x--;
                                cout << "It succeeds." << endl;
                                this->updateRoomState(
                                        this->getRoom());
                            } else {
                                cout << "It fails, southern room is full." << endl;
                            }
                        }
                        break;
                    case 'e':
                        if (thisRoom->getEastNeighbor() != nullptr) {
                            cout << name
                                 << " attempts to go east." << endl;
                            if (thisRoom->getEastNeighbor()->getPopulation().size() <
                                9) {
                                // add the creature to the desired room
                                thisRoom->getEastNeighbor()->insertCreatureInRoom(
                                        this);
                                // move the creature's internal reference to the new room
                                this->setRoom(
                                        thisRoom->getEastNeighbor());
                                // remove the creature from this room
                                thisRoom->removeCreatureFromRoom(
                                        this->getID());
                                moved = true;
                                x--;
                                cout << "It succeeds." << endl;
                                this->updateRoomState(
                                        this->getRoom());
                            } else {
                                cout << "It fails, eastern room is full." << endl;
                            }
                        }
                        break;
                    case 'w':
                        if (thisRoom->getWestNeighbor() != nullptr) {
                            cout << name
                                 << " attempts to go west." << endl;
                            if (thisRoom->getWestNeighbor()->getPopulation().size() <
                                9) {
                                // add the creature to the desired room
                                thisRoom->getWestNeighbor()->insertCreatureInRoom(
                                        this);
                                // move the creature's internal reference to the new room
                                this->setRoom(
                                        thisRoom->getWestNeighbor());
                                // remove the creature from this room
                                thisRoom->removeCreatureFromRoom(
                                        this->getID());
                                moved = true;
                                x--;
                                cout << "It succeeds." << endl;
                                this->updateRoomState(
                                        this->getRoom());
                            } else {
                                cout << "It fails, western room is full." << endl;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            // if we've moved, don't bother with looping anymore
            if (moved)
                break;
        }
        return x;
    }
};

class PC : public Creature {

    //PC wariables
private:
    int respect = 40;

    //PC methods
public:
    //constructor
    PC(Room *cR, int id) : Creature(cR, id) {
        type = 0;
        name = "Player";
    }

    //getters
    int getRespect() const {
        return respect;
    }

    //setters
    void setRespect(int r) {
        respect = r;
    }

    // actions
    void look() {
        Room *thisRoom = this->getRoom();
        cout << "Room: " << thisRoom->getRoomNum() << endl;
        switch (thisRoom->getState()) {
            case 0:
                cout << "State: Clean" << endl;
                break;
            case 1:
                cout << "State: Half Dirty" << endl;
                break;
            case 2:
                cout << "State: Dirty" << endl;
                break;
        }
        cout << "Neighboring Rooms: " << endl;
        if (thisRoom->getNorthNeighbor() != nullptr) {
            cout << "Room " << thisRoom->getNorthNeighbor()->getRoomNum() << " is to the north."
                 << endl;
        }
        if (thisRoom->getSouthNeighbor() != nullptr) {
            cout << "Room " << thisRoom->getSouthNeighbor()->getRoomNum() << " is to the south."
                 << endl;
        }
        if (thisRoom->getEastNeighbor() != nullptr) {
            cout << "Room " << thisRoom->getEastNeighbor()->getRoomNum() << " is to the east." << endl;
        }
        if (thisRoom->getWestNeighbor() != nullptr) {
            cout << "Room " << thisRoom->getWestNeighbor()->getRoomNum() << " is to the west." << endl;
        }
        cout << "Creatures in Room: " << endl;
        cout << "Player" << endl;
        for (int p = 0; p < thisRoom->getPopulation().size(); p++) {
            cout << thisRoom->getPopulation().at(p)->getName() << endl;
        }
    }

    void move(int dir) {

        // save a reference of the current room to make references a bit easier to read
        Room *thisRoom = this->getRoom();

        switch (dir) {
            // N
            case 0:
                if (thisRoom->getNorthNeighbor() != nullptr) {
                    this->setRoom(thisRoom->getNorthNeighbor());
                    cout << "You leave towards the North.";
                } else {
                    cout << "There's no room to the north! Try again!" << endl;
                }
                break;
                // S
            case 1:
                if (thisRoom->getSouthNeighbor() != nullptr) {
                    this->setRoom(thisRoom->getSouthNeighbor());
                    cout << "You leave towards the South.";
                } else {
                    cout << "There's no room to the south! Try again!" << endl;
                }
                break;
                // E
            case 2:
                if (thisRoom->getEastNeighbor() != nullptr) {
                    this->setRoom(thisRoom->getEastNeighbor());
                    cout << "You leave towards the East.";
                } else {
                    cout << "There's no room to the east! Try again!" << endl;
                }
                break;
                // W
            case 3:
                if (thisRoom->getWestNeighbor() != nullptr) {
                    this->setRoom(thisRoom->getWestNeighbor());
                    cout << "You leave towards the West.";
                } else {
                    cout << "There's no room to the west! Try again!" << endl;
                }
                break;
            default:
                break;
        }
        cout << "You are now in room " << this->getRoom()->getRoomNum() << endl;
    }

    void updateRoomState(Room *r) override {
        // nothing needed here
    }

    int reactToClean(PC *pc, int x) override {
        // nothing needed here
        return x;
    }

    void toldToClean(PC *pc) override {
        // nothing needed here
    }

    int reactToDirty(PC *pc, int x) override {
        // nothing needed here
        return x;
    }

    void toldToDirty(PC *pc) override {
        // nothing needed here
    }
};

class NPC : public Creature {

    //methods
public:
    // constructor
    NPC(Room *cR, int id) : Creature(cR, id) {
        type = 2;
        name = "Human " + to_string(id);
    }

    //actions
    // used when a creature moves to another room that isn't in a state that the creature would otherwise like
    void updateRoomState(Room *r) override {
        if (r->getState() == 0) {
            r->setState(1);
            cout << name << " dirtied Room " << this->getRoom()->getRoomNum() << endl;
        }
    }

    //clean based actions
    int reactToClean(PC *pc, int x) override{

        // reference to the current room
        Room *thisRoom = this->getRoom();

        // modify respect and print accordingly
        cout << name << " grumbles at you!" << endl;
        pc->setRespect(pc->getRespect() - 1);
        cout << "Respect -1" << endl;
        // this creature may need to move...
        if (thisRoom->getState() == 0) {
            // Creature wants to leave this room
            x = this->wantsToLeave(thisRoom, pc, x);
        }
        // return x for that for loop I mentioned earlier
        return x;
    }

    void toldToClean(PC *pc) override{
        cout << name << " did not want to clean. They grumble twice!"
             << endl;
        pc->setRespect(pc->getRespect() - 2);
        cout << "Respect -2" << endl;
    }

    //dirty based actions
    int reactToDirty(PC *pc, int x) override{
        cout << name << " smiles to you!" << endl;
        pc->setRespect(pc->getRespect() + 1);
        cout << "Respect +1" << endl;
        return x;
    }

    void toldToDirty(PC *pc) override{
        cout << name
             << " is happy to make the room dirty for you! They smile to you twice!"
             << endl;
        pc->setRespect(pc->getRespect() + 2);
        cout << "Respect +2" << endl;
    }
};

class Animal : public Creature {

    //methods
public:
    // constructor
    Animal(Room *cR, int id) : Creature(cR, id) {
        type = 1;
        name = "Animal " + to_string(id);
    }

    // actions
    void updateRoomState(Room *r) override{
        if (r->getState() == 2) {
            r->setState(1);
            cout << name << " cleaned Room " << this->getRoom()->getRoomNum() << endl;
        }
    }

    // clean based actions
    int reactToClean(PC *pc, int x) override{
        cout << name << " licks your face!" << endl;
        pc->setRespect(pc->getRespect() + 1);
        cout << "Respect +1" << endl;
        return x;
    }

    void toldToClean(PC *pc) override{
        cout << name
             << " is happy to help you clean! They lick your face twice!"
             << endl;
        pc->setRespect(pc->getRespect() + 2);
        cout << "Respect +2" << endl;
    }

    // dirty based actions
    int reactToDirty(PC *pc, int x) override{
        Room *thisRoom = this->getRoom();

        cout << name << " growls at you!" << endl;
        pc->setRespect(pc->getRespect() - 1);
        cout << "Respect -1" << endl;
        // this creature may need to move...
        if (thisRoom->getState() == 2) {
            x = this->wantsToLeave(thisRoom, pc, x);
        }
        return x;
    }

    void toldToDirty(PC *pc) override{
        cout << name
             << " did not want to make the room dirty. They growl twice!" << endl;
        pc->setRespect(pc->getRespect() - 2);
        cout << "Respect -2" << endl;
    }
};


//global functions cont.
int *stringToIntArray(string str) {

    // make a pointer to a heap integer array of length 5
    int *outputData = new int[5];

    // for array element counting
    int j = 0;

    // loop through the elements of str ex. "0 1 10 100 6"
    for (int i = 0; i < str.size(); i++) {

        // move to next element
        if (str[i] == ' ') {
            // go to the next element, which should be a number
            continue;
        }

        // detect a negative number
        else if (str[i] == '-') {
            i++; // bump to the next value on the input string (passed the negative sign)
            if (str[i] == '1') {
                outputData[j] = -1;
                j++;
            } else {
                cout << "Please enter correct input!" << endl;
                exit(4);
            }
        }

        //detect a positive number
        else {
            // single digit num
            if (isdigit(str[i])) {
                //double digit num
                if (isdigit(str[i + 1])) {
                    //triple digit num
                    if (isdigit(str[i + 2])) {
                        string inputVal = str.substr(i, 3);
                        outputData[j] = stoi(inputVal);
                        j++;
                        // handle extra digits
                        i += 2;
                    } else {
                        string inputVal = str.substr(i, 2);
                        outputData[j] = stoi(inputVal);
                        j++;
                        // handle extra digit
                        i++;
                    }
                } else {
                    string inputVal = str.substr(i, 1);
                    outputData[j] = stoi(inputVal);
                    j++;
                }
            }
        }
    }
    return outputData;
}

char *shuffle_array(char arr[], int n) {
    // To obtain a time-based seed
    unsigned seed = 0;

    // Shuffling our array
    // uses #include <bits/stdc++.h>
    shuffle(arr, arr + n, default_random_engine(seed));

    return arr;
}

int getIndexByIDInRoom(Room *r, int id) {

    // loop through the population in this room to find by the given id value
    // returns -1 if not found
    for (int f = 0; f < r->getPopulation().size(); f++) {
        if (r->getPopulation().at(f)->getID() == id) {
            return f;
        }
    }
    return -1;
}

int getIndexByID(vector<Creature*> c, int id) {

    // loop through the population in this room to find by the given id value
    // returns -1 if not found
    for (int f = 0; f < c.size(); f++) {
        if (c.at(f)->getID() == id) {
            return f;
        }
    }
    return -1;
}

void allBad(Room *r, PC *pc){
    // Handle respect deductions if a creature leaves the game

    cout << "All remaining creatures act negatively as a result of this."
         << endl;
    for (int b = 0; b < r->getPopulation().size(); b++) {
        // handle animals
        if (r->getPopulation().at(b)->getType() == 1) {
            cout << r->getPopulation().at(b)->getName()
                 << " growls at you!" << endl;
            pc->setRespect(pc->getRespect() - 1);
            cout << "Respect -1" << endl;
        }
        // handle NPC's
        if (r->getPopulation().at(b)->getType() == 2) {
            cout << r->getPopulation().at(b)->getName()
                 << " grumbles at you!" << endl;
            pc->setRespect(pc->getRespect() - 1);
            cout << "Respect -1" << endl;
        }
    }
}

int main() {

    //---------------------------------------------------SETUP---------------------------------------------------------

    // Keep constant reference of the PC to be initialized later
    PC *ThePC;
    // Create a vector of rooms for the simulation
    vector<Room> simulation;

    // Initial Room creation input block
    cout << "Welcome to the game! Please enter your input line by line!" << endl;
    cout << "Please enter number of rooms (0-100)" << endl;
    int numRooms;
    cin >> numRooms;
    cin.ignore();

    // Read room input and initialize rooms in vector
    for (int i = 0; i < numRooms; i++) {

        // Take room input by line
        string roomData;
        cout << "Please enter room data for room #" << i << ":" << endl;
        getline(cin, roomData);

        // Parse the data and insert into vector
        int *rD = stringToIntArray(roomData);
        simulation.push_back(*new Room(i, rD[0]));

        // Parse the data needed for initializing neighbors (handled later after all rooms are made)
        int *neighbors = new int[4];
        for (int t = 0; t < 4; t++) {
            neighbors[t] = rD[t + 1];
        }
        simulation.at(i).setNeighbors(neighbors);
    }

    // Continue with room input to connect the rooms accordingly
    for (int i = 0; i < numRooms; i++) {
        // loop through n,s,e,w accordingly as 4 directions
        for (int a = 0; a < 4; a++) {
            switch (a) {
                // N
                case 0:
                    if (simulation.at(i).getNeighbors()[a] != -1) {
                        // store this destination room in the respective cardinal direction that was specified
                        Room *destinationRoom = &simulation.at(simulation.at(i).getNeighbors()[a]);
                        simulation.at(i).setNorthNeighbor(destinationRoom);
                    }
                    break;
                    // S
                case 1:
                    if (simulation.at(i).getNeighbors()[a] != -1) {
                        Room *destinationRoom = &simulation.at(simulation.at(i).getNeighbors()[a]);
                        simulation.at(i).setSouthNeighbor(destinationRoom);
                    }
                    break;
                    // E
                case 2:
                    if (simulation.at(i).getNeighbors()[a] != -1) {
                        Room *destinationRoom = &simulation.at(simulation.at(i).getNeighbors()[a]);
                        simulation.at(i).setEastNeighbor(destinationRoom);
                    }
                    break;
                    // W
                case 3:
                    if (simulation.at(i).getNeighbors()[a] != -1) {
                        Room *destinationRoom = &simulation.at(simulation.at(i).getNeighbors()[a]);
                        simulation.at(i).setWestNeighbor(destinationRoom);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // Initial Creature creation input block
    cout << "Please enter number of creatures (0-100)" << endl;
    int numCreatures;
    cin >> numCreatures;
    cin.ignore();

    // Read creature input and place them in corresponding rooms
    for (int l = 0; l < numCreatures; l++) {

        // Take creature input
        string cData;
        cout << "Please enter room data for creature #" << l << ":" << endl;
        getline(cin, cData);

        // Parse data into array and create creatures based on type
        int *creatureData = stringToIntArray(cData);
        // PC
        if (creatureData[0] == 0) {
            // Create PC with the given data and the address of the room it's placed in
            auto *pc = new PC(&simulation.at(creatureData[1]), l);
            ThePC = pc;
        }
            // ANIMAL
        else if (creatureData[0] == 1) {
            // Create Animal with the given data and the address of the room it's placed in
            auto *a = new Animal(&simulation.at(creatureData[1]), l);
            // Add the Animal to the room's reference as well
            simulation.at(creatureData[1]).insertCreatureInRoom(a);
        }
            // NPC
        else if (creatureData[0] == 2) {
            // Create NPC with the given data and the address of the room it's placed in
            auto *npc = new NPC(&simulation.at(creatureData[1]), l);
            // Add the NPC to the room's reference as well
            simulation.at(creatureData[1]).insertCreatureInRoom(npc);
        }
    }

    //--------------------------------------------------GAMEPLAY-------------------------------------------------------

    // Obnoxious showmanship
    cout << "   ______                             __             __  __\n"
            "  / ____/___ _____ ___  ___     _____/ /_____ ______/ /_/ /\n"
            " / / __/ __ `/ __ `__ \\/ _ \\   / ___/ __/ __ `/ ___/ __/ / \n"
            "/ /_/ / /_/ / / / / / /  __/  (__  ) /_/ /_/ / /  / /_/_/  \n"
            "\\____/\\__,_/_/ /_/ /_/\\___/  /____/\\__/\\__,_/_/   \\__(_)   \n"
            "                                                           " << endl;

    // Bool to track game state should the player decide to exit early
    bool exitVal = false;

    // Leave the game if player wins, loses, or exits respectively
    while (ThePC->getRespect() < 80 && ThePC->getRespect() > 0 && !exitVal) {

        // save a reference for the current room that the PC resides in for easy referencing
        Room *thisRoom = ThePC->getRoom();

        // Input line for player actions each turn
        cout << endl;
        cout << "Please enter your input:" << endl;
        string inputLine;
        cin >> inputLine;
        cout << endl;

        // use this to prevent any duplicate reactions in a singular all connecting room case
        vector<Creature*> reacted;

        // we have a nonPC character input, as noticed by th colon
        if (inputLine.find(':') != -1) {

            // Parse Creature's ID number
            int creatureNumber = stoi(inputLine.substr(0, inputLine.find(':')));
            // get the rest of the input string as a substring
            string command = inputLine.substr(inputLine.find(':') + 1);
            int index = getIndexByIDInRoom(thisRoom, creatureNumber);
            if (index != -1) {
                // Store a reference for the creature we're dealing with here
                Creature *thisCreature = thisRoom->getPopulation().at(index);
                //commands for this creature
                if (command == "clean") {
                    if (thisRoom->getState() > 0) {
                        // run clean code for this creature
                        thisRoom->setState(thisRoom->getState() - 1);
                        thisCreature->toldToClean(ThePC);
                        // loop through the creatures and update x accordingly if deletions are needed
                        for (int x = 0; x < thisRoom->getPopulation().size(); x++) {
                            // if this creature has not reacted yet
                            if(getIndexByID(reacted,thisRoom->getPopulation().at(x)->getID()) == -1) {
                                reacted.push_back(thisRoom->getPopulation().at(x));
                                x = thisRoom->getPopulation().at(x)->reactToClean(ThePC,x);
                            }
                        }
                    } else
                        cout << "Room is already clean! Try a different command!" << endl;
                }
                if (command == "dirty") {
                    if (thisRoom->getState() < 2) {
                        // run dirty code for this creature
                        thisRoom->setState(thisRoom->getState() + 1);
                        thisCreature->toldToDirty(ThePC);
                        // loop through the creatures and update y accordingly if deletions are needed
                        for (int y = 0; y < thisRoom->getPopulation().size(); y++) {
                            if(getIndexByID(reacted,thisRoom->getPopulation().at(y)->getID()) == -1) {
                                reacted.push_back(thisRoom->getPopulation().at(y));
                                y = thisRoom->getPopulation().at(y)->reactToDirty(ThePC, y);
                            }
                        }
                    } else
                        cout << "Room is already dirty! Try a different command!" << endl;
                }
                if (command == "north") {
                    if (thisRoom->getNorthNeighbor() != nullptr) {
                        thisCreature->move(thisRoom->getNorthNeighbor(), 0);
                    }
                }
                if (command == "south") {
                    if (thisRoom->getSouthNeighbor() != nullptr) {
                        thisCreature->move(thisRoom->getSouthNeighbor(), 1);
                    }
                }
                if (command == "east") {
                    if (thisRoom->getEastNeighbor() != nullptr) {
                        thisCreature->move(thisRoom->getEastNeighbor(), 2);
                    }
                }
                if (command == "west") {
                    if (thisRoom->getWestNeighbor() != nullptr) {
                        thisCreature->move(thisRoom->getWestNeighbor(), 2);
                    }
                }
            } else {
                cout << "Creature not found. Please try again." << endl;
            }
        }

            // we have a PC character input
        else {
            if (inputLine == "exit") {
                exitVal = true;
            }
            if (inputLine == "look") {
                ThePC->look();
            }
            if (inputLine == "clean") {
                if (thisRoom->getState() > 0) {
                    thisRoom->setState(thisRoom->getState() - 1);
                    cout << "You cleaned the room" << endl;
                    // handle AI action...
                    // loop through the creatures and update x accordingly if deletions are needed
                    for (int x = 0; x < thisRoom->getPopulation().size(); x++) {
                        // if this creature has not reacted yet
                        if(getIndexByID(reacted,thisRoom->getPopulation().at(x)->getID()) == -1) {
                            reacted.push_back(thisRoom->getPopulation().at(x));
                            x = thisRoom->getPopulation().at(x)->reactToClean(ThePC, x);
                        }
                    }
                } else
                    cout << "Room is already clean! Try a different command!" << endl;
            }
            if (inputLine == "dirty") {
                if (thisRoom->getState() < 2) {
                    thisRoom->setState(thisRoom->getState() + 1);
                    cout << "You dirtied the room" << endl;
                    // handle AI action...
                    // loop through the creatures and update y accordingly if deletions are needed
                    for (int y = 0; y < thisRoom->getPopulation().size(); y++) {
                        if(getIndexByID(reacted,thisRoom->getPopulation().at(y)->getID()) == -1) {
                            reacted.push_back(thisRoom->getPopulation().at(y));
                            y = thisRoom->getPopulation().at(y)->reactToDirty(ThePC, y);
                        }
                    }
                } else
                    cout << "Room is already dirty! Try a different command!" << endl;
            }
            if (inputLine == "north") {
                ThePC->move(0);
            }
            if (inputLine == "south") {
                ThePC->move(1);
            }
            if (inputLine == "east") {
                ThePC->move(2);
            }
            if (inputLine == "west") {
                ThePC->move(3);
            }
        }
        cout << endl;
        cout << "Current Respect: " << ThePC->getRespect() << endl;
    }

    // Game end conditions
    if (ThePC->getRespect() >= 80) {
        cout << "Respect has reached 80 points! You win! :D" << endl;
        cout << "__  __                        _       __\n"
                "\\ \\/ /___  __  __   _      __(_)___  / /\n"
                " \\  / __ \\/ / / /  | | /| / / / __ \\/ / \n"
                " / / /_/ / /_/ /   | |/ |/ / / / / /_/  \n"
                "/_/\\____/\\__,_/    |__/|__/_/_/ /_(_)   \n"
                "                                        " << endl;
    } else if (ThePC->getRespect() <= 0) {
        cout << "Respect has fallen to 0 points! You lose! D:" << endl;
        cout << "__  __               __                __\n"
                "\\ \\/ /___  __  __   / /___  ________  / /\n"
                " \\  / __ \\/ / / /  / / __ \\/ ___/ _ \\/ / \n"
                " / / /_/ / /_/ /  / / /_/ (__  )  __/_/  \n"
                "/_/\\____/\\__,_/  /_/\\____/____/\\___(_)   \n"
                "                                         " << endl;
    } else {
        cout << "Play again soon!" << endl;
    }
}
