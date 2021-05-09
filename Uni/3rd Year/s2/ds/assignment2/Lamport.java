import java.util.Random;

// responsible for the lamport clock implmentation
public class Lamport 
{
    // local clock of this process
    private int clock;

    // Constructor
    public Lamport()
    {
        Random rand = new Random ();
        this.clock = rand.nextInt (2020);
    }

    public int get_clock ()
    {
        return this.clock;
    }

    // given an int array of clocks, return the largest + 1
    public int next_clock (int a, int b)
    {
        if (a < b)
        {
            this.clock = b+1;
            return b+1;
        }
        this.clock = a+1;
        return a+1;
    }
}