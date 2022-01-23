import Couplings.AtomicCoupling;
import Models.AtomicModel;
import Models.NetworkModel;
import java.util.ArrayList;

// TO DO
// Look at event creations below. Consider where to logically create events, as well as the advancement component
// Java lambdas to allow for universal input type casting? Talk to Billy and Professor

public class Framework<X,Y> {

    private double elapsedTime = 0;
    private final EventScheduler<X,Y> pQ = new EventScheduler<>();

    public void simulate(NetworkModel<X,Y> nM, ArrayList<Event<X,Y>> inputs){

        // first, add the events we got to the eventScheduler
        for (Event<X,Y> input : inputs) {
            pQ.insert(input);
        }
        
        // next, add output events for all the models at t=MAX.VALUE, to appropriate with the
        // "there is always one output event" rule
        for (int i = 0; i < nM.getModelCollection().size(); i++) {
            AtomicModel<X,Y> aM = nM.getModelAt(i);
            Time t = new Time(aM.timeAdvance(),i);
            Event<X,Y> e = new Event<>(t,aM);
            pQ.insert(e);
        }

        // simulation loop that stops once we hit output events of positive infinity or no events at all
        while(pQ.isNotEmpty() && pQ.peek().getMoment().getReal() != Double.MAX_VALUE){

            // check at the top will ensure that we don't have any unaddressed confluent events given the following criteria
            // confluents have the same real and c values on the same machine
            pQ.checkForConfluence();

            // now that we have checked for confluents, we can poll the event
            Event<X,Y> top = pQ.poll();

            // update elapsed time to this event
            elapsedTime = top.getMoment().getReal();

            // process the event based on the type
            if(top.getType() == Event.Type.INPUT){

                // COUPLING
                // Store input variable for later reference
                X input;
                // If we're looking at the first machine in the NM
                if(top.getMachine().equals(nM.getInputCoupling().getModelTo())){
                    // read from the input coupling model's value
                    // we need give and take at the same step
                    nM.getInputCoupling().give((Y) top.getInput());
                    input = nM.getInputCoupling().take();
                }
                else{
                    // find the input coupling for this machine
                    AtomicCoupling<X,Y> ourCouple = nM.findCouplingByModel(top.getMachine(),1);
                    input = ourCouple.take();
                }

                // derive a timePassed variable, so we know how much time has passed since the last input event
                // cases in which there is NO part already in there are covered with the if conditions
                // - we're at t=-1 (start of simulation
                // - OR there were no parts in the machine before this input, and therefore no time should be subtracted
                double timePassed;
                if(top.getMachine().getTimeOfLastInput() == -1 ||
                        elapsedTime - top.getMachine().getTimeOfLastInput() > top.getMachine().getProcessingTime())
                    timePassed = 0;
                else
                    timePassed = elapsedTime - top.getMachine().getTimeOfLastInput();
                // run this particular machine's delta external
                nM.deltaExt(top.getMachine(),input,timePassed);
                // update the time of last input, since we have taken in a new input at this time
                top.getMachine().setTimeOfLastInput(elapsedTime);

                // Corresponding output event created here
                // (only do this if s = processing time, since we know we can process a new input)
                if(top.getMachine().getS() == top.getMachine().getProcessingTime()) {
                    double r = elapsedTime + top.getMachine().timeAdvance();
                    // c value will be equivalent to the machine's index
                    int c = nM.getModelCollection().indexOf(top.getMachine());
                    Time t = new Time(r, c);
                    // output from this same machine at the designated time
                    Event<X, Y> newO = new Event<>(t, top.getMachine());

                    // remove the old event and add the new
                    pQ.removeOutputEventByMachine(top.getMachine());
                    pQ.insert(newO);
                }
            }
            else if(top.getType() == Event.Type.OUTPUT) {

                // COUPLING
                // Store output variable for later reference
                Y output = top.getMachine().lambda();
                // If we're looking at the last machine in the NM
                if(top.getMachine().equals(nM.getOutputCoupling().getModelFrom())) {

                    // set the network model's output coupling to this value
                    nM.getOutputCoupling().give(output);

                    // perform the network model's lambda here, since now we can!
                    // (also functions as outputCoupling's take method)
                    System.out.println(this.elapsedTime + " " + nM.lambda());

                } else {
                    // find the output coupling for this machine
                    AtomicCoupling<X, Y> ourCouple = nM.findCouplingByModel(top.getMachine(), 0);
                    ourCouple.give(output);
                }

                // run this particular machine's deltaInt (p is decremented here)
                nM.deltaInt(top.getMachine());

                // if this model is going to another model in any case, we need to make a new input event
                if(nM.findCouplingByModel(top.getMachine(), 0) != null){

                    // Corresponding input event created here
                    // receive information on the next model in the network model based on the couplings
                    AtomicModel<X, Y> nextMachine = nM.findCouplingByModel(top.getMachine(), 0).getModelTo();

                    // time will be the SAME for the coming input event as the current output event!
                    double r = elapsedTime;
                    // c value will be the value of the next machine
                    Time t = new Time(r, nM.getModelCollection().indexOf(nextMachine));
                    // input from this same machine at the designated time
                    Event<X, Y> newI = new Event<>(t, nextMachine, (X) output, Event.Type.INPUT);

                    // add the new input event for the next model in the sequence
                    pQ.insert(newI);
                }

                // if the top machine has a bin of processing inputs to work with,
                // then we'll need to schedule a new output event accordingly
                if (top.getMachine().getP() > 0) {
                    // new output event on the top machine since we have finished processing a part
                    double re = elapsedTime + top.getMachine().timeAdvance();
                    // c value will be the same
                    Time ti = new Time(re, nM.getModelCollection().indexOf(top.getMachine()));
                    // output from this same machine at the designated time
                    Event<X, Y> newOu = new Event<>(ti, top.getMachine());

                    // remove the old event (if it exists) and add the new one
                    // (shouldn't find one in this case though...)
                    pQ.removeOutputEventByMachine(top.getMachine());
                    pQ.insert(newOu);
                }
            }
            else{
                // COUPLING
                // OUTPUT FIRST
                Y output = top.getMachine().lambda();
                // If we're looking at the last machine in the NM
                if(top.getMachine().equals(nM.getOutputCoupling().getModelFrom())) {

                    // set the network model's output coupling to this value
                    nM.getOutputCoupling().give(output);

                    // perform the network model's lambda here, since now we can!
                    // (also functions as outputCoupling's take method)
                    System.out.println(this.elapsedTime + " " + nM.lambda());

                } else {
                    // find the output coupling for this machine
                    AtomicCoupling<X, Y> ourCouple = nM.findCouplingByModel(top.getMachine(), 0);
                    ourCouple.give(output);
                }
                // INPUT SECOND
                // Store input variable for later reference
                X input;
                // If we're looking at the first machine in the NM
                if(top.getMachine().equals(nM.getInputCoupling().getModelTo())){
                    // read from the input coupling model's value
                    // we need give and take at the same step
                    nM.getInputCoupling().give((Y) top.getInput());
                    input = nM.getInputCoupling().take();
                }
                else{
                    // find the input coupling for this machine
                    AtomicCoupling<X,Y> ourCouple = nM.findCouplingByModel(top.getMachine(),1);
                    input = ourCouple.take();
                }

                // update the time of last input, since we have taken in a new input at this time
                top.getMachine().setTimeOfLastInput(elapsedTime);

                // call deltaCon (no need to worry about time passed, since processing time will be reset)
                nM.deltaCon(top.getMachine(),input);

                // EVENT CREATION
                // output first
                // Corresponding output event created here
                // this should run in every case, since we just ran deltaCon to reset S
                if(top.getMachine().getS() == top.getMachine().getProcessingTime()) {
                    double r = elapsedTime + top.getMachine().timeAdvance();
                    // c value will stay the same
                    Time t = new Time(r, nM.getModelCollection().indexOf(top.getMachine()));
                    // output from this same machine at the designated time
                    Event<X, Y> newOut = new Event<>(t, top.getMachine());
                    pQ.insert(newOut);
                }

                // input second
                // if this model is going to another model in any case, we need to make a new input event
                if(nM.findCouplingByModel(top.getMachine(), 0) != null){

                    // Corresponding input event created here
                    // receive information on the next model in the network model based on the couplings
                    AtomicModel<X, Y> nextMachine = nM.findCouplingByModel(top.getMachine(), 0).getModelTo();

                    // time will be the SAME for the coming input event as the current output event!
                    double r = elapsedTime;
                    // c value will be the value of the next machine
                    Time t = new Time(r, nM.getModelCollection().indexOf(nextMachine));
                    // input from this same machine at the designated time
                    Event<X, Y> newI = new Event<>(t, nextMachine, (X) output, Event.Type.INPUT);

                    // add the new input event for the next model in the sequence
                    pQ.insert(newI);
                }
                // no need to see if we have an output event on this confluent case,
                // because we did that with the confluent's input event
            }
        }
    }
}
