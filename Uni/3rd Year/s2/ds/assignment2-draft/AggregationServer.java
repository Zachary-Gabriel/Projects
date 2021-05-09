import java.net.*; 
import java.io.*;
import java.util.Vector;

public class AggregationServer
{
    // private variables of the database
    private Vector database = new Vector ();
    private boolean verbose = true;
    
    // converts a String number input to an int i.e. "42" => 42
    private static int string_to_int (String input)
    {
        int char_to_int;
        int ret = 0;
        for (int i = 0 ; i < input.length(); ++i)
        {
            char_to_int = input.charAt(i);
            // ascii of '0' is 48
            ret = 10*ret + char_to_int - 48;            
        }
        return ret;
    }
    
    // identifies if request was PUT or GET
    private String get_req_type (String input)
    {
        int i = 0;
        String ret = "";
        while (i < input.length())
        {
            if (input.charAt(i) == ' ')
            {
                break;
            }
            ret += input.charAt(i);
            ++i;
        }
        return ret;
    }
    
    // Aggregation Server constructor
    public AggregationServer (int port)
    {
        // for establishing a connection and sending data (2-ways)
        ServerSocket listen_socket;
        Socket connection_socket;
        DataOutputStream output;
        DataInputStream input;

        // for storing data that has been recieved/ will be sent
        String message;
        String req_type = "";

        // for getting the message data from a PUT request
        boolean is_body;
        String body;

        try 
        {
            // checking port
            if (port <= 0)
            {
                System.out.println ("bad server port number");
                System.exit (1);
            }
            
            // setting up server
            listen_socket = new ServerSocket (port);
            System.out.println ("listening on port " + port + " ...");
            
            // spinning if valid listen_port
            while (true)
            {
                try 
                {
                    // connecting to client using connection_socket
                    connection_socket = listen_socket.accept();
                    
                    // reading the request using InputStream
                    input = new DataInputStream(connection_socket.getInputStream());
                    message = input.readUTF();
                    
                    // check request type
                    req_type = get_req_type (message);
                    
                    // do server operations based on the http req type
                    // send the XML database                    
                    if (req_type.equals("GET"))
                    {
                        //sending the database
                        output  = new DataOutputStream(connection_socket.getOutputStream()); 
                        output.flush ();
                        
                        message = "";
                        for (int i = 0; i < database.size(); ++i)
                        {
                            message += database.get(i);
                        }
                        output.writeUTF(message);
                        output.close ();
                    }
                    // put the incoming XML into the database
                    else if (req_type.equals("PUT"))
                    {
                        is_body = false;
                        body = "";
                        // separating the body from the request
                        for (int i = 1; i < message.length(); ++i)
                        {
                            if (is_body)
                            {
                                body += message.charAt(i);
                            }
                            if (message.charAt(i-1) == '\n' && message.charAt(i) == '\r')
                            {
                                is_body = true;
                            }
                        }
                        if (verbose) // for debugging
                        {
                            // printing the input that has just been recieved
                            System.out.print (body);
                        }
                        database.add (body);
                    }
                    else if (req_type != "")
                    {
                        System.out.println ("http request not implemented or not valid");
                    }
                    
                    // terminate connection
                    input.close ();
                    connection_socket.close ();
                } 
                catch (Exception e) 
                {
                    System.out.println (e);
                    System.exit (1);
                }
            }
        }
        // if bad port
        catch (Exception e) 
        {
            System.out.println (e);
            System.exit (1);
        }
    }
    
    // initiates an aggregate server
    public static void main(String args[]) 
    {
        // take input port
        if (1 <= args.length)
        {
            int port = string_to_int(args[0]);
            AggregationServer server = new AggregationServer(port);
        }
        else
        {
            // take default port 8080
            AggregationServer server = new AggregationServer(8080); 
        }
    } 
}