import java.net.*; 
import java.io.*; 

// generic user that access the server's database
public class GETClient 
{
    public static void main(String[] args) 
    {
        // want to use the arg as a name to a file in which the xhtml document will be printed to
        // i.e. OutputBuffer webpage = args[0] + ".xhtml"
        // ideally, the ip address and port are also used as arguments

        try 
        {
            // establishing connection to server with 2-way traffic
            Socket socket = new Socket ("localhost", 8080);
            DataInputStream input  = new DataInputStream(socket.getInputStream()); 
            DataOutputStream output  = new DataOutputStream(socket.getOutputStream()); 
            System.out.println ("Connected");

            // clearing the output pipeline before sending data (a simple GET request without all the bells and whistles)
            output.flush ();
            output.writeUTF("GET");

            // printing the response to the terminal - plan to write this to a .xhtml doc
            System.out.println (input.readUTF());
            output.close ();
            input.close ();
        }
        // if can't connect to server
        catch (Exception e) 
        {
            System.out.println (e);
        }
    }
}
