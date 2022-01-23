public class Model {

    // private wariables
    private final String name;
    private final double processingTime;
    private final int iD;
    private int numParts;
    private double timeRemaining;

    // variable to help keep track of passed time since input (modified later)
    double timeOfLastInput = -1;

    // Constructor
    public Model(String n, double t, int i){
        name = n;
        processingTime = t;
        iD = i;
        numParts = 0;
        timeRemaining = 0;
    }

    // Getters and Setters
    public String getName(){
        return this.name;
    }
    public int getNumParts() { return this.numParts; }
    public double getTimeOfLastInput() {
        return timeOfLastInput;
    }
    public void setTimeOfLastInput(double elapsedTime) {
        timeOfLastInput = elapsedTime;
    }
    public double getProcessingTime() {
        return processingTime;
    }
    public int getID() {
        return iD;
    }

    // lambda
    public int lambda(){
        // simply spit out a processed ball bearing as a 1
        return 1;
    }

    // deltaInt
    // called when a part is completed
    public void deltaInt(){
        // finish a part
        numParts--;
        // and move on to the next one with processingTime remaining
        timeRemaining = processingTime;
    }

    // deltaExt
    // called when ball bearings are added to this model's bin
    public void deltaExt(int incoming, double elapsedTime){
        // if there's no part being processed in the machine before processing these new ones
        if(numParts == 0){
            // work on this one with processingTime remaining
            timeRemaining = processingTime;
        }
        // if there are already parts in the bin as these are inserted
        else{
            // prepare to continue based on the current operation,
            // since there has to be a ball bearing in the machine already
            timeRemaining -= elapsedTime;
        }
        // insert the inputted ball bearings now that time has been updated
        numParts+=incoming;
    }

    public void deltaCon(int incoming){
        // work on this one with processingTime remaining
        timeRemaining = processingTime;
        // insert the inputted ball bearings (without the one we just finished) now that time has been updated
        numParts+=incoming-1;
    }

    // time advancement (same as we've done in other projects)
    public double timeAdvance(){
        if(numParts == 0){
            return Double.POSITIVE_INFINITY;
        }
        else{
            return timeRemaining;
        }
    }
}
