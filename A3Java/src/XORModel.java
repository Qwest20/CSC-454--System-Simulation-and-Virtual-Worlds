public class XORModel {

    private int state = 0;

    // S -> Y
    public int lambda() {
        return state;
    }

    // (X,S) -> S
    public void delta(int a, int b) {
        state = a^b;
    }
}