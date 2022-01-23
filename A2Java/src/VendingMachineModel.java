public class VendingMachineModel {

    // Wariables
    private int numQuarters = 10;
    private int numDimes = 10;
    private int numNickels = 10;
    private int value = 0;
    private boolean changeButtonPressed = false;

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

    public boolean isChangeButtonPressed() {
        return changeButtonPressed;
    }

    public void setChangeButtonPressed(boolean changeButtonPressed) {
        this.changeButtonPressed = changeButtonPressed;
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

    private void getChange(int type) {

        // local wariables
        // for output and data management
        int numQuartersOut = 0;
        int numDimesOut = 0;
        int numNickelsOut = 0;
        // hold the amount of coins and the value that exists at the start of this change operation
        int initNumQuarters = this.getNumQuarters();
        int initNumDimes = this.getNumDimes();
        int initNumNickels = this.getNumNickels();
        int initVal = this.getValue();

        // this is the first attempt to find change
        numQuartersOut = calculateQuarters();
        numDimesOut = calculateDimes();
        numNickelsOut = calculateNickels();

        // if we did not get the proper change with the original coin combo, try alternate measures
        // ex. "q=1,d=5,n=0,v=50,c=true" will need a second attempt and will go into this if block
        if (this.getValue() != 0) {

            // use this to see if we have come to an optimal change solution
            boolean success = false;

            // loop until we succeed or fail completely
            while (!success) {

                // reset internal monetary value to what it was originally before trying again
                this.setValue(initVal);

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
                System.out.println("Here is your change:");
                for (int q = 0; q < numQuartersOut; q++) {
                    System.out.print("q");
                }
                for (int d = 0; d < numDimesOut; d++) {
                    System.out.print("d");
                }
                for (int n = 0; n < numNickelsOut; n++) {
                    System.out.print("n");
                }

                if (numQuartersOut == 0 && numDimesOut == 0 && numNickelsOut == 0) {
                    System.out.println("Nothing");
                }
                System.out.println();
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
                this.setChangeButtonPressed(false);
                break;
        }
    }

    // Lambda function:
    // Takes in State from this class
    // Outputs string to console
    public void lambda() {

        // if both coffee and changeButton
        if (this.value >= 100 && this.isChangeButtonPressed()) {
            // output coffees
            int coffeeCap = this.getValue() / 100;
            System.out.println("You got " + coffeeCap + " coffees!");
            System.out.println("Please wait. Change is queued for next tick.");
        }

        // if value >= 100
        else if (this.value >= 100) {
            // output coffees
            int coffeeCap = this.getValue() / 100;
            System.out.println("You got " + coffeeCap + " coffees!");
        }

        // if the change button is pressed and we don't have any value in the machine
        else if (this.isChangeButtonPressed() && value == 0) {
            System.out.println("System cancelled with no change to give.");
            System.out.println("Have a good day!");
            System.exit(0);
        }

        // if the change button is pressed and we don't have coffees to worry about and we DO have change to give
        else if (this.isChangeButtonPressed()) {
            // handle change for lambda
            this.getChange(0);
        }

        // if no coffee output, lambda produces nothing
        else
            System.out.println("Machine produces nothing.");
    }

    // Delta function:
    // Takes in input string
    // Updates the State in this class
    public void delta(String input) {

        // ACTION BASED ON PREVIOUS LINES

        // boolean value to see if we have given a coffee on this tick
        boolean coffeeReceived = false;

        // Handle coffees
        while (this.getValue() / 100 > 0) {
            this.setValue(this.getValue() - 100);
            coffeeReceived = true;
        }

        // Handle change, but only if we haven't given a coffee on this tick
        if (isChangeButtonPressed() && !coffeeReceived) {
            // handle change for delta
            this.getChange(1);
        }

        // ACTIONS BASED ON THIS LINE

        // how many of these inputs are we putting into the machine?
        int numQs = 0;
        int numDs = 0;
        int numNs = 0;
        boolean containsC = false;

        // read through the characters
        for (int i = 0; i < input.length(); i++) {
            if (input.charAt(i) == 'q') {
                numQs++;
            }
            if (input.charAt(i) == 'd') {
                numDs++;
            }
            if (input.charAt(i) == 'n') {
                numNs++;
            }
            if (input.charAt(i) == 'c') {
                containsC = true;
            }
        }

        // update coins
        this.numQuarters += numQs;
        this.numDimes += numDs;
        this.numNickels += numNs;

        // update value
        int addedTotal = (numQs * 25 + numDs * 10 + numNs * 5);
        this.setValue(this.getValue() + addedTotal);

        // update change button pressed
        if (containsC) {
            setChangeButtonPressed(true);
        }
    }
}
