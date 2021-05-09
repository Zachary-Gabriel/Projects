import java.rmi.registry.Registry; 
import java.rmi.registry.LocateRegistry; 
import java.rmi.RemoteException; 
import java.rmi.server.UnicastRemoteObject;
import java.io.Serializable;


// what the client connects to in order to use the rmi
public class SorterServer extends SorterImplementation
{ 
	public SorterServer () {}
	public static boolean verbose = false;
	
	// establishes connection to rmi user
	public static void main(String args[]) 
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

		// listening for rmi users
		try 
		{ 
			// creating stub and binding it to the RMI registry
			SorterImplementation sorter = new SorterImplementation (); 
			Sorter stub = (Sorter) UnicastRemoteObject.exportObject (sorter, 0);  
			Registry registry = LocateRegistry.getRegistry ("localhost", 2099); 
			registry.bind ("Sorter", stub); 
			
			System.out.println ("Server ready"); 
		}
		catch (Exception not_an_issue) 
		{ 
			try
			{
				// creating stub and rebinding it to the RMI registry (if server was run previously)
				SorterImplementation sorter = new SorterImplementation (); 
				Sorter stub = (Sorter) UnicastRemoteObject.exportObject (sorter, 0);  
				Registry registry = LocateRegistry.getRegistry ("localhost", 2099); 
				registry.rebind ("Sorter", stub); 
				
				System.out.println ("Server ready");
			}
			// if something goes wrong
			catch (Exception e)
			{
				System.err.println ("Server exception: " + e.toString ()); 
				e.printStackTrace ();
			}
		}

		return ;
	} 
} 