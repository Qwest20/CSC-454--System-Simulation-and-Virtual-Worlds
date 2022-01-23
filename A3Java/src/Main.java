import java.util.Scanner;

public class Main {
    public static void main(String[] args) {

        // Scanner object for input
        Scanner in = new Scanner(System.in);

        // Model instances
        NetworkModel n = new NetworkModel();

        // input for this tick
        System.out.println("Welcome to the XOR State Machine!");
        System.out.println("NOTE: enter -1 for EITHER NUMBER INPUT and the program will exit");

        // make a simulation exception. Your whole simulation should be in a try catch block

        int networkTick = 0;
        while (true) {

            System.out.println();
            System.out.println("Please enter your first value");
            int a = in.nextInt();
            System.out.println("Please enter your second value");
            int b = in.nextInt();

            if (a == -1 || b == -1)
                System.exit(0);

            // run this three times to complete an entire tick
            System.out.println("-------------------Network Tick = "+networkTick+"-------------------");
            for (int i = 0; i < 3; i++) {

                System.out.println("-------------------Atomic Tick = "+(i+1)+"-------------------");

                // inner lambdas if desired
                System.out.println("XOR1.lambda() = ");
                System.out.println(n.getX1().lambda());
                System.out.println("XOR2.lambda() = ");
                System.out.println(n.getX2().lambda());
                System.out.println("MEM.lambda() = ");
                System.out.println(n.getM().lambda());

                // coupling
                n.coupling();

                // delta
                n.delta(a, b);
            }

            // lambda for the whole network outputs only on every network tick
            System.out.println(n.lambda());

            networkTick++;
        }
    }
}
