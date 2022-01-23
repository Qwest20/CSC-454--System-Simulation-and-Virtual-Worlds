import Couplings.InputCoupling;
import Couplings.ModelsCoupling;
import Couplings.OutputCoupling;
import Models.NetworkModel;
import Models.PressDrillNetworkModel.DrillModel;
import Models.PressDrillNetworkModel.PressModel;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] a) {

        //setup for PressDrill Network Model Case
        Framework<Integer, Integer> Cliff = new Framework<>();
        // index parameter included to differentiate, should we have multiple instances of the same model type
        PressModel p = new PressModel(1);
        DrillModel d = new DrillModel(1);
        ModelsCoupling<Integer, Integer> c = new ModelsCoupling<>(p, d);
        NetworkModel<Integer, Integer> n = new NetworkModel<>(new InputCoupling<>(p), new OutputCoupling<>(d));
        n.addModel(p);
        n.addModel(d);
        n.addCoupling(c);

        // input loop to derive an arraylist of events
        ArrayList<Event<Integer, Integer>> inputEvents = new ArrayList<>();

        //      INPUT VIA FILE
        try {
            File inputFile = new File("/home/lunos/Desktop/Programs/CSC 454/A6Java/src/A6Input.txt");
            Scanner myReader = new Scanner(inputFile);
            while (myReader.hasNextLine()) {
                // parse the data from each line
                String l = myReader.nextLine();
                double occurrenceTime = Double.parseDouble(l.substring(0, l.indexOf(',')));
                int cVal = Integer.parseInt(l.substring(l.indexOf(',') + 1, l.indexOf(',') + 2));
                Integer input = Integer.parseInt(l.substring(l.length() - 1));

                // form a new time instance for the new event instance that follows
                Time time = new Time(occurrenceTime, cVal);
                // only create this event for the press machine since this is where it will be inserted
                Event<Integer, Integer> e = new Event<>(time, n.getModelAt(0), input, Event.Type.INPUT);

                // add this event to the queue
                inputEvents.add(e);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        // INPUT VIA SCANNER
        // Scanner object for input
//        Scanner in = new Scanner(System.in);
//        while (in.hasNextLine()) {
//
//            // parse the data from each line
//                String l = myReader.nextLine();
//                double occurrenceTime = Double.parseDouble(l.substring(0, l.indexOf(',')));
//                int cVal = Integer.parseInt(l.substring(l.indexOf(',') + 1, l.indexOf(',') + 2));
//                Integer input = Integer.parseInt(l.substring(l.length() - 1));
//
//                // form a new time instance for the new event instance that follows
//                Time time = new Time(occurrenceTime, cVal);
//                // only create this event for the press machine since this is where it will be inserted
//                Event<Integer, Integer> e = new Event<>(time, n.getModelAt(0), input, Event.Type.INPUT);
//
//                // add this event to the queue
//                inputEvents.add(e);
//        }

        // run the simulation with this list of composed events
        Cliff.simulate(n, inputEvents);
    }
}
