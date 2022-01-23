package Models;

// Generics for input and output type
public abstract class AtomicModel<X,Y> {

    // state (p = num inputs in the bin. s = time remaining to process the current part)
    protected int p;
    protected double s;

    // private wariables
    protected final String name;
    protected final double processingTime;
    // start at -1 ie before simulation begins
    protected double timeOfLastInput = -1;

    // constructor
    public AtomicModel(String n, double pT){
        this.name = n;
        this.processingTime = pT;
    }

    // getters and setters
    public String getName(){ return this.name; }
    public double getTimeOfLastInput(){ return this.timeOfLastInput; }
    public double getProcessingTime() { return processingTime; }
    public void setTimeOfLastInput(double t) { this.timeOfLastInput = t; }
    public int getP() {
        return p;
    }
    public double getS() {
        return s;
    }

    // abstract classes
    public abstract Y lambda();
    public abstract void deltaInt();
    public abstract void deltaCon(X input);
    public abstract void deltaExt(X input, double elapsedTime);
    public abstract double timeAdvance();
}
