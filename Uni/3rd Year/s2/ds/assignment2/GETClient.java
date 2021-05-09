import java.net.*; 
import java.io.*; 

// generic user that access the server's database
public class GETClient 
{
    private static boolean verbose = false;
    private static int time = -1;
    
    // converts the string to an int
    private static int string_to_int (String input)
    {
        int res = 0;
        for (int i = 0; i < input.length (); ++i)
        {
            if ( 48 <= (int) input.charAt(i) && (int) input.charAt(i) <= 57)
            res = 10*res + (int) input.charAt(i) - 48;
            else
            {
                System.out.println ("error: bad port number (arg[1]");
                System.exit(1);
            }
        }
        return res;
    }
    
    public GETClient (String[] args)
    {
        // want to use the arg as a name to a file in which the xhtml document will be printed to
        // i.e. OutputBuffer webpage = args[0] + ".xhtml"
        // ideally, the ip address and port are also used as arguments
        String host = "localhost";
        String message = "";
        int port = 8080;
        verbose = false;
        
        if (args.length == 0)
        {
            // nothing
        }
        else if (args.length == 1)
        {
            if (args[0].equals ("-v"))
            {
                verbose = true;
            }
        }
        else if (args.length == 2)
        {
            host = args[0];
            port = string_to_int (args[1]);
        }
        else if (args.length == 3)
        {
            host = args[0];
            port = string_to_int (args[1]);
            if (args[2].equals ("-v"))
            {
                verbose = true;
            }
        }
        
        try 
        {
            // Setting up the lamport clock
            Lamport clock = new Lamport ();
            time = clock.get_clock ();
            
            // establishing connection to server with 2-way traffic
            Socket socket = new Socket (host, port);
            DataInputStream input  = new DataInputStream(socket.getInputStream()); 
            DataOutputStream output  = new DataOutputStream(socket.getOutputStream()); 
            System.out.println ("Connection Established");
            System.out.println ("Lamport clock: " + time);
            
            // clearing the output pipeline before sending data (a simple GET request without all the bells and whistles)
            output.flush ();
            message = "GET\n";
            message += "lamport-clock: " + time + (char) 13 + (char) 10 + "\n";
            output.writeUTF(message);
            // printing the response to the terminal - plan to write this to a .xhtml doc
            if (verbose) 
            System.out.println (input.readUTF());
            
            System.out.println ("Connection Terminated");
            output.close ();
            input.close ();
        }
        
        // if can't connect to server
        catch (Exception e) 
        {
            System.out.println (e);
        }
    }
    
    public static void main(String[] args) 
    {
        GETClient client = new GETClient (args);
    }
}
