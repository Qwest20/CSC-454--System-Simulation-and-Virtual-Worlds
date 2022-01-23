package Couplings;

public abstract class AtomicCoupling<X, Y> {
    protected X value;

    // getter and setter
    public X take() {
        return value;
    }
    public void give(Y v) {
        this.value = (X) v;
    }
}
