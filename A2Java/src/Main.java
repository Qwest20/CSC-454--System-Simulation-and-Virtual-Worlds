import java.util.Scanner;

public class Main {
    public static void main(String[] args) {

        // A Scanner for user input
        Scanner input = new Scanner(System.in);

        // create the vending machine
        VendingMachineModel diane = new VendingMachineModel();

        // beginning output
        System.out.println();
        System.out.println("I am the coffee Vending Machine! Please enter your input with respect to the key below:");
        System.out.println("--------------------------------------------------------------------------------------------");
        System.out.println("q = 1 quarter");
        System.out.println("d = 1 dime");
        System.out.println("n = 1 nickel");
        System.out.println("w = wait");
        System.out.println("c = cancel (dispense change)");
        System.out.println("--------------------------------------------------------------------------------------------");
        System.out.println("Note: you may enter as many of these LOWERCASE ONLY characters as you'd like in each singular input line.");
        System.out.println("Additionally, you can exit the program by inputting a \"c\" when the \"value\" variable in the machine is equal to 0");
        System.out.println();

        // Run the simulation
        while(true){

            // read input here
            System.out.println("Please enter your input:");
            String in = input.nextLine();

            // check the previously existing state and print accordingly
            // S -> Y
            diane.lambda();
            // delta returns an exit state to see if we need to close the program
            // X,S -> S
            diane.delta(in);
            System.out.println();
        }
    }
}
