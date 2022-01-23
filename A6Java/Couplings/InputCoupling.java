package Couplings;

import Models.AtomicModel;

import java.util.ArrayList;

public class InputCoupling <X,Y> extends AtomicCoupling<X, Y> {

    // private reference
    private final AtomicModel<X, Y> modelTo;

    public InputCoupling(AtomicModel<X,Y> i) {
        this.modelTo = i;
    }

    // getters
    public AtomicModel<X, Y> getModelTo() {
        return modelTo;
    }
}
