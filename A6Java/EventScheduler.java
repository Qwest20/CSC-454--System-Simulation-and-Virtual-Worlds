import Models.AtomicModel;

import java.util.ArrayList;

public class EventScheduler<X, Y> {

    // Member variables of this class
    private final ArrayList<Event<X, Y>> Heap;

    // Constructor of this class
    public EventScheduler() {
        this.Heap = new ArrayList<>();
    }

    // Returning the position of
    // the parent for the node currently
    // at pos
    private int parent(int pos) {
        if (pos % 2 != 0) {
            return pos / 2;
        } else {
            return pos / 2 - 1;
        }
    }

    // Returning the position of the
    // left child for the node currently at pos
    private int leftChild(int pos) {
        return (2 * pos) + 1;
    }

    // Returning the position of
    // the right child for the node currently
    // at pos
    private int rightChild(int pos) {
        return (2 * pos) + 2;
    }

    // To swap two nodes of the heap
    private void swap(int fpos, int spos) {
        Event<X, Y> tmp;
        tmp = Heap.get(fpos);
        Heap.set(fpos, Heap.get(spos));
        Heap.set(spos, tmp);
    }

    // To insert a node into the heap
    public void insert(Event<X, Y> element) {

        Heap.add(Heap.size(), element);
        int current = Heap.size() - 1;

        while (current > 0 && Heap.get(current).compareTo(Heap.get(parent(current))) < 0) {
            swap(current, parent(current));
            current = parent(current);
        }
    }

    // To print the contents of the heap at each parent and their respective children
    public void print() {
        System.out.println("Here is the current size of the heap: " + Heap.size());

        System.out.println("Here is the heap as an array:");
        for (Event<X, Y> current : Heap) {
            System.out.println(current.getMoment().getReal() + ", " + current.getMoment().getC() + ", " +
                    current.getMachine().getName() + ", " + current.getInput() + ", " + current.getType());
        }
//
//        for (int i = 0; i <= Heap.size() / 2; i++) {
//
//            Event<X, Y> current = Heap.get(i);
//            Event<X, Y> leftChild = null;
//            Event<X, Y> rightChild = null;
//            if((2*i)+1 < Heap.size()){
//                leftChild = Heap.get((2*i)+1);
//            }
//            if((2*i)+2 < Heap.size()){
//                rightChild = Heap.get((2*i)+2);
//            }
//
//            // Printing the parent and both children
//            System.out.println("Parent: " + current.getMoment().getReal() + ", " + current.getMoment().getC() + ", " +
//                    current.getMachine().getName() + ", " + current.getInput() + ", " + current.getType());
//            if (leftChild != null) {
//                System.out.println("Left Child: " + leftChild.getMoment().getReal() + ", " + leftChild.getMoment().getC() + ", " +
//                        leftChild.getMachine().getName() + ", " + leftChild.getInput() + ", " + leftChild.getType());
//            }
//            if (rightChild != null) {
//                System.out.println("Right Child: " + rightChild.getMoment().getReal() + ", " + rightChild.getMoment().getC() + ", " +
//                        rightChild.getMachine().getName() + ", " + rightChild.getInput() + ", " + rightChild.getType());
//            }
//            System.out.println();
//        }
    }

    // To remove and return the minimum
    // element from the heap
    public Event<X, Y> poll() {

        // read the top event for removal
        Event<X, Y> polled = Heap.get(0);
        Heap.remove(0);

        // read the greatest event as the new min, and then remove the duplicate at the end
        if (Heap.size() > 0) {
            Heap.add(0, Heap.get(Heap.size() - 1));
            Heap.remove(Heap.size() - 1);

            int i = 0;
            while (i < Heap.size() - 1) {
                // if the left child is smaller than the parent AND THE RIGHT IF IT EX, YISTS
                if (((leftChild(i) < Heap.size() - 1 &&
                        Heap.get(leftChild(i)).compareTo(Heap.get(rightChild(i))) <= 0) ||
                        (leftChild(i) == Heap.size() - 1)) &&
                        Heap.get(i).compareTo(Heap.get(leftChild(i))) > 0) {
                    swap(i, leftChild(i));
                    i = leftChild(i);
                }
                // otherwise, if the right child is smaller than the parent AND THE LEFT
                else if (rightChild(i) <= Heap.size() - 1 && Heap.get(i).compareTo(Heap.get(rightChild(i))) > 0) {
                    swap(i, rightChild(i));
                    i = rightChild(i);
                } else
                    break;
            }
        }
        return polled;
    }

    public boolean isNotEmpty() {
        return !Heap.isEmpty();
    }

    // run me before you poll at each step in the simulation
    public void checkForConfluence() {

        // navigate through the tree and see if two events eX, Yist such that
        // we need a new confluent event to replace it. This occurs if the machine
        // is the same and the events occur at the same time
        // we process this potentially on both sides since I'm not quite sure where
        // confluent events would be just yet in the tree ordering

        // Event variables to potentially be used here
        Event<X, Y> e, con;
        Time t;

        // if they both have the same machine and real time, there's a confluent case!
        if (Heap.size() > 1 && Heap.get(leftChild(0)).getMachine().equals(Heap.get(0).getMachine()) &&
                Heap.get(leftChild(0)).getMoment().getReal() - Heap.get(0).getMoment().getReal() == 0) {
            // snag one of the events and remove the other as well
            // if the first is the input event
            if(Heap.get(0).getType().equals(Event.Type.INPUT)){
                e = poll();
                poll();
            }
            // if the second is the input event
            else{
                poll();
                e = poll();
            }
            // we have the information to mark a confluent event which will be run when we call poll in the next iteration
            t = e.getMoment();
            con = new Event<>(t, e.getMachine(), e.getInput(), Event.Type.CONFLUENT);
            this.insert(con);
        } else if (Heap.size() > 2 && Heap.get(rightChild(0)).getMachine().equals(Heap.get(0).getMachine()) &&
                Heap.get(rightChild(0)).getMoment().getReal() - Heap.get(0).getMoment().getReal() == 0) {
            // if the first is the input event
            if(Heap.get(0).getType().equals(Event.Type.INPUT)){
                e = poll();
                poll();
            }
            // if the second is the input event
            else{
                poll();
                e = poll();
            }
            // we have the information to mark a confluent event which will be run when we call poll in the neX, Yt iteration
            t = e.getMoment();
            con = new Event<>(t, e.getMachine(), e.getInput(), Event.Type.CONFLUENT);
            this.insert(con);
        }
    }

    public Event<X, Y> peek() {
        return Heap.get(0);
    }

    public void removeOutputEventByMachine(AtomicModel<X, Y> machine) {
        int index = indexOfMachineOutput(machine);
        if (index != -1) {
            Heap.remove(index);
        }
    }

    private int indexOfMachineOutput(AtomicModel<X, Y> machine) {
        for (int i = 0; i < Heap.size(); i++) {
            if (Heap.get(i).getMachine() == machine && Heap.get(i).getType().equals(Event.Type.OUTPUT)) {
                return i;
            }
        }
        // if not found
        return -1;
    }
}
