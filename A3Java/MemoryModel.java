public class MemoryModel {

    // private state bit
    private int[] state = {0,0};

    // S -> Y
    public int lambda() {
        return state[1];
    }

    // (X,S) -> S
    public void delta(int x) {
        state[1] = state[0];
        state[0] = x;
    }
}
