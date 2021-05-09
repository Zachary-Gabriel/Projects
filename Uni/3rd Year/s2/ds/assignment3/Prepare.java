
// Executes the preparation stage of the Paxos algorithm
public class Prepare
{
    // private variables
    private int port;
    private int seconds;
    private int id; 
    private int largest_id;
    private int numb_voters;

    // creates the object
    public Prepare (int port, int seconds, int id, int largest_id, int numb_voters)
    {
        this.port = port;
        this.seconds = seconds;
        this.id = id; 
        this.largest_id = largest_id;
        this.numb_voters = numb_voters;
        return ;
    }

    // executes the preparation phase
    public void exe_prep ()
    {
        // broadcasts its unique ID / timestamp nonce
        long start_time = System.currentTimeMillis ();
        int temp_l_id = largest_id;
        
        // finding the amount of digits in the largest ID
        start_time = start_time * 10;
        while (temp_l_id != 0)
        {
            temp_l_id = temp_l_id / 10;
            start_time = start_time * 10;
        }

        // start time now represents the "nonce" and will be broadcast if possible
        start_time = start_time + id;
        System.out.println("Nonce: " + start_time);

        // Trying to save the broadcast of an ACCEPT-OK if the broadcast fails.
        try
        {
            // initialising variables
            long wait_time_start = System.currentTimeMillis ();
            long wait_time_end = System.currentTimeMillis ();
            boolean consensus = false;
            Broadcast broadcast = new Broadcast (this.id, port, seconds, start_time, numb_voters, consensus);
            long new_message = 0;

            while (true)
            {
                // multithreaded broadcast so multiple voters can send data at once.
                Thread t1 = new Thread(broadcast);
                Thread t2 = new Thread(broadcast);
                t1.start ();
                t1.run ();
                t2.start ();
                t2.run ();

                // lock the thread in an exponentially increasing loop to avoid live locks
                wait_time_start = System.currentTimeMillis ();
                while (wait_time_end- wait_time_start <= seconds * 1000)
                {
                    // If another Runner is broadcasting an ACCEPT-OK, save this so that this process can run it later if it fails
                    if (Helpers.is_port_active (port))
                    {
                        String bc_msg = Helpers.get_broadcast_message (port);
                        if (bc_msg.charAt(0) == 'A')
                        {
                            consensus = true;
                            new_message = (long) Long.parseLong (bc_msg.replace ("ACCEPT ", ""));
                        }
                    }

                    wait_time_end = System.currentTimeMillis ();
                }

                if (!consensus)
                {
                    seconds = seconds << 1; // exponential increase to remove live locks
                    this.exe_prep (); // run algorithm again
                }
                else
                {
                    // attempt to broadcast the ACCEPT-OK ID in the case that it fails
                    broadcast = new Broadcast (this.id, port, seconds, new_message, numb_voters, consensus);
                }
            }
        }
        catch (Exception e)
        {
            System.out.println(e);
        }
        
        return ;
    }
}
