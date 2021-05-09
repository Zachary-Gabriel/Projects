import java.net.*; 
import java.io.*;

// Executes the promise stage of the Paxos algorithm
public class Promise
{
    // promise variables
    private int promise_id = -1;
    private long promise_nonce = -1;
    private boolean accepted = false;

    // function parameters
    private int port = 8080;
    private int largest_id = 1;
    private int id;

    // creates the Promise object
    public Promise (int port, int largest_id, int id)
    {
        this.id = id;
        this.port = port;
        this.largest_id = largest_id;
        return ;
    }

    // assuming the promise policy is picking the member process that is greatest
    public void exe_promise ()
    {
        // Initialising variables for connecting
        Socket socket = null; // socket used for data transfer
        DataOutputStream output = null; // for sending data
        DataInputStream input = null; // for receiving data
        String msg;

        try
        {
            // establish connection with client
            socket = new Socket ("localhost", port);

            // getting nonce
            input = new DataInputStream (socket.getInputStream ());
            msg = input.readUTF();
            System.out.println ("Receiving data: " + msg);
            
            // invalid input
            if (msg == "")
            {
                input.close ();
                socket.close ();
            }

            // if the message type is an accept
            if (msg.charAt(0) == 'A')
            {
                msg = msg.replace ("ACCEPT ", "");
                accepted = true;
                this.promise_nonce = 0;
            }

            // if the message type is an update
            else if (msg.charAt(0) == 'U')
            {
                msg = msg.replace ("UPDATE ", "");
            }

            // The promise policy is one where once a process has promised a number, it won't promise numbers smaller than this.
            long promise_nonce = Long.parseLong (msg);
            if (promise_nonce > this.promise_nonce)
            {
                this.promise_nonce = promise_nonce;
                int largest_id = this.largest_id;
                int base = 1;

                // finding out the amount of digits in the largest ID.
                if (largest_id != 0)
                {
                    base *= 10;
                    largest_id /= 10;
                }

                // the last n-digits are used to store the ID of the sender.
                this.promise_id = (int) (this.promise_nonce % base) ;
                System.out.println ("base: " + base);
            }
            
            System.out.println ("msg: " + msg);
            System.out.println ("Current promise: " + this.promise_id);

            // send the message / nonce
            output = new DataOutputStream (socket.getOutputStream ());
            output.flush ();
            if (accepted)
            {
                output.writeUTF ("ACCEPT " + this.promise_id);
            }
            else
            {
                output.writeUTF ("PROMISE " + this.promise_id + " " + this.id);
            }
            
            // terminate connection and check loop
            input.close ();
            socket.close ();
        }
        // rough stuff
        catch (Exception e)
        {
            System.out.println(e);
        }
        return ;
    }

    // returns the promised id 
    public int get_promise_id ()
    {
        return this.promise_id ;
    }
    
    // returns the state of accepted 
    public boolean is_Accepted ()
    {
        return this.accepted ;
    }
}
