import java.net.*; 
import java.io.*;

import java.util.Vector;

import java.util.Timer;

// import java.lang.Object;
// import java.lang.Number;
import java.lang.*;


public class AggregationServer
{
    // private variables of the database
    private Vector <String> database = new Vector <String> ();
    private Vector <Integer> metadata_id = new Vector <Integer> ();
    private Vector <Long> metadata_time = new Vector <Long> ();
    
    private static boolean verbose = true;
    private int time = -1;
    
    // converts a String number input to an int i.e. "42" => 42
    private static int string_to_int (String input)
    {
        int char_to_int;
        int ret = 0;
        for (int i = 0 ; i < input.length(); ++i)
        {
            char_to_int = input.charAt(i);
            // ascii of '0' is 48
            if (48 <= char_to_int && char_to_int <= 57)
            {
                ret = 10*ret + char_to_int - 48;
            }
            else
            {
                System.out.println ("invalid number");
                System.exit (1);
            }
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
            if (input. charAt (i) == ' ' || input. charAt (i) == '\n')
            {
                break;
            }
            ret += input. charAt (i);
            ++i;
        }
        return ret;
    }
    
    private void send_heartbeat (DataOutputStream output )
    {
        try 
        {
            String message = "";
            time++;
            message += "HEARTBEAT_RES 200 HTTP/1.1\n";
            message += "User-Agent: ATOMClient/1/0\n";
            message += "lamport-clock: " + time + (char) 13 + (char) 10 + "\n";
            
            output.flush();
            output.writeUTF(message);
        } 
        
        catch (Exception e) 
        {
            System.out.println (e);
            System.exit(-1);
        }
        return;
        
    }

    // gets lamport clock from message
    private int get_lamport_clock (String message)
    {
        int incoming_clock = 0;
        int index = message.lastIndexOf ("lamport-clock: ");
        index += 15;
        
        while (message.charAt(index) != (char) 13 && message.charAt(index) != '\n')
        {
            incoming_clock = 10 * incoming_clock + (int) message.charAt(index) - 48;
            ++ index;
        }

        return incoming_clock;
    }

