import java.util.ArrayList;

// A Min Heap Priority Queue
public class EventScheduler {

    // Member variables of this class
    private final ArrayList<Event> Heap;

    // Constructor of this class
    public EventScheduler() {
        this.Heap = new ArrayList<>();
    }

    // Returning the position of the parent for the node currently at pos
    private int parent(int pos) {
        if (pos % 2 != 0) {
            return pos / 2;
        } else {
            return pos / 2 - 1;
        }
    }

    // Returning the position of the left child for the node currently at pos
    private int leftChild(int pos) {
        return (2 * pos) + 1;
    }

    // Returning the position of the right child for the node currently at pos
    private int rightChild(int pos) {
        return (2 * pos) + 2;
    }

    // To swap two nodes of the heap
    private void swap(int fpos, int spos) {
        Event tmp;
        tmp = Heap.get(fpos);
        Heap.set(fpos, Heap.get(spos));
        Heap.set(spos, tmp);
    }

    // To insert a node into the heap
    public void insert(Event element) {

        // add the event to the heap at the end
        Heap.add(Heap.size(), element);
        int current = Heap.size() - 1;

        // now move it to where it ought to go
        while (current > 0 && Heap.get(current).compareTo(Heap.get(parent(current))) < 0) {
            swap(current, parent(current));
            current = parent(current);
        }
    }

    // To return the minimum element from the heap WITHOUT removing it
    public Event peek() {
        return Heap.get(0);
    }

    // To remove and return the minimum element from the heap
    public Event poll() {

        // read the first event for removal
        Event polled = Heap.get(0);
        Heap.remove(0);

        // read the greatest event as the new min, and then remove the duplicate at the end
        if (Heap.size() > 0) {
            Heap.add(0, Heap.get(Heap.size() - 1));
            Heap.remove(Heap.size() - 1);

            int i = 0;
            while (i < Heap.size() - 1) {
                // if the left child is smaller than the parent AND THE RIGHT IF IT EXISTS
                if (leftChild(i) <= Heap.size() - 1 &&
                        Heap.get(leftChild(i)).compareTo(Heap.get(i)) < 0 &&
                        ((leftChild(i) == Heap.size() - 1) ||
                                (Heap.get(leftChild(i)).compareTo(Heap.get(rightChild(i))) <= 0))){
                    swap(i, leftChild(i));
                    i = leftChild(i);
                }
                // otherwise, if the right child is smaller than the parent AND THE LEFT
                else if (rightChild(i) <= Heap.size() - 1 &&
                        Heap.get(rightChild(i)).compareTo(Heap.get(i)) < 0 &&
                        (Heap.get(rightChild(i)).compareTo(Heap.get(leftChild(i))) <= 0)){
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

    // run this before you poll at each step in the simulation
    public void checkForConfluence() {

        // Event variables to potentially be used here
        Event e, con;
        Time t;

        // if they both have the same machine and real time, there's a confluent case
        // (but we need to check which side has it...)
        if (
                /* LEFT SIDE */
                (Heap.size() > 1 && Heap.get(leftChild(0)).getModel().equals(Heap.get(0).getModel()) &&
                Heap.get(leftChild(0)).getMoment().getReal() - Heap.get(0).getMoment().getReal() == 0) ||

                /* RIGHT SIDE */
                (Heap.size() > 2 && Heap.get(rightChild(0)).getModel().equals(Heap.get(0).getModel()) &&
                Heap.get(rightChild(0)).getMoment().getReal() - Heap.get(0).getMoment().getReal() == 0)){

            // snag one of the events and remove the other as well
            // if the first is the input event
            if(peek().getType().equals(Event.Type.INPUT)){
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
            con = new Event(t, e.getModel(), e.getNumPartsIn(), Event.Type.CONFLUENT);
            this.insert(con);
        }
    }

    // used for event cleaning (only one output event at a time)
    public void removeOutputEventByMachine(Model machine) {
        for (int i = 0; i < Heap.size(); i++) {
            if (Heap.get(i).getModel() == machine && Heap.get(i).getType().equals(Event.Type.OUTPUT)) {
                Heap.remove(i);
                break;
            }
        }
    }

    // To print the contents of the heap at each parent and their respective children (used for troubleshooting
    public void print() {
        System.out.println("Here is the current size of the heap: " + Heap.size());

        System.out.println("Here is the heap as an array:");
        for (Event current : Heap) {
            System.out.println(current.getMoment().getReal() + ", " + current.getMoment().getC() + ", " +
                    current.getModel().getName() + ", " + current.getNumPartsIn() + ", " + current.getType());
        }
    }
}
