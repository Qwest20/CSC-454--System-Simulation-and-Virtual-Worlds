public class Time implements Comparable<Time>{

    // private Wariables
    private double real; // real time value
    private int count; // 2D time count index

    // Constructor
    public Time (double r, int c){
        real = r;
        count = c;
    }

    // Getters and Setters
    public double getReal() {
        return real;
    }
    public int getC() {
        return count;
    }

    // New CompareTo method
    @Override
    public int compareTo(Time o) {
        // time compareTo from class
        // LOGIC BEHIND THIS:
        // returns the mathematical difference between the times! 0 means same

        final int comparison = Double.compare(real, o.real);
        return (comparison != 0) ? comparison : Integer.compare(count, o.count);
    }
}
