import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {

        // Create Model Instances
        Model press = new Model("press", 1, 0);
        Model drill = new Model("drill", 2, 1);

        // Create our Couple Class
        Couple pressToDrill = new Couple();

        // Create Event Queue
        EventScheduler ePQ = new EventScheduler();

        // Overall elapsed time variable
        double elapsedTime;

        // INPUT VIA FILE
        try {
            File inputFile = new File("/home/lunos/Desktop/Programs/CSC 454/A5Java/src/A5Input.txt");
            Scanner myReader = new Scanner(inputFile);
            while (myReader.hasNextLine()) {
                // parse the data from each line
                String l = myReader.nextLine();
                double occurrenceTime = Double.parseDouble(l.substring(0, l.indexOf(',')));
                int cVal = Integer.parseInt(l.substring(l.indexOf(',') + 1, l.indexOf(',') + 2));
                int input = Integer.parseInt(l.substring(l.length() - 1));

                // form a new time instance for the new event instance that follows
                Time time = new Time(occurrenceTime, cVal);
                // only create this event for the press machine since this is where it will be inserted
                Event e = new Event(time, press, input, Event.Type.INPUT);

                // add this event to the queue
                ePQ.insert(e);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        // next, add output events for all the models at t=MAX.VALUE, to appropriate with the
        // "there is always one output event" rule
        ePQ.insert(new Event(new Time(press.timeAdvance(), 0), press));
        ePQ.insert(new Event(new Time(drill.timeAdvance(), 1), drill));

        // simulation loop that stops once we hit output events of positive infinity or no events at all
        while (ePQ.isNotEmpty() && ePQ.peek().getMoment().getReal() != Double.MAX_VALUE) {

            // check at the top will ensure that we don't have any unaddressed confluent events
            ePQ.checkForConfluence();

            // now that we have checked for confluents, we can poll the event
            Event top = ePQ.poll();

            // update elapsed time to this event
            elapsedTime = top.getMoment().getReal();

            // process the event based on the type
            if (top.getType() == Event.Type.INPUT) {

                // COUPLING
                // snag input variable from the coupling
                int input;
                if (top.getModel() == drill)
                    input = pressToDrill.take();
                else
                    input = top.getNumPartsIn();

                // derive a timePassed variable, so we know how much time has passed since the last input event
                // cases in which there is NO part already in there are covered with the following if conditions
                // - we're at t=-1 (start of simulation)
                // - OR there were no parts in the machine before this input, and therefore no time should be subtracted
                double timePassed;
                if (top.getModel().getTimeOfLastInput() == -1 ||
                        elapsedTime - top.getModel().getTimeOfLastInput() > top.getModel().getProcessingTime())
                    timePassed = 0;
                else
                    timePassed = elapsedTime - top.getModel().getTimeOfLastInput();
                // run this particular machine's delta external
                top.getModel().deltaExt(input, timePassed);
                // update the time of last input, since we have taken in a new input at this time
                top.getModel().setTimeOfLastInput(elapsedTime);

                // EVENT CREATION

                // Corresponding output event created here removed only if we can process a new part
                if (top.getModel().timeAdvance() == top.getModel().getProcessingTime()) {
                    // Corresponding output event created here
                    // remove the old event and add the new
                    ePQ.removeOutputEventByMachine(top.getModel());
                    ePQ.insert(new Event(new Time(elapsedTime + top.getModel().timeAdvance(),
                            top.getModel().getID()), top.getModel()));
                }
            } else if (top.getType() == Event.Type.OUTPUT) {

                // COUPLING

                // Store output variable for later reference
                int output = top.getModel().lambda();
                // If we're looking at the last machine in the NM
                if (top.getModel() == drill)
                    // perform the network model's lambda here, since now we can!
                    System.out.println(elapsedTime + " " + output);
                else
                    // find the output coupling for this machine
                    pressToDrill.give(output);

                // run this particular machine's deltaInt (p is decremented here)
                top.getModel().deltaInt();

                // EVENT CREATION

                // if this model is going to another model in any case, we need to make a new input event
                if (top.getModel() == press)
                    // add the new input event for the next model in the sequence
                    ePQ.insert(new Event(new Time(elapsedTime, top.getModel().getID()),
                            drill, output, Event.Type.INPUT));

                // if the top machine has a bin of processing inputs to work with,
                // then we'll need to schedule a new output event accordingly
                if (top.getModel().getNumParts() > 0) {
                    ePQ.insert(new Event(new Time(elapsedTime + top.getModel().timeAdvance(),
                            top.getModel().getID()), top.getModel()));
                }
            } else {

                // COUPLING

                // OUTPUT FIRST
                int output = top.getModel().lambda();
                // If we're looking at the last machine in the NM
                if (top.getModel() == drill)
                    // perform the network model's lambda here, since now we can!
                    System.out.println(elapsedTime + " " + output);
                else
                    // find the output coupling for this machine
                    pressToDrill.give(output);

                // INPUT SECOND
                int input;
                if (top.getModel() == drill)
                    input = pressToDrill.take();
                else
                    input = top.getNumPartsIn();


                // run this particular machine's delta con
                top.getModel().deltaCon(input);
                // update the time of last input, since we have taken in a new input at this time
                top.getModel().setTimeOfLastInput(elapsedTime);

                // EVENT CREATION

                // OUTPUT FIRST
                ePQ.insert(new Event(new Time(elapsedTime + top.getModel().timeAdvance(),
                        top.getModel().getID()), top.getModel()));

                // INPUT SECOND
                if (top.getModel() == press) {
                    // add the new input event for the next model in the sequence
                    ePQ.insert(new Event(new Time(elapsedTime, top.getModel().getID()),
                            drill, output, Event.Type.INPUT));
                }
            }
        }
    }
}
