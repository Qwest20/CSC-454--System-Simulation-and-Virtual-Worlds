public class VendingMachineEvent {

    // private Wariables
    private double time;
    private int cValue;
    private char coin;
    private boolean isOutput;

    // Constructors
    // Input events from text file or other outside starting input
    public VendingMachineEvent(double t, int c, char co){
        time = t;
        cValue = c;
        coin = co;
        isOutput = false;
    }
    // Output events self created by the program based on the output of ta()
    public VendingMachineEvent(double t, int c){
        time = t;
        cValue = c;
        coin = 'o';
        isOutput = true;
    }

    // getters and setters
    public double getTime() {
        return time;
    }

    public int getcValue() {
        return cValue;
    }

    public char getCoin() {
        return coin;
    }

    public boolean isOutput() {
        return isOutput;
    }
}