    // gets process id from content server message
    private int get_process_id (String message)
    {
        int proc_id = 0;
        int index = message.lastIndexOf ("server-id: ");
        index += 11;
        
        while (message.charAt(index) != (char) 13 && message.charAt(index) != '\n')
        {
            proc_id = 10 * proc_id + (int) message.charAt(index) - 48;
            ++ index;
        }

        return proc_id;
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
        
        // for clocks
        Lamport clock = new Lamport ();
        time = clock.get_clock ();
        
        // time management
        long current_time = System.currentTimeMillis();
        
        try 
        {
            // checking port
            if (port < 0)
            {
                System.out.println ("bad server port number");
                System.exit (1);
            }
            else if (port == 0)
            {
                port = 8080;
            }
            
            // setting up server
            listen_socket = new ServerSocket (port);
            System.out.println ("listening on port " + port + "...");
            
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
                    
                    // get lamport clock
                    int incoming_clock = get_lamport_clock (message);
                    time = clock.next_clock (incoming_clock, time);
                    
                    // do server operations based on the http req type
                    // send the XML database                    
                    if (req_type.equals("GET"))
                    {
                        System.out.println ("GET request recieved. Sending table.");
                        
                        //sending the database
                        output  = new DataOutputStream (connection_socket. getOutputStream ()); 
                        output.flush ();
                        
                        message += "<?xml version='1.0' encoding='iso-8859-1' ?>\n<feed xml:lang='en-US' xmlns='http://www.w3.org/2005/Atom'>\n";
                        
                        current_time = System.currentTimeMillis();
                        int counter = 0;
                        for (int i = 0; i < database.size(); ++i)
                        {
                            if ( metadata_time.get(i).longValue() > current_time - 12 * 1000 && counter < 20)
                            {
                                message += database.get(i);
                                ++ counter;
                            }
                        }
                        message += "</feed>\n";
                        
                        output.writeUTF (message);
                        output.close ();
                    }
                    
                    // put the incoming XML into the database
                    else if (req_type.equals("PUT"))
                    {
                        System.out.println ("PUT request recieved. Updating table.");
                        
                        is_body = false;
                        body = "";
                        
                        // separating the body from the request
                        for (int i = 1; i < message.length(); ++i)
                        {
                            if (is_body)
                            {
                                body += message. charAt (i);
                            }
                            if (message. charAt(i-1) == '\n' && message.charAt(i) == '\r')
                            {
                                is_body = true;
                            }
                        }
                        
                        if (verbose) // for debugging
                        {
                            // printing the input that has just been recieved
                            System.out.print (body);
                        }
                        
                        if (XMLParser.is_valid_XML (body, verbose))
                        {
                            // setting up the database metadata
                            Integer id = time;
                            current_time = System.currentTimeMillis();
                            
                            // modifying the input XML
                            body = body.replace ("<?xml version='1.0' encoding='iso-8859-1' ?>\n<feed xml:lang='en-US' xmlns='http://www.w3.org/2005/Atom'>\n", "");
                            body = body.replace ("</feed>", "");
                            
                            // storing the data
                            metadata_id.add (id);
                            metadata_time.add (current_time);
                            database.add (body);
                            
                            output = new DataOutputStream (connection_socket. getOutputStream()); 
                            output.flush ();
                            output.writeUTF("lamport-clock: " + time + "\n");
                        }
                        
                        else
                        {
                            System.out.println ("invalid XML");
                            output = new DataOutputStream (connection_socket. getOutputStream()); 
                            output.flush ();
                            output.writeUTF("invalid XML\n");
                            output.close ();
                        }
                    }
                    
                    else if (req_type.equals("HEARTBEAT"))
                    {
                        current_time = System.currentTimeMillis();
                        int process = get_process_id (message);
                        
                        for (int i = 0; i < database.size(); ++i)
                        {
                            if (metadata_id.get(i) == process)
                            {
                                // saving variables and resetting the time
                                int temp_id = metadata_id.get(i);
                                Long temp_time = current_time;
                                String temp_xml = database.get(i);
                                
                                // popping from vector
                                metadata_id.remove(i);
                                metadata_time.remove(i);
                                database.remove(i);
                                
                                // pushing to front
                                metadata_id.add(0, temp_id);
                                metadata_time.add(0, temp_time);
                                database.add(0, temp_xml);
                            }
                        }
                        output = new DataOutputStream (connection_socket. getOutputStream()); 
                        send_heartbeat (output);
                        System.out.println ("hearbeat response sent");
                        output.close();
                    }
                    
                    else if (req_type != "")
                    {
                        System.out.println ("http request not implemented or not valid");
                    }
                    
                    // terminate connection
                    input.close ();
                    connection_socket.close ();
                }
                
                // if somethink goes wrong
                catch (Exception e) 
                {
                    e.printStackTrace ();
                    System.out.println (e);
                    System.exit (1);
                }
            }
        }
        
        // if bad port
        catch (Exception e) 
        {
            e.printStackTrace ();
            System.out.println (e);
            System.exit (1);
        }
    }
    
    // initiates an aggregate server
    public static void main(String args[]) 
    {
        verbose = false;
        int port = 0;
        int index = -1;
        
        for (int i = 0; i < args.length; ++i)
        {
            if (args[i].equals ("-v")) 
            {
                verbose = true;
                index = 1-i;
            }
        }
        
        if (index != -1 && args.length == 1)
        {
            port = 8080;
        }
        else if (index != -1 && args.length == 2)
        {
            port = string_to_int (args[index]);
        }
        else if (args.length == 1)
        {
            port = string_to_int (args[0]);
        }
        
        AggregationServer server = new AggregationServer(port);
        return ;
    } 
}