import java.util.ArrayList;

public class VendingMachineModel {

    // Private Wariables
    private int numQuarters;
    private int numDimes;
    private int numNickels;
    private int value;
    private double time;

    // internally stored event "queue"
    private ArrayList<VendingMachineEvent> vMEList;

    // Constructor
    public VendingMachineModel(){
        numQuarters = 1;
        numDimes = 0;
        numNickels = 0;
        value = 0;
        time = Double.POSITIVE_INFINITY;
        vMEList = new ArrayList<>();
    }

    // Getters and Setters
    public int getNumQuarters() {
        return numQuarters;
    }

    public void setNumQuarters(int numQuarters) {
        this.numQuarters = numQuarters;
    }

    public int getNumDimes() {
        return numDimes;
    }

    public void setNumDimes(int numDimes) {
        this.numDimes = numDimes;
    }

    public int getNumNickels() {
        return numNickels;
    }

    public void setNumNickels(int numNickels) {
        this.numNickels = numNickels;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public double getTime() {
        return this.time;
    }

    public ArrayList<VendingMachineEvent> getVMEList(){
        return this.vMEList;
    }

    public VendingMachineEvent getEvent(int index){
        return this.vMEList.get(index);
    }

    public void addEvent(int index, VendingMachineEvent e){
        vMEList.add(index, e);
    }

    public void removeEvent(int index){
        vMEList.remove(index);
    }

    // Private functions for neater code
    // The calculate functions assist the getChange method
    private int calculateQuarters() {
        int numQuartersOut = 0;
        // if we actually need quarters
        if (this.getValue() / 25 > 0) {
            // either returns enough quarters or returns all quarters in the machine. Whichever is LESS!
            numQuartersOut = Math.min(this.getValue() / 25, this.getNumQuarters());
            // subtract value owed by numQuartersOut many quarters
            this.setValue(this.getValue() - 25 * numQuartersOut);
        }
        return numQuartersOut;
    }

    private int calculateDimes() {
        int numDimesOut = 0;
        // if we actually need dimes
        if (this.getValue() / 10 > 0) {
            // either returns enough dimes or returns all dimes in the machine. Whichever is LESS!
            numDimesOut = Math.min(this.getValue() / 10, this.getNumDimes());
            // subtract value owed by numDimesOut many dimes
            this.setValue(this.getValue() - 10 * numDimesOut);
        }
        return numDimesOut;
    }

    private int calculateNickels() {
        int numNickelsOut = 0;
        // if we actually need nickels
        if (this.getValue() / 5 > 0) {
            // either returns enough nickels
            numNickelsOut = Math.min(this.getValue() / 5, this.getNumNickels());
            // subtract value owed by numNickelsOut many nickels
            this.setValue(this.getValue() - 5 * numNickelsOut);
        }
        return numNickelsOut;
    }

    private String getChange(int type, int numCoffeesServedPrior) {

        // output string will be built upon for the lambda output as we go through
        String output = "";

        // local wariables
        // for output and data management
        int numQuartersOut;
        int numDimesOut;
        int numNickelsOut;
        // hold the amount of coins and the value that exists at the start of this change operation for later reference
        int initNumQuarters = this.getNumQuarters();
        int initNumDimes = this.getNumDimes();
        int initNumNickels = this.getNumNickels();
        int initVal = this.getValue();

        // a modified value that accounts for the coffees made prior to this method call
        int accountedVal = this.getValue() - (100*numCoffeesServedPrior);
        this.setValue(accountedVal);

        // this is the first attempt to find change
        numQuartersOut = calculateQuarters();
        numDimesOut = calculateDimes();
        numNickelsOut = calculateNickels();

        // a modified value to consider the coffees made prior to this
        this.setValue(accountedVal);

        // if we did not get the proper change with the original coin combo, try alternate measures
        // ex. "q=1,d=5,n=0,v=50,c=true" will need a second attempt and will go into this if block
        if (this.getValue() != 0) {

            // use this to see if we have come to an optimal change solution
            boolean success = false;

            // loop until we succeed or fail completely
            while (!success) {

                // reset internal monetary value to what it was originally before trying again
                this.setValue(accountedVal);

                // intentionally exclude a quarter each time we loop
                if (this.getNumQuarters() > 0) {
                    this.setNumQuarters(this.getNumQuarters() - 1);
                }
                // after quarters, intentionally exclude a dime each time we loop
                else if (this.getNumDimes() > 0) {
                    this.setNumDimes(this.getNumDimes() - 1);
                }

                // calculate again
                numQuartersOut = calculateQuarters();
                numDimesOut = calculateDimes();
                numNickelsOut = calculateNickels();

                // if this combo works, call succeed and use it
                if (this.getValue() == 0) {
                    success = true;
                }

                // if we can't get a good combo, even with just nickels, then we're out of luck here...
                else if (getNumQuarters() == 0 && getNumDimes() == 0) {
                    throw new InsufficientChangeException("Inefficient change in machine. Please call 1-800-COMPSCI for support.");
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

    // Lambda function
    public String lambda() {
        
        String output = "";

        // show the event information before showing machine output
        System.out.print(this.vMEList.get(0).getTime() + "," + this.vMEList.get(0).getcValue() + " ");

        // output coffees if possible
        int coffeeCap = this.getValue() / 100;
        if(coffeeCap >= 1)
            System.out.print('c');
        // handle change for lambda with respect to coffees made in the line above
        output += this.getChange(0, coffeeCap);
        return output;
    }

    // this method occurs when the time advance function arrives at 0, or in other words, 2 seconds of waiting has passed
    // this is when we do the math for the change in the machine
    // does not receive input variable here
    public void deltaInternal() {
        // serve coffees and get the change needed
        this.value -= (this.getValue()/100) * 100;
        // since coffees served is a lambda component and not a delta component, we pass in a 0 for the second argument
        getChange(1, 0);
    }

    // this method occurs when we see user input. We read in that input as well as the amount of time that has passed since
    // the time advance function began
    public void deltaExternal(double timeElapsed, char input) {
        switch (input) {
            case 'q':
                this.numQuarters++;
                this.value += 25;
                break;
            case 'n':
                this.numNickels++;
                this.value += 5;
                break;
            case 'd':
                this.numDimes++;
                this.value += 10;
                break;
            default:
                break;
        }
    }

    // this method occurs when we see user input at exactly 2 seconds passed.
    // We read in that input, but assume a time elapsed of 2 seconds
    public void deltaConfluent(char input) {
        // serve coffees and get the change needed
        this.value -= (this.getValue()/100) * 100;
        switch (input) {
            case 'q':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this.numQuarters++;
                this.value = 25;
                break;
            case 'n':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this.numNickels++;
                this.value += 5;
                break;
            case 'd':
                // handle change for lambda with respect to coffees made in the line above
                getChange(1, 0);
                this.numDimes++;
                this.value += 10;
                break;
            default:
                break;
        }
    }

    // time advancement function handles the amount of time stamped for planned output events
    // if an output is planned with respect to ta() -> 2, then we will plan on an output event
    // 2 seconds after the corresponding deltaExternal or deltaConfluent call.
    // otherwise, a time of INFINITY will induce an output that will never actually be used in our event queue
    public void timeAdvancement (){
        if(this.value == 0){
            this.time = Double.POSITIVE_INFINITY;
        }
        else{
            this.time = 2;
        }
    }
}
