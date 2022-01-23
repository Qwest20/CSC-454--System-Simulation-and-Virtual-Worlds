public class Couple {

    // private value held in here like a basket essentially
    private int val;

    // constructor
    public Couple(){
        val = 0;
    }

    // getters and setters
    public int take() {
        return val;
    }
    public void give(int i){
        val = i;
    }
}
