import java.net.*; // sockets
import java.io.*; // sending data through sockets
import java.util.*; // vectors

// For the broadcast
public class Broadcast implements Runnable
{   
    // private variables
    private Vector <Integer> promises = new Vector <Integer> ();
    private boolean consensus = false;
    private int id;
    private int num_voters;
    
    // function arguments required for the broadcast
    private int port;
    private int seconds;
    private long message;

    // Constructor of the Broadcast class. 
    // Notably function arguments will be input here so we can use Run ()
    public Broadcast (int id, int port, int seconds, long message, int num_voters, boolean consensus)
    {
        this.id = id;
        this.consensus = false;
        this.num_voters = num_voters;
        this.port = port;
        this.seconds = seconds;
        this.message = message;
    
        return ;
    }
    
    // if new member has greater nonce, then terminate current broadcast and let new member broadcast
    private boolean broadcast_greater_nonce (int port, long new_nonce)
    {
        // for connecting
        Socket socket ; // socket used for data transfer
        DataOutputStream output ; // for sending data
        DataInputStream input ; // for receiving data
        
        long current_nonce = 0;
        try
        {
            socket = new Socket ("localhost", port);
            
            // getting input
            input = new DataInputStream (socket.getInputStream ());
            String message = input.readUTF(); 
            
            // getting output
            output = new DataOutputStream (socket.getOutputStream ()); 
            output.flush ();
            
            // if broadcasting an update, can be modified
            if (message.charAt(0) == 'U')
            {
                message = message.replace ("UPDATE ", "");
            } 
            
            // broadcasting an accept - nothing can trump that
            else if (message.charAt(0) == 'A')
            {
                message = message.replace ("ACCEPT ", "");
                output.writeUTF ("NO_RES ");
                input.close();
                output.close();
                socket.close ();
                return false;
            }
            
            current_nonce = Long.parseLong (message);
            
            if (new_nonce < current_nonce)
            {    
                // this processes nonce is too old
                output.writeUTF ("NO_RES ");
                
                // terminate the connection
                output.close ();
                input.close ();
                socket.close ();
                return false;
            }

            // if new broadcast should exist
            output.writeUTF ("UPDATE " + String.valueOf (new_nonce));
            
            // terminate the connection
            output.close ();
            input.close ();
            socket.close ();
            return true;
        }
        catch (Exception e)
        {
            // for debugging
            System.out.println(e);
        }
        return false;
    }
    
    // if the input string was a NO_RES
    private int parse_no_res (ServerSocket listen_socket, Socket connection_socket)
    {
        try
        {
            connection_socket.close ();
        }
        catch (Exception e)
        {
            // for debugging
            System.out.println(e);
        }
        return 0;
    }
    
    // if the input string was an UPDATE
    private int parse_update (String in_msg, long nonce, ServerSocket listen_socket, Socket connection_socket)
    {
        try
        {
            in_msg = in_msg.replace ("UPDATE ", "");
            long input_nonce = Long.parseLong (in_msg);
            if (input_nonce > nonce)
            {
                // terminate the server broadcast
                connection_socket.close ();
                System.out.println ("Terminating Broadcast.");
                listen_socket.close ();
                return 1;
            }
            connection_socket.close ();
            return 0;
        }
        
        catch (Exception e)
        {
            // for debugging
            System.out.println(e);
        }
        return 0;
    }
    
    // updates the consensus variable
    private void consensus_check ()
    {
        this.consensus = false;
        if (promises.size() >= (this.num_voters / 2) + 1)
        {
            this.consensus = true;
        }
        return ;
    }
    
