package Couplings;

import Models.AtomicModel;

public class ModelsCoupling<X,Y> extends AtomicCoupling<X, Y> {

    // private references
    private final AtomicModel<X, Y> modelFrom;
    private final AtomicModel<X, Y> modelTo;

    public ModelsCoupling(AtomicModel<X,Y> o, AtomicModel<X,Y> i) {
        this.modelFrom = o;
        this.modelTo = i;
    }

    // getters
    public AtomicModel<X, Y> getModelFrom() {
        return modelFrom;
    }
    public AtomicModel<X, Y> getModelTo() {
        return modelTo;
    }
}
