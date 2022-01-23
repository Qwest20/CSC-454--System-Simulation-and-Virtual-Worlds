import Models.AtomicModel;

public class Event<X,Y> implements Comparable<Event<X,Y>>{


    // Establish enumeration:
    public enum Type {INPUT, OUTPUT, CONFLUENT}
    public static final int INPUT = 0;
    public static final int OUTPUT = 1;
    public static final int CONFLUENT = 2;

    // private Wariables
    private final Time moment;
    private final AtomicModel<X,Y> machine;
    private X input;
    private final Type type;

    // Constructor

    // input and confluent events
    public Event(Time m, AtomicModel<X,Y> ma, X p, Type t){
        moment = m;
        machine = ma;
        input = p;
        type = t;
    }

    // output events
    public Event(Time m, AtomicModel<X,Y> ma){
        moment = m;
        machine = ma;
        type = Type.OUTPUT;
    }

    // Getters and Setters
    public Time getMoment(){
        return this.moment;
    }
    public AtomicModel<X,Y> getMachine(){
        return this.machine;
    }
    public Type getType(){
        return this.type;
    }
    public X getInput() { return this.input; }

    // CompareTo
    @Override
    public int compareTo(Event o) {
        // if they're the same at the time level
        if(this.moment.compareTo(o.getMoment()) == 0){
            // outputs come before other events at the same time.
            // lambda before delta ;)
            if(this.getType().equals(Event.Type.OUTPUT)){
                return -1;
            }
            else{
                return 1;
            }
        }
        return this.moment.compareTo(o.getMoment());
    }
}
