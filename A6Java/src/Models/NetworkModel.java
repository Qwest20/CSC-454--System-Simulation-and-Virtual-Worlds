package Models;
import Couplings.InputCoupling;
import Couplings.ModelsCoupling;
import Couplings.OutputCoupling;

import java.util.ArrayList;

public class NetworkModel<X, Y> {

    // Arraylist to store models (do atomics need to have different types?)
    private final ArrayList<AtomicModel<X,Y>> modelCollection = new ArrayList<>();
    private final ArrayList<ModelsCoupling<X,Y>> couplingsCollection = new ArrayList<>();
    private final InputCoupling<X, Y> inputCoupling;
    private final OutputCoupling<X, Y> outputCoupling;

    public NetworkModel(InputCoupling<X, Y> iC, OutputCoupling<X, Y> oC) {
        this.inputCoupling = iC;
        this.outputCoupling = oC;
    }

    // return the lambda of the last modelCollection model as the network model's output
    public Y lambda() {
        // local variable for some short output coupling here...
        Y output = (Y) this.outputCoupling.take();
        return output;
    }

    public void deltaInt(AtomicModel<X, Y> aM) {
        // find the index of this model in our collection and run it this way
        int index = modelCollection.indexOf(aM);
        modelCollection.get(index).deltaInt();
    }

    public void deltaCon(AtomicModel<X, Y> aM, X input) {
        // find the index of this model in our collection and run it this way
        int index = modelCollection.indexOf(aM);
        modelCollection.get(index).deltaCon(input);
    }

    public void deltaExt(AtomicModel<X, Y> aM, X input, double elapsedTime) {
        // find the index of this model in our collection and run it this way
        int index = modelCollection.indexOf(aM);
        modelCollection.get(index).deltaExt(input, elapsedTime);
    }

    public void addModel(AtomicModel<X, Y> a) {
        modelCollection.add(a);
    }

    public AtomicModel<X, Y> getModelAt(int index) {
        return modelCollection.get(index);
    }

    public ArrayList<AtomicModel<X, Y>> getModelCollection() {
        return modelCollection;
    }

    public void addCoupling(ModelsCoupling<X, Y> c) {
        couplingsCollection.add(c);
    }

    public ArrayList<ModelsCoupling<X, Y>> getCouplingsCollection() {
        return couplingsCollection;
    }

    public InputCoupling<X, Y> getInputCoupling() {
        return inputCoupling;
    }

    public OutputCoupling<X, Y> getOutputCoupling() {
        return outputCoupling;
    }

    public ModelsCoupling<X,Y> findCouplingByModel(AtomicModel<X, Y> machine, int type) {
        for (ModelsCoupling<X, Y> xyModelsCoupling : couplingsCollection) {
            // input event
            if (xyModelsCoupling.getModelTo().equals(machine) && type == 1) {
                return xyModelsCoupling;
            }
            // output event
            else if (xyModelsCoupling.getModelFrom().equals(machine) && type == 0) {
                return xyModelsCoupling;
            }
        }
        // no coupling found with this machine
        return null;
    }
}
