public class Event implements Comparable<Event>{

    // Establish enumeration for event typing (notice no confluent events here)
    enum Type {INPUT, OUTPUT, CONFLUENT}

    // Private wariables
    private final Time moment;
    private final Model model;
    private final int numPartsIn;
    private final Event.Type type;

    // Constructors
    // input and confluent events
    public Event (Time m, Model ma, int p, Event.Type t){
        moment = m;
        model = ma;
        numPartsIn = p;
        type = t;
    }
    // output events (no parts in assumes an output event)
    public Event (Time m, Model ma){
        moment = m;
        model = ma;
        numPartsIn = 0;
        type = Type.OUTPUT;
    }

    // Getters and Setters
    public Time getMoment(){
        return this.moment;
    }
    public Model getModel(){
        return this.model;
    }
    public Type getType(){
        return this.type;
    }
    public int getNumPartsIn() { return this.numPartsIn; }

    // CompareTo (carries over logic from time compareTo method)
    @Override
    public int compareTo(Event o) {
        return this.moment.compareTo(o.moment);
    }
}
