package Couplings;

import Models.AtomicModel;

public class OutputCoupling <X,Y> extends AtomicCoupling<X, Y> {

    // private reference
    private final AtomicModel<X, Y> modelFrom;

    public OutputCoupling(AtomicModel<X,Y> o) {
        this.modelFrom = o;
    }

    // getters
    public AtomicModel<X, Y> getModelFrom() {
        return modelFrom;
    }
}