    // if the input string was a PROMISE
    private int parse_promise (String in_msg, long nonce, ServerSocket listen_socket, Socket connection_socket)
    {
        try
        {
            int split = 0;
            int sender_id;
            int incoming_id;
            
            // checking where the split between the two values are
            in_msg = in_msg.replace ("PROMISE ", "");
            while (in_msg.charAt(split) != ' ')
            {
                split ++;
            }
            
            // converting input string into variables sender_id and incoming_id
            String temp = in_msg. substring (split+1);
            in_msg = in_msg.replace (temp, "");
            in_msg = in_msg.replace (" ", "");
            incoming_id = Helpers.string_to_int (temp);
            sender_id = Helpers.string_to_int (in_msg);
            
            // if sender ID is this process and the incoming's ID is its first time connecting.
            int index = promises.indexOf (incoming_id);
            if (sender_id == this.id)
            {
                if (index == -1)
                {
                    promises.add ((Integer) incoming_id);
                    consensus_check ();
                }
            }
            // if the sender ID is not this process, and they're in the table, then this process is out of date. (according to the sender)
            else
            {
                if (index != -1)
                {
                    promises.remove ((Integer) incoming_id);
                }
            }

            // terminating connection
            connection_socket.close ();
        }
        catch (Exception e)
        {
            System.out.println(e);
        }
        return 0;
    }
    
    // converts the input string into something useful
    private int parse_input (String in_msg, long nonce, ServerSocket listen_socket, Socket connection_socket)
    {
        // update nonce
        if (in_msg.charAt(0) == 'U')
        {
            return parse_update (in_msg, nonce, listen_socket, connection_socket);
        }
        
        // rcv promise
        else if (in_msg.charAt(0) == 'P')
        {
            return parse_promise (in_msg, nonce, listen_socket, connection_socket);
        }
        
        // rcv Accept
        else if (in_msg.charAt(0) == 'A')
        {
            return 0;
        }
        
        // no response
        return parse_no_res (listen_socket, connection_socket);        
    }

    // Executes a broadcast.
    public int broadcast ()
    {
        // Initialising variables
        ServerSocket listen_socket = null; // socket used for listening
        Socket connection_socket = null; // socket used for data transfer
        DataOutputStream output = null; // for sending data
        DataInputStream input = null; // for receiving data
        String in_msg = "";
        
        // do the broadcast
        try
        {
            // If there's already a broadcast, connect to broadcast instead.
            if (Helpers.is_port_active (port))
            {
                throw new Exception ("Port in use");
            }
            
            // establishing server socket
            listen_socket = new ServerSocket (port);
            listen_socket.setSoTimeout (seconds * 1000);
            
            while (true)
            {
                // establish connection with client
                connection_socket = listen_socket.accept ();
                
                // send the message nonce
                output = new DataOutputStream (connection_socket.getOutputStream ());
                output.flush ();
                
                // message depends on consensus
                if (this.consensus)
                {
                    output.writeUTF ("ACCEPT " + String.valueOf (message));
                }
                else
                {
                    output.writeUTF ("UPDATE " + String.valueOf (message));
                }
                
                // get response
                input = new DataInputStream (connection_socket.getInputStream ());
                in_msg = input.readUTF();
                System.out.println ("Receiving data: " + in_msg);
                
                if (parse_input (in_msg, message, listen_socket, connection_socket) == 1)
                {
                    return 0;
                }
                
                // rcv Accept - in case the server wasnt emitting the right signal
                if (in_msg.charAt(0) == 'A')
                {
                    in_msg = in_msg.replace ("ACCEPT ", "");
                    message = (long) Helpers.string_to_int(in_msg);
                    this.consensus = true;
                }
            }           
        }
        
        // assuming theres a broadcast going on the port already
        catch (Exception e)
        {
            // checking if error in port
            if (Helpers.is_port_active (port))
            {
                // broadcast new nonce
                if (broadcast_greater_nonce (port, message))
                {
                    this.broadcast ();
                }
                // don't broadcast
                else
                {
                    System.out.println ("Resetting nonce");
                    return -1;
                }
            }
            
            return 0;
        }
        
    }
    public void run ()
    {
        this.broadcast ();
    }
}
