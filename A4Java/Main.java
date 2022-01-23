import java.util.Scanner;

public class Main {
    public static void main(String[] args) {

        // create the vending machine
        VendingMachineModel frasier = new VendingMachineModel();

        // Scanner object for input
        Scanner in = new Scanner(System.in);

        // INPUT HANDLING BEGINS HERE ----------------------------------------------------------------------------------

        while (in.hasNextLine()) {
            // parse the data from each line
            String l = in.nextLine();
            double elapsedTime = Double.parseDouble(l.substring(0, l.indexOf(',')));
            int cVal = Integer.parseInt(l.substring(l.indexOf(',') + 1, l.indexOf(',') + 2));
            char coin = l.charAt(l.length() - 1);

            // add the events into an ArrayList to work like a queue, where the newest event is placed at the end of the collection.
            // To read this like a queue, we will only read from the first index in the main program.
            VendingMachineEvent vme = new VendingMachineEvent(elapsedTime, cVal, coin);
            frasier.addEvent(frasier.getVMEList().size(), vme);
        }

        // SIMULATION BEGINS HERE --------------------------------------------------------------------------------------

        // boolean tracked to end the program when necessary
        boolean end = false;
        while (!end) {

            // first, look to see if this event is the only existing event
            if (frasier.getVMEList().size() == 1) {
                // if this is an output event
                if (frasier.getEvent(0).isOutput()) {
                    // call lambda as well as delta internal, since we are outputting, as well as adjusting the state
                    // for said output
                    System.out.println();
                    System.out.println(frasier.lambda());
                    frasier.deltaInternal();
                    // we can end the program here since we know there won't be another event after this one
                    end = true;
                } else {
                    // otherwise, treat this like an input event and insert a new output event in the next slot
                    // we'll handle this newly created event on the next iteration
                    frasier.deltaExternal(frasier.getEvent(0).getTime(), frasier.getEvent(0).getCoin());
                    // call timeAdvancement since we have a new input to account for
                    frasier.timeAdvancement();
                    VendingMachineEvent vme = new VendingMachineEvent(frasier.getEvent(0).getTime() + 2, frasier.getEvent(0).getcValue());
                    frasier.addEvent(1, vme);
                }
            }
            // otherwise, we won't have to consider ending the program just yet
            else {
                // compare this element to the next one, since our actions depend on the current event and the next one
                // with respect to it
                // if the first event is scheduled to occur before the second
                if (frasier.getEvent(0).getTime() < frasier.getEvent(1).getTime()) {
                    if (frasier.getEvent(0).isOutput()) {
                        // call a delta internal since we can output at this point
                        // we will call lambda here as well
                        System.out.println();
                        System.out.println(frasier.lambda());
                        frasier.deltaInternal();
                    } else {
                        // otherwise, treat this like an input event and insert a new output event in the next slot
                        // we'll handle this newly created event on the next iteration
                        frasier.deltaExternal(frasier.getEvent(0).getTime(), frasier.getEvent(0).getCoin());
                        frasier.timeAdvancement();
                        VendingMachineEvent vme = new VendingMachineEvent((frasier.getEvent(0).getTime() + frasier.getTime()), frasier.getEvent(0).getcValue());
                        frasier.addEvent(1, vme);
                    }
                }
                // if the second event is scheduled to occur before the first
                else if (frasier.getEvent(0).getTime() > frasier.getEvent(1).getTime()) {
                    // remove this first event since we know that it can only ever be an output event placed before a preceding input event
                    // ex. "2.4,0 o" has no use remaining in the Queue if we must instead call "1.2,0 q"
                    frasier.removeEvent(0);
                    // treat this like an input event and insert a new output event in the next slot
                    // we'll handle this newly created event on the next iteration
                    frasier.deltaExternal(frasier.getEvent(0).getTime(), frasier.getEvent(0).getCoin());
                    frasier.timeAdvancement();
                    VendingMachineEvent vme = new VendingMachineEvent((frasier.getEvent(0).getTime() + frasier.getTime()), frasier.getEvent(0).getcValue());
                    frasier.addEvent(1, vme);
                }
                // if both events occur at the exact same time
                else {
                    // if the second line is the input event
                    if (frasier.getEvent(0).isOutput()) {
                        // call lambda and deltaConfluent since we are inputting and outputting at the same time
                        System.out.println();
                        System.out.println(frasier.lambda());
                        frasier.deltaConfluent(frasier.getEvent(1).getCoin());
                        frasier.timeAdvancement();
                        VendingMachineEvent vme = new VendingMachineEvent((frasier.getEvent(0).getTime() + frasier.getTime()), frasier.getEvent(0).getcValue());
                        frasier.addEvent(2, vme);
                    }
                    // if the first line is the input event
                    else {
                        // call lambda and deltaConfluent since we are inputting and outputting at the same time
                        System.out.println();
                        System.out.println(frasier.lambda());
                        frasier.deltaConfluent(frasier.getEvent(0).getCoin());
                        frasier.timeAdvancement();
                        VendingMachineEvent vme = new VendingMachineEvent((frasier.getEvent(0).getTime() + frasier.getTime()), frasier.getEvent(0).getcValue());
                        frasier.addEvent(2, vme);
                    }
                    // we have processed one event and will soon process the other at the end of this loop.
                    // This is to ensure that deltaConfluent is read correctly without screwing up our Event list indexing
                    // since deltaConfluent technically requires the presence of two events to be called.
                    frasier.removeEvent(0);
                }
            }
            // remove this element from the event list after we have processed it accordingly
            frasier.removeEvent(0);
        }
    }
}
