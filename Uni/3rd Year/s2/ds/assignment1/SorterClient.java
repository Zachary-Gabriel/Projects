import java.rmi.registry.Registry; 
import java.rmi.registry.LocateRegistry;
import java.io.Serializable;

// user trying to use rmi
public class SorterClient 
{  
	// debugging switch
	private static boolean verbose = false;
	
	private SorterClient() {}  

	private static int parse_int (String[] input, int i)
	{
		// checking that there is another element in the input array
		if ( !(i < input.length))
		{
			throw new RuntimeException ("missing value");
		}

		// checking the input - making sure it's an integer
		try 
		{
			int val = Integer.parseInt (input[i]);
			return val;
		}

		// if invalid int, throw exception
		catch (Exception e)
		{
			System.err.println("Client exception - bad input in parse_int: " + e.toString()); 
			e.printStackTrace();
			java.lang.System.exit (-1);

			return -1; // here's me hoping this is not needed :)
		}

	}

	// validating the input for pushOperator
	private static String parse_operator (String[] input, int i)
	{
		// checking that there is another element in the input array
		if ( !(i < input.length))
		{
			throw new RuntimeException ("missing command");
		}

		// checking that the input matches one of the four operations
		switch (input[i])
		{
			case "ascending":
			// for debugging
			if (verbose)
				System.out.println ("switch triggered: ascending");
			break;

			case "descending":
			// for debugging
			if (verbose)
				System.out.println ("switch triggered: descending");
			break;

			case "max":
			// for debugging
			if (verbose)
				System.out.println ("switch triggered: max");
			break;

			case "min":
			// for debugging
			if (verbose)
				System.out.println ("switch triggered: min");
			break;
			
			// if not a valid operator:
			default:
			throw new RuntimeException ("invalid command");
		}
		
		return input[i];
	}

	// converting the inputs into method calls
	private static void parse_input (String[] args, Sorter stub)
	{
		// initialising variables
		int val = -1;
		String op = "";
		int i = 0;

		// looping through all the inputs of the input argument
		for (i = 0; i < args.length; ++i)
		{

			switch (args[i])
			{
				// Parsing input for pushValue method
				/* Dev note: returns void */
				case "pushValue":
				case "pushVal":
				case "push_value": 

				// making sure valid int input
				++ i;
				val = parse_int (args, i);

				// for debugging
				if (verbose)
					System.out.println ("pushValue " + val);

				// executing method using stub
				try
				{
					stub.push_value (val);
				}
				catch (Exception e)
				{
					System.out.println ("we have a problem");
				}
				break;

				// parsing input for pushOperator method
				/* Dev note: returns void */
				case "pushOperator" :
				case "pushOp" :
				case "push_operator" :

				// sending array to handler, to see if the next value is a valid operator
				++ i;
				op = parse_operator (args, i);

				// for debugging
				if (verbose)
					System.out.println ("pushOperator " + op);

				// executing method using stub
				try
				{
					stub.push_operator (op);
				}
				catch (Exception e)
				{
					System.out.println ("we have a problem");
				}
				break;

				// parsing input for pop method
				/* Dev note: returns int */
				case "pop" :

				// for debugging
				if (verbose)
					System.out.println ("pop");

				// executing method using stub
				try
				{
					stub.pop ();
				}
				catch (Exception e)
				{
					System.out.println ("we have a problem");
				}
				break;

				// parsing input for isEmpty method
				/* Dev note: returns int */ 
				case "isEmpty" : 
				case "is_empty" : 

				// for debugging
				if (verbose)
					System.out.println ("isEmpty");

				// executing method using stub
				try
				{
					stub.is_empty ();
				}
				catch (Exception e)
				{
					System.out.println ("we have a problem");
				}
				break;

				// parsing input for delayPop method
				/* Dev note: returns int */
				case "delayPop":
				case "delay_pop":

				// sending array to handler, to see if the next value is a valid operator
				++ i;
				val = parse_int (args, i);

				// for debugging
				if (verbose)
					System.out.println ("delayPop " + val + " milliseconds");

				// executing method using stub
				try
				{
					stub.delay_pop (val);
				}
				catch (Exception e)
				{
					System.out.println ("we have a problem");
				}
				break;

				// if the input is unknown
				default:

				// maybe the user wants to see this idk, but tis great for debugging
				if (verbose)
					System.out.println ("Unknown input: " + args[i]);
			}
		}
	}
	
	// converts input into methods, then uses a stub to run said methods
	public static void main (String[] args) 
	{
		// enables verbose mode
		if (args.length != 0)
		{
			if (args[0].equals ("-v"))
			{
				verbose = true;

				// removing "-v" from array
				String[] temp_args = new String[args.length -1];
				for (int i = 0; i < temp_args.length; ++i)
				{
					temp_args[i] = args[i+1];
				}
				args = temp_args;
			}
		}

		// tries to establish connection with rmi registry and server
		try 
		{  
         	// finding the registry i.e. rmi://localhost:2099/Sorter
			Registry registry = LocateRegistry.getRegistry ("localhost", 2099); 
			Sorter stub = (Sorter) registry.lookup("Sorter"); 

         	// converting input into method calls, and using stub to execute
			parse_input (args, stub);

			// feedback to the client
			System.out.println ("Done");

		}

		// if something goes wrong
		catch (Exception e) 
		{
			System.err.println("Client exception: " + e.toString()); 
			e.printStackTrace(); 
		}
		return ;
	} 
}
