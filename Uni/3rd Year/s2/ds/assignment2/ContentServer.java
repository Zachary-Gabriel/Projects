import java.io.*;
import java.net.*; 
import java.util.Timer;

/* needs physical clock */

// the content server
public class ContentServer 
{
    private static boolean verbose = true;
    private static int time = -1;
    private int process = -1;
    
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
    
    private void get_hb_response (Socket socket, Lamport clock)
    {
        try 
        {
         
            DataInputStream input = new DataInputStream (socket.getInputStream ()); 
            String message = input.readUTF();   
            time = clock.next_clock (get_lamport_clock (message), time);
        } 
        
        catch (Exception e) 
        {
            System.out.println (e);
            System.exit(-1);
        }
        return ;
    }
    
    private void send_heartbeat (Socket socket)
    {
        try 
        {
            DataOutputStream output = new DataOutputStream (socket.getOutputStream ()); 
            String message = "";
            
            message += "HEARTBEAT HTTP/1.1\n";
            message += "User-Agent: ATOMClient/1/0\n";
            message += "lamport-clock: " + time + "\n";
            message += "server-id: " + process + (char) 13 + (char) 10 + "\n";
            
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
    
    public ContentServer (String[] args)
    {
        // initialising file parsing variables
        FileReader file_in = null; // file pointer
        int ascii = -1; // int value of the input character
        
        // initialising xml parsing variables
        XMLParser XML; // used to convert the xml from 'input'
        String xml = ""; // the converted xml from 'input'
        
        // initialising socket programming variables
        Socket socket; // socket used to connect to aggregate server
        DataOutputStream output; // output stream - the stream that is used to send data to the server
        DataInputStream input; // output stream - the stream that is used to send data to the server
        String message = "";
        
        // for periodic heartbeats
        long start_beat = System.currentTimeMillis();
        long last_beat = 0L;
        
        
        Lamport clock = new Lamport ();
        time = clock.get_clock ();
        
        try 
        {
            // reading input file
            file_in = new FileReader(args[2]);
            
            // starting file read
            ascii = file_in.read();
            
            // if the input character is valid, append to string
            while (ascii != -1) 
            {
                xml += (char) ascii;
                ascii = file_in.read();
            }
            
            // finished reading file
            file_in.close();
            
            // push the input file through the XML parser
            XML = new XMLParser (xml, verbose);
            // making HTTP PUT header
            xml = XMLParser.HTTP_PUT_header (XML.get_XML ().length (), time);
            // appending the parsed xml
            xml += XML.get_XML ();
            
            // sending the xml to aggregation server
            // establishing connection 
            int port = string_to_int (args[1]);
            socket = new Socket (args[0], port);
            output = new DataOutputStream (socket.getOutputStream ()); 
            System.out.println ("Establishing connection");
            System.out.println ("Lamport time " + time);
            
            // sending data
            output.flush ();
            output.writeUTF(xml);
            
            start_beat = System.currentTimeMillis();            
            while (message == "")
            {
                last_beat = System.currentTimeMillis() - start_beat;
                if (last_beat > 10 * 1000)
                {
                    System.out.println ("lost connection to aggregate server");
                    System.exit(-1);
                }
                input = new DataInputStream (socket.getInputStream ()); 
                message = input.readUTF();   
            }
            
            if (message.equals("invalid XML\n"))
            {
                System.out.println ("invalid XML");
                System.exit(-1);
            }
            
            int incoming_clock = get_lamport_clock(message);
            
            process = incoming_clock;
            time = clock.next_clock (incoming_clock, time);
            
            // terminating connection
            System.out.println ("Message sent. Terminating connection.");
            output.close ();
            socket.close ();

            // periodically sending heartbeat messages to the aggregate server
            while (true)
            {
                last_beat = System.currentTimeMillis() - start_beat;
                
                // increment the lamport clock and send a heartbeat
                if (6*1000 < last_beat)
                {
                    socket = new Socket (args[0], port);
                    start_beat = System.currentTimeMillis();
                    // output = new DataOutputStream (socket.getOutputStream ()); 
                    send_heartbeat (socket);
                    System.out.println ("Heartbeat sent. lamport-time: " + time);

                    // input = new DataInputStream (socket.getInputStream ()); 
                    get_hb_response (socket, clock);
                    System.out.println ("Heartbeat response recieved. lamport-time: " + time);
                }
            }
        }
        
        // if can't connect to server or read file
        catch (Exception e)
        {
            System.out.println (e);
        }
    }
    
    public static void main(String[] args) 
    {
        // checking if valid number of args
        if (args.length != 3 && args.length != 4)
        {
            System.out.println ("Content server requires: 'hostname' 'port number' 'input xml name' '-v' (optional)");
            System.exit (1);
        }
        
        verbose = false;
        if (args.length == 4)
        {
            if (args[3] == "-v") verbose = true;
        }
        
        ContentServer server = new ContentServer (args);
        
        return ;
    }
}
