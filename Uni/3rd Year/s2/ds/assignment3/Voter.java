// A thread that votes is know as a 'voter'
public class Voter 
{
    // private variables
    private int id;
    private int max_id;
    private int port;

    // creates the object
    public Voter (int id, int max_id, int port)
    {
        this.id = id;
        this.max_id = max_id;
        this.port = port;
        return ;
    }

    // driver for the promise phase
    public void voter_exe ()
    {    
        System.out.println("Voter");
        System.out.println("ID: " + this.id);
        System.out.println("Max ID: " + this.max_id);
        System.out.println("Port: " + this.port);
        Promise promise = new Promise (port, max_id, id);
        while (!promise.is_Accepted())
        {
            promise.exe_promise ();
        }
        System.out.println ("The member voted in is: " + promise.get_promise_id());
    }
    public static void main(String[] args) 
    {
        if (args.length != 3)
        {
            System.out.println ("args.length should be 3");
            System.exit(1);
        }

        Voter voter = new Voter (Helpers.string_to_int (args[0]), Helpers.string_to_int (args[1]), Helpers.string_to_int (args[2]));
        voter.voter_exe ();
        return ;
    }
}
