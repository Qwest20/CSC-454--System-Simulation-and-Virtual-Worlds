public class NetworkModel {

    // private state bit
    private int state = 0;

    // private coupling variables

    // Models
    private XORModel x1 = new XORModel();
    private XORModel x2 = new XORModel();
    private MemoryModel m = new MemoryModel();
    private int x1Out = 0;
    private int x2Out = 0;
    private int memOut = 0;

    // Getters and Setters


    public XORModel getX1() {
        return x1;
    }

    public XORModel getX2() {
        return x2;
    }

    public MemoryModel getM() {
        return m;
    }

    // S -> Y
    public int lambda() {
        // output the state of the network model
        // only output on every NETWORK tick, not every ATOMIC tick!
        System.out.println("NETWORK.lambda() = ");
        return state;
    }

    public void coupling(){
        // coupling variables set for later
        x1Out = x1.lambda();
        x2Out = x2.lambda();
        memOut = m.lambda();
        state = x2Out;
    }

    // (X,S) -> S
    public void delta(int a, int b) {

        // inner deltas
        x1.delta(a,b);
        x2.delta(x1Out, memOut);
        m.delta(x2Out);
    }
}
