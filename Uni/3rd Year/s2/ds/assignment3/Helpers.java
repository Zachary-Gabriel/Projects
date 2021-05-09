import java.net.*; 
import java.io.*; 

// global helper functions that might be used in multiple other classes
public class Helpers 
{
    // converts a string of an integer to an int
    public static int string_to_int (String input)
    {
        int output = 0; // the output int
        int length = input.length(); // speed up computations

        for (int i = 0; i < length; ++i)
        {
            if ('0' <= input.charAt(i)  && input.charAt(i) <= '9')
            {
                output = 10 * output + input.charAt(i)-'0';
            }
            else
            {
                System.out.println("Invalid digit. Unicode of input: " + (int) input.charAt(i));
                System.exit(1);
            }
        }
        return output;
    }

    // Used to check if member can make the server socket
    public static boolean is_port_active (int port)
    {
        try
        {
            Socket socket = new Socket ("localhost", port);
            
            // get the input stream
            DataInputStream input = new DataInputStream (socket.getInputStream ());
            String in_msg = input.readUTF();

            // get the output stream and give the broadcaster an ACK
            DataOutputStream output = new DataOutputStream (socket.getOutputStream ());
            output.flush ();
            output.writeUTF ("NO_RES ");

            // terminate the connection
            socket.close ();
            return true;
        }

        // couldn't find the port, so it mustn't exist
        catch (Exception e)
        {
            return false;
        }
    }
    
    // Used to get the broadcast message without sending any data that might affect the algorithm
    public static String get_broadcast_message (int port)
    {
        try
        {
            Socket socket = new Socket ("localhost", port);
            
            // get the input stream
            DataInputStream input = new DataInputStream (socket.getInputStream ());
            String in_msg = input.readUTF();
            
            // get the output stream and give the broadcaster an ACK
            DataOutputStream output = new DataOutputStream (socket.getOutputStream ());
            output.flush ();
            output.writeUTF ("NO_RES ");

            // terminate the connection
            socket.close ();
            return in_msg;
        }
        
        // couldn't find the port, so it mustn't exist
        catch (Exception e)
        {
            // this is quite bad because caller's of this method expect there to be a broadcaster active
            System.out.println(e);
        }
        return "";
    }
}
