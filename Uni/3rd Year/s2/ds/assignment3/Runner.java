
// A thread that runs for presidency is known as a 'Runner'
class Runner 
{
    // private variables
    private int id;

    // Driver for the prepare stage
    public void exe_member (String args[])
    {
        // variables
        this.id = Helpers.string_to_int (args[0]);
        int largest_id = Helpers.string_to_int (args[1]);
        int port = Helpers.string_to_int (args[2]);
        int seconds = 4;
        int num_of_voters = Helpers.string_to_int(args[3]);

        // Displaying the member ID
        System.out.println ("Runner");
        System.out.println ("ID: " + this.id);
        
        Prepare prep = new Prepare (port, seconds, id, largest_id, num_of_voters);
        prep.exe_prep ();
        return ;
    }

    // Executes the member's program
    public static void main (String args[])
    {
        Runner member = new Runner ();
        member.exe_member (args);

        return ;
    }
}
